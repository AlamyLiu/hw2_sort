
#include <sstream>
#include <algorithm>		/* std::reverse */
#include <iomanip>		/* std::setfill, std::setw */
#include "sidll.hpp"

using namespace std;

/*
 * LSN: Least Significant Node
 * MSN: Most Significant Node
 */
SIDLList::SIDLList(
	int sign,
	string& num,
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

int SIDLList::insertAtTail( string value )
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

int SIDLList::insertAtHead( string value )
{
	insertAtTail( value );
	pHead = pHead->prev;

	return 0;
}

int SIDLList::insertAtTail( int value )
{
	std::stringstream ss;

	ss << std::setfill('0') << std::setw(digit_per_node) << value;
	insertAtTail( ss.str() );
}

int SIDLList::insertAtHead( int value )
{
	insertAtTail( value );
	pHead = pHead->prev;

	return 0;
}

/* Return: -1) smaller; 0) equal; 1) bigger */
int SIDLList::compare(SIDLList *his)
{
	int rc;

#if 1	/* This should not happen */
	if ((this->list_len == 0) || (his->list_len == 0)) {
		cerr << "What's going on ?" << endl;
		return -2;
	}
#endif

	/* 1. Compare sign */
	if (this->sign != his->sign)
		return this->sign;

	/* 2. Compare length (same sign) */
	/* Assume it's Positive, reverse result if Negative */
	rc = this->list_len - his->list_len;
	if (rc != 0) {
		/* Adjust to (-1) & (1) */
		rc = (rc > 0) ? 1 : (-1);

		/* Reverse if Negative */
		if (this->sign == -1)
			rc = -rc;

		return rc;
	}

	/* 3. Compare value (same sign, same length) */
	Node *pThis = this->pHead;
	Node *pHis  = his->pHead;
	for (int len = 0; len < this->list_len; len++) {
		int myValue = stoi(pThis->value);
		int hisValue = stoi(pHis->value);

		rc = (myValue - hisValue);
		if (rc != 0) {
			rc = (rc > 0) ? 1 : (-1);
			if (this->sign == -1)
				rc = -rc;
			break;
		}

		pThis = pThis->next;
		pHis  = pHis->next;
	}

	return rc;
}


#if 0
std::ostream &operator<<(std::ostream &out, const SIDLList &L)
{
	SIDLList::Node *pNode = L.pHead;

	out << "SIDLList: (" << L.sign << ") ";
	while (true) {
		out << pNode->value;
		pNode = pNode->next;

		if (pNode == L.pHead)
			break;
		else
			out << " ";
	}
	out << std::endl;

	return out;
}
#endif

/* ------------------------------------------------------------ */
#if 0	/* Replaced by <list> class */

/* Append to tail */
int INTList::addInteger( string signInteger )
{
	Node *pNode = new(nothrow) Node( signInteger );
	if (!pNode) {
		return -ENOMEM;
	}

	return 0;
}

/*
 *
 */
INTList::Node::Node( string signInteger )
{
	string str_sign ("+-");
	string str_digit("0123456789");
	int sign = 1;		/* Default to positive '+' */

	size_t pSign  = signInteger.find_first_of( str_sign );
	size_t pValue = signInteger.find_first_not_of( str_sign );

	if (pSign == pValue) {
		/* No sign: default */
	} else if (pSign + 1 != pValue) {
		/* Wrong format */
		if (pValue < pSign) {
			/* [:digit:][+-] */
		} else {
			/* [+-]+[:digit:] */
		}
		cerr << "Wrong Format: " << signInteger << endl;
		return;
	} else {
		/* pSign < pValue */
		if (signInteger.at(pSign) == '-')
			sign = -1;

		/* If it's not '-', it must be '+' : find_first_of() */
		/* We don't handle '+' case, as it's default case */
	}

	string integer = signInteger.substr(pValue);	/* [[:digit:]] */
	if (integer.find_first_not_of( str_digit ) != std::string::npos) {
		/* [+-][[:digit:]]\w */
		cerr << "Wrong format: " << signInteger << endl;
		return;
	}

#if 0
	this.integer
#endif
}

#endif
