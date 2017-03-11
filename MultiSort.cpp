/*
    Implementation of MultiSort class

    Copyright (C) 2017  Alamy Liu <alamy.liu@gmail.com>


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iomanip>		/* setw */
#include <cerrno>
#include "DebugW.hpp"
#include "MultiSort.hpp"

extern Debug	*dbg;

/* ------------------------------------------------------------
 * Sorting methods
 */
int MultiSort::SelectSort()
{
	int rc;

	*dbg << "Select sort ... " << std::endl;
	resetCount();
	typename std::list<SIDLList*>::iterator i, j, min;

	/* iterator 'i' walks over the solt, moving the smallest to it */
	for (i = intList.begin(); i != intList.end(); i++) {
		min = i;
		*dbg << "compare: ";

		/* iterator 'j' walks over the rest, find the smallest one */
		j = std::next( i );
		for ( ; j != intList.end(); j++) {
			rc = (*min)->compare(*j);
			bigO_compare_count++;
			sort_compare_count++;
			*dbg << std::setw(2) << rc << ", ";

			/* found smaller one */
			if (rc > 0)
				min = j;
		}
		*dbg << std::endl;

		/* Now we got the smllest one, move it to solt 'i' */
		bigO_swap_count++;
		if (min != i) {
			std::swap(*i, *min);
			sort_swap_count++;
		}
	}

	return 0;
}

int MultiSort::InsertSort()
{
	int rc;

	*dbg << "Insertion sort ... " << std::endl;
	resetCount();
	typename std::list<SIDLList*>::iterator i, j, key;

	/* iterator 'i' walks over the list */
	/* Skip the 1st one (sorted) */
	i = std::next( intList.begin() );
	for ( ; i != intList.end(); i++) {
		key = i;	/* Compare with front sorted list */
		*dbg << *key << ":";

		do {
			j = std::prev( key );

			/* Compare with previous value */
			rc = (*key)->compare( *j );
			bigO_compare_count++;
			sort_compare_count++;
			*dbg << std::setw(2) << rc << ", ";

			/* Swap if 'key' is smaller (rc < 0) */
			bigO_swap_count++;
			if ( rc < 0 ) {
				std::swap(*j, *key);
				sort_swap_count++;

				/* swap() doesn't touch iterator */
				key--;	/* adjust 'key' iterator */
			}
		} while ((j != intList.begin()) && (rc < 0));

		/* Adjust bigO: worst case = compare/swap every one */
		while ( j != intList.begin() ) {
			bigO_compare_count++;
			bigO_swap_count++;
			j--;
		}

		*dbg << std::endl;
	}

	return 0;
}

/*
 *  0   1   2   . . .   h-1   h   h+1   . . .   n-1   n
 *   \                    /  iHalf                    /
 *    \___ left (iL) ____/      \____ right (iR) ____/
 */
int MultiSort::_merge_sort(std::list<SIDLList*> *left, int depth)
{
	int rc;
	std::list<SIDLList*> right;

	/* only one element, nothing to sort */
	if (left->size() <= 1) {
		return 0;
	}

#if 0
	/* for debugging */
	if (depth > 2) {
		return 0;
	}
#endif

	*dbg << "-------------------- depth = " << depth << std::endl;
	*dbg << *left << std::endl;

	/* iterator::iHalf points to the middle element */
	typename std::list<SIDLList*>::iterator iHalf;
	iHalf = left->begin();
	advance(iHalf, (left->size() >> 1) );

	/* Split list into Left & Right */
	/* [0..iHalf-1, iHalf..n] */
	right.splice(right.begin(), *left, iHalf, left->end());

	*dbg << "left# = " << left->size() << std::endl;
	*dbg << *left;
	*dbg << "right# = " << right.size() << std::endl;
	*dbg << right;


	/* Recurse to sort each half */
	_merge_sort(  left, depth+1 );
	_merge_sort(&right, depth+1 );


	/* Merge left & right */
	typename std::list<SIDLList*>::iterator iL, iR;
	iL = left->begin();
	iR = right.begin();	/* actually: iR alway == right.begin() */

	/* single elimination match */
	bigO_compare_count = left->size() + right.size() - 1;

	while ( (iL != left->end()) || (iR != right.end()) ) {
		/* 'left' is empty, get everything from 'right' */
		if (iL == left->end()) {
			left->splice(iL, right);
			break;
		}

		/* 'right' is empty, get everything from 'left' */
		if (iR == right.end()) {
			/* we use left as base, nothing to do */
			break;
		}

		rc = (*iL)->compare( *iR );
		sort_compare_count++;

		/* left < right: get one item from left
		 * left > right: get one item from right
		 * left == right: get from left & right (total = two items)
		 */
		if (rc <= 0) {
			/* we use left as base, just move to next */
			/* 0   1   2   3   . . .   n
			 *     iL
			 *         iL
			 */
			advance( iL, 1 );
		}
		if (rc >= 0) {
			/*  left: 4 4 *8 10 18    : *iL = 8
			 * right: *7 15 16        : *iR = 7
			 *
			 *  left: 4 4 7 *8 10 18  : *iL = 8
			 * right: 15 16
			 */
			left->splice( iL, right, right.begin() );
			iR = right.begin();
		}
	}

	*dbg << "merge# = " << left->size() \
		<< " (depth = " << depth << ")" << std::endl;
	*dbg << *left;

	return 0;
}

int MultiSort::MergeSort()
{
	int rc;

	*dbg << "Merge sort ... " << std::endl;

	resetCount();
	_merge_sort( &intList );

	return 0;
}

/* Most heap-sort operation is based on Array
 *
 * 0-based index
 *	 left child: 2n + 1
 *	right child: 2n + 2
 *
 * 1-based index
 *	 left child: 2n
 *	right child: 2n + 1
 *
 * Since this code use 'list' structure, will use list methods to emulate
 * Array operations. i.e.: advance(iterator.begin(), n).
 *
 * Parameter
 *	*tree: the tree to heapify
 *	i: index of the parent node
 *	depth: recursive call depth
 */
int MultiSort::_heapify(std::list<SIDLList*> *tree, int i, int depth)
{
	int	size, left, right, max, rc;
	std::list<SIDLList*>::iterator pNode, pLeft, pRight, pMax;

	/* 0-based formula (we surely have left child) */
	size = tree->size();
	left = (i << 1) + 1;
	right = left + 1;

	if (size <= 1) {
		/* Empty or only 1 element */
		return 0;
	}
	if (left >= size) {
		std::cerr << "ERROR: Tree size = " << size \
			<< ", left child index = " << left << std::endl;
		/* Something wrong, need debugging */
		return -EPERM;
	}

	/* index -> iterator */
	pNode = tree->begin();	advance( pNode, i );
	pLeft = pNode;		advance( pLeft, left - i );
	pRight = pLeft;		advance( pRight, 1 );

	*dbg << "----- size=" << size \
		<< ", node=[" << i \
		<< "], l=[" << left \
		<< "], r=[" << right << "]" << std::endl;
	*dbg << "pNode = " << *pNode \
		<< ", pLeft = " << *pLeft;
	*dbg << ", pRight = ";
	if (pRight == tree->end()) {
		*dbg << "(nil)";
	} else {
		*dbg << *pRight;
	}
	*dbg << std::endl;

	/* Find the maximum one among the three */
	bigO_compare_count++;
	sort_compare_count++;
	rc = (*pNode)->compare( *pLeft );
	if (rc >= 0) {
		pMax = pNode;
		max = i;
	} else {
		pMax = pLeft;
		max = left;
	}
	if (pRight != tree->end()) {
		bigO_compare_count++;
		sort_compare_count++;
		rc = (*pMax)->compare( *pRight );
		if (rc < 0) {
			pMax = pRight;
			max = right;
		}
	}
	*dbg << "Max[" << max << "]=" << *pMax << std::endl;

	/* Maximum one is in child */
	bigO_swap_count++;
	if (pNode != pMax) {
		sort_swap_count++;
		std::swap( *pNode, *pMax );
		/* Only if it has child */
		if (((max << 1) + 1) < size) {
			_heapify( tree, max, depth+1 );
		}
	} else {
		/* Simulate the bigO case, go as deep as child goes */
		max = (max << 1) + 1;	/* dive into left child */
		while (max < size) {
			bigO_compare_count++;
			/* right child */
			if (max + 1 < size) {
				bigO_compare_count++;
			}
			bigO_swap_count++;

			/* dive into left child */
			max = (max << 1) + 1;
		}
	}

	*dbg << *tree;

	return 0;
}

/*
 * This code will implement Maximum-Heap tree
 * Like the examples at:
 *	https://www.cs.usfca.edu/~galles/visualization/HeapSort.html
 *	http://faculty.simpson.edu/lydia.sinapova/www/cmsc250/LN250_Weiss/L13-HeapSortEx.htm
 *
 */
int MultiSort::HeapSort()
{
	int	i;

	*dbg << "Heap sort ... " << std::endl;

	resetCount();

	/* Reversely walk: heapify each parent node */
	i = (intList.size() >> 1);
	if (i == 0) {
		/* Only 0 or 1 number, nothing to sort */
		return 0;
	}

	*dbg << "heapify ..." << std::endl;
	/* 0-based node index */
	while (i--) {
		_heapify( &intList, i );
	}

	*dbg << "sorting ..." << std::endl;
	/* Now the tree is max-heapified, time to sort */
	std::list<SIDLList*> sort;
	while (intList.size() > 1) {
		/* move the 1st/maximum one to the front of 'sort' list */
		sort.push_front( intList.front() );
		intList.pop_front();

		/* move the last one to the top */
		intList.push_front( intList.back() );
		intList.pop_back();

		bigO_swap_count++;
		sort_swap_count++;

		_heapify( &intList, 0 );
	}	/* End of while */

	/* Finally, move it back */
	intList.splice( intList.end(), sort, sort.begin(), sort.end() );

	return 0;
}

/* Sort a range of list */
/* Pivot:
 *	For list, it's harder to use the middle one.
 *	Just use the first element, list->begin().
 * Sorting:
 *	Ascending order
 */
int MultiSort::_quick_sort(std::list<SIDLList*> *left, int depth)
{
	int rc;
	std::list<SIDLList*> right;

	/* only one element, nothing to sort */
	if (left->size() <= 1) {
		return 0;
	}

#if 0
	/* for debugging */
	if (depth > 3) {
		return 0;
	}
#endif

	*dbg << "-------------------- depth = " << depth << std::endl;
	*dbg << *left << std::endl;

	typename std::list<SIDLList*>::iterator i, p, boundary;

	/* range to sort: [p, 1 .. n] */
	p = left->begin();	/* Pivot = first element */
	boundary = p;		/* boundary of 'left'/'right' list*/

	/* Partition */
	/* Walk through list, move smaller one to left side */
	for (i = std::next( p ); i != left->end(); i++) {

		rc = (*i)->compare( *p );
		bigO_compare_count++;
		sort_compare_count++;

		/* Found a smaller one */
		bigO_swap_count++;
		if (rc < 0) {
			advance(boundary, 1);
			std::swap( *i, *boundary );
			sort_swap_count++;
		}
	}
	/* Now: [pivot, 1..boundary, boundary+1..n]
	 * all value in 1..boundary < pivot
	 */

	/* Note: Could not swap pivot with boundary+1
	 * or [0] might > pivot after swap
	 */
	std::swap( *p, *boundary );
	bigO_swap_count++;
	sort_swap_count++;

	/* Now: value[0..boundary-1] < value(pivot) <= value[boundary..n] */

	/* Split list into Left & Right */
#if 1
	advance(boundary, 1);
	/* [0..boundary, boundary+1..n] */
	/* Left  = [0..boundary] */
	/* Right = [boundary+1..n] */
#else
	/* ERROR: 2 elements: 5 5 */

	/* [0..boundary-1, boundary..n] */
	/* [0..boundary-1] < pivot <= [boundary..n] */
	/* Left  = [0..boundary-1] */
	/* Right = [boundary..n] */
#endif
	right.splice(right.begin(), *left, boundary, left->end());

	*dbg << "left# = " << left->size() << std::endl;
	*dbg << *left;
	*dbg << "right# = " << right.size() << std::endl;
	*dbg << right;


	/* Recursive to sort Left & Right list */
	if (left->size() > 1)	_quick_sort(  left, depth+1 );
	if (right.size() > 1)	_quick_sort( &right, depth+1 );


	/* merge back */
	left->splice( left->end(), right, right.begin(), right.end() );
}

int MultiSort::QuickSort()
{
	*dbg << "Quick sort ... " << std::endl;

	resetCount();
	_quick_sort( &intList );

	return 0;
}

/* ------------------------------------------------------------ */


void MultiSort::statistics( std::ofstream& ofs, std::string& algorithm )
{
	/* New file, need a header */
	if (ofs.tellp() == 0) {
		ofs << "algorithm  #numbers(n)  #Comparisons  #Swaps" \
			"  #bigOComparisons  #bigOSwaps" << std::endl;
	}

	ofs << std::setw( 9) << algorithm;
	ofs << std::setw(13) << size();
	ofs << std::setw(14) << sort_compare_count;
	ofs << std::setw( 8) << sort_swap_count;
	ofs << std::setw(18) << bigO_compare_count;
	ofs << std::setw(12) << bigO_swap_count;
	ofs << std::endl;
}

/* ------------------------------------------------------------ */

std::ostream &operator<<(std::ostream &out, const MultiSort &S)
{
	for (const auto& i : S.intList) {
		out << *i;
	} /* End of for(i) */

	out << std::endl;
	return out;
}

std::ostream &operator<<(std::ostream &out, const std::list<SIDLList*> &L)
{
	for (const auto& i : L) {
		out << *i;
	} /* End of for(i) */

	out << std::endl;
	return out;
}

int MultiSort::test()
{
	*dbg << "----- Testing -----" << std::endl;

	resetCount();

#if 0
	typename std::list<SIDLList*>::iterator i, j, key;

	/* "<<" overloading operation testing */
	i = intList.begin();
	j = std::next( i );

	std::cout << "----- compare -----" << std::endl;
	if ((*i)->compare(*j) < 0)
		std::cout << *i << " < " << *j;
	if ((*i)->compare(*j) == 0)
		std::cout << *i << " == " << *j;
	if ((*i)->compare(*j) > 0)
		std::cout << *i << " > " << *j;
	std::cout << std::endl;

	std::cout << "----- overload -----" << std::endl;
	if (*i < *j)
		std::cout << *i << " < " << *j;
	if (*i == *j)
		std::cout << *i << " == " << *j;
	if (*i > *j)
		std::cout << *i << " > " << *j;
	std::cout << std::endl;
#endif

#if 1
	/* Heap right child detection testing */
	int	i, left, right, size;
	std::list<SIDLList*> *tree;
	std::list<SIDLList*>::iterator pNode, pLeft, pRight;

	tree = &intList;
	i = (tree->size() >> 1);
	if (i == 0) {
		/* Only 0 or 1 number, nothing to sort */
		std::cout << "Empty or only 1 element" << std::endl;
		return -ENOSYS;
	}
	/* 0-based node index */
	while (i--) {
		left = (i << 1) + 1;
		right = left + 1;

		std::cout << "Node[" << i \
			<< "], l=[" << left \
			<< "], r=[" << right \
			<< "]" << std::endl;

		if (right >= size -1 )

		pNode = tree->begin();	advance( pNode, i );
		pLeft = pNode;		advance( pLeft, left - i );
		pRight = pLeft;		advance( pRight, 1 );

		std::cout << "pNode = " << *pNode \
			<< ", pLeft = " << *pLeft;
		std::cout << ", pRight = ";
		if (pRight == tree->end()) {
			std::cout << "Nil";
		} else {
			std::cout << *pRight;
		}
		std::cout << std::endl;

		advance( pRight, 3 );
		std::cout << "pRight+3 = ";
		if (pRight == tree->end()) {
			std::cout << "Nil";
		} else {
			std::cout << *pRight;
		}
		std::cout << std::endl;
	}


#endif

	/* Simulate an unsupported return */
	return -ENOSYS;
}
