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
	std::cerr << "InsertSort: Not supported yet!" << std::endl;

	return 0;
}

int MultiSort::MergeSort()
{
	std::cerr << "MergeSort: Not supported yet!" << std::endl;

	return 0;
}

int MultiSort::HeapSort()
{
	std::cerr << "HeapSort: Not supported yet!" << std::endl;

	return 0;
}

int MultiSort::QuickSort()
{
	std::cerr << "QuickSort: Not supported yet!" << std::endl;

	return 0;
}

/* ------------------------------------------------------------ */


void MultiSort::statistics( std::string& oFile, std::string& algorithm )
{
	/* if oFile does not exist
	 *  a) create file
	 *  b) header
	 */

#if 0	/* Header */
	algorithm  #numbers(n)  #Comparisons  #Swaps  #bigOComparisons  #bigOSwaps
#endif
	/* Append to file the statistics */

	/* Ignore file for now */
	std::cout << "algorithm  #numbers(n)  #Comparisons  #Swaps" \
		"  #bigOComparisons  #bigOSwaps" << std::endl;

	std::cout << std::setw( 9) << algorithm;
	std::cout << std::setw(13) << size();
	std::cout << std::setw(14) << sort_compare_count;
	std::cout << std::setw( 8) << sort_swap_count;
	std::cout << std::setw(18) << bigO_compare_count;
	std::cout << std::setw(12) << bigO_swap_count;
	std::cout << std::endl;
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

