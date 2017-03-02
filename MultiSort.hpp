/*
    Header of MultiSort class

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

#ifndef	_MY_SORT_HPP_
#define	_MY_SORT_HPP_

#include <list>
#include <iostream>
#include <fstream>      /* ofstream */
#include "sidll.hpp"


enum SORT_ALGORITHM {UNKNOWN, SELECT, INSERT, MERGE, HEAP, QUICK};

class MultiSort
{
	friend std::ostream &operator<<(std::ostream &out, const MultiSort &S);

private:
	std::list<SIDLList*> intList;

	unsigned int	sort_compare_count;
	unsigned int	sort_swap_count;
	unsigned int	bigO_compare_count;
	unsigned int	bigO_swap_count;

protected:
	void resetCount() {
		sort_compare_count = 0;
		sort_swap_count = 0;
		bigO_compare_count = 0;
		bigO_swap_count = 0;
	}

public:
	/* Constructor/Destructor */
	MultiSort() {};
	~MultiSort() {};

	/* Sorting methods */
	int SelectSort();
	int InsertSort();
	int MergeSort();
	int HeapSort();
	int QuickSort();

	/* Misc methods */
	int addItem( SIDLList* num ) {
		intList.push_back( num );
	}

	unsigned int size() {
		return intList.size();
	}

	void free() {
		for (const auto& i : intList) {
			delete i;
		}
		intList.clear();
	}

	void statistics( std::ofstream& ofs, std::string& algorithm );

#if 0
	friend std::ostream &operator<<(std::ostream &out, const MultiSort &S) {
		for (const auto& i : S.intList) {
//			out << const_cast<SIDLList*>(i);
			out << *i;
		}

		out << std::endl;
		return out;
	}
#endif


};


#endif	/* _MY_SORT_HPP_ */
