
#include <sstream>
#include <algorithm>		/* std::reverse */
#include <iomanip>		/* std::setfill, std::setw */
#include "sidll.hpp"

/*
 * LSN: Least Significant Node
 * MSN: Most Significant Node
 */
SignedIntegerDoubleLinkedList::SignedIntegerDoubleLinkedList(
	int sign,
	string num,
	int digits
) {
	this->sign = sign;
	// Reverse the number, so it would be aligned from LSN
	std::reverse(num.begin(), num.end());
	string value;

	for (int i = 0; i < num.length(); i += digits) {
		value = num.substr(i, digits);
		reverse(value.begin(), value.end());

		insertAtHead( value );
	}
}

int SignedIntegerDoubleLinkedList::insertAtTail( string value )
{
	Node *pNode = new(nothrow) Node();
	pNode->value = value;

	if (isEmpty()) {
		pHead = pNode;
		pNode->prev = pNode;
		pNode->next = pNode;
	} else {
		Node *pTail = pHead->prev;

		pNode->prev = pTail;
		pNode->next = pHead;

		pTail->next = pNode;
		pHead->prev = pNode;
	}

	list_len++;
}

int SignedIntegerDoubleLinkedList::insertAtTail( int value )
{
    std::stringstream ss;

    ss << std::setfill('0') << std::setw(digit_per_node) << value;
    insertAtTail( ss.str() );
}

int SignedIntegerDoubleLinkedList::insertAtHead( string value )
{
	insertAtTail( value );
	pHead = pHead->prev;

	return 0;
}



/* Append to tail */
int INTList::addInteger( string integer )
{
	Node *pNode = new(nothrow) Node( integer );
	if (!pNode) {
		return -ENOMEM;
	}

	return 0;
}

/*
 *
 */
INTList::Node::Node( string integer )
{
	string str_sign ("+-");
	int sign = 1;		/* Default to positive '+' */

	size_t pSign  = integer.find_first_of( str_sign );
	size_t pValue = integer.find_first_not_of( str_sign );

	if (pSign == pValue) {
		/* No sign: default */
	}
	else if (pSign < pValue) {

	} else if (pValue < pSign) {
		/* Wrong format : [:digit:][+-] */
	}



}

