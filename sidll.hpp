/*
 * Signed Integer Double Linked-List
 */

#ifndef	_SIDLL_HPP_
#define	_SIDLL_HPP_

#include <string>
#include <iostream>		/* std::cout, std::endl */

using namespace std;

#if 0
namespace sidll
{

}
#endif

class SignedIntegerDoubleLinkedList {

private:
	class Node {
		friend class SignedIntegerDoubleLinkedList;
		string value;
		Node *prev;
		Node *next;
	};

	Node	*pHead;
	Node	*pTail;

	int	digit_per_node;	/* How many digits per Node */
	int	sign;		/* POSITIVE (1) or NEGATIVE (-1) */
	int	list_len;	/* List length (Number of Node) */

public:
	/* Constructor / Destructor */
	SignedIntegerDoubleLinkedList ( int sign, string num, int digits );
	SignedIntegerDoubleLinkedList ( string num, int digits )
		: SignedIntegerDoubleLinkedList( 1, num, digits )
		{};
	SignedIntegerDoubleLinkedList ( string num )
		: SignedIntegerDoubleLinkedList( num, 1 )
		{};
	SignedIntegerDoubleLinkedList ()
		: SignedIntegerDoubleLinkedList( string("0"), 1 )
		{};

#if 0
	SignedIntegerDoubleLinkedList () {
		head = NULL;
		tail = NULL;

		digit_per_node = 0;
		sign = 1;
		node_num = 0;
	};
#endif
	~SignedIntegerDoubleLinkedList() {
		Node *pNode;
		while (pHead) {
			pNode = pHead;
			pHead = pHead->next;
			delete pNode;
		};
	}

	/* Element(Node) operation */
	int insertAtTail( string value );
	int insertAtHead( string value );
	int insertAtTail( int value );
	int insertAtHead( int value );

	/* List operation */
	bool isEmpty() {
		return (pHead == NULL);
	}

};

class INTList
{
private:
	class Node {
		friend class INTList;
		SignedIntegerDoubleLinkedList integer;
		Node *prev;
		Node *next;

		Node( string integer );
	};

	Node	*pHead;
	Node	*pTail;
	int	list_len;	/* List length (Number of Node) */

public:
	/* Constructor / Destructor */
	INTList() : pHead(NULL), pTail(NULL), list_len(0) {};
	~INTList() {
		Node *pNode;
		while (pHead) {
			pNode = pHead;
			pHead = pHead->next;
			delete pNode;
		};
	}

	int	addInteger( string integer );	/* Append to tail */

};


#if 0
class NUMBER
#endif

#endif	/* _SIDLL_HPP_ */
