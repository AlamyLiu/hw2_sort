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

int MultiSort::MergeSort()
{
	std::cerr << "MergeSort: Not supported yet!" << std::endl;

	return -ENOSYS;
}

int MultiSort::HeapSort()
{
	std::cerr << "HeapSort: Not supported yet!" << std::endl;

	return -ENOSYS;
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

	typename std::list<SIDLList*>::iterator i, j, key;

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

	/* Simulate an unsupported return */
	return -ENOSYS;
}
