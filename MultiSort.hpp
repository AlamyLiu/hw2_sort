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

enum SORT_ALGORITHM {UNKNOWN, SELECT, INSERT, MERGE, HEAP, QUICK};

class SIDLList;

class MultiSort
{
private:
	std::list<SIDLList*> intList;

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
	int addItem( SIDLList* num );

	int printAllItems();
};

#endif	/* _MY_SORT_HPP_ */
