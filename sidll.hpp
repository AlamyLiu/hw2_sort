/*
 * Signed Integer Double Linked-List
 */

#ifndef	_SIDLLIST_HPP_
#define	_SIDLLIST_HPP_

#include <string>
#include <iostream>		/* std::cout, std::endl */

using namespace std;

#if 0
namespace sidll
{

}
#endif

class SIDLList {

protected:
	class Node {
		friend class SIDLList;
		friend ostream &operator<<(ostream &out, const SIDLList &L);

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
	SIDLList ( int sign, string num, int digits );
	SIDLList ( string num, int digits )
		: SIDLList( 1, num, digits )
		{};
	SIDLList ( string num )
		: SIDLList( num, 1 )
		{};
	SIDLList ()
		: SIDLList( string("0"), 1 )
		{};

#if 0
	SIDLList () {
		head = NULL;
		tail = NULL;

		digit_per_node = 0;
		sign = 1;
		node_num = 0;
	};
#endif
	~SIDLList() {
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
#if 0
	friend ostream& operator<<(ostream& out, const SIDLList& list);
#else
	friend ostream &operator<<(ostream &out, const SIDLList &L) {
		Node *pNode = L.pHead;

		while (true) {
			out << pNode->value;
			pNode = pNode->next;

			if (pNode == L.pHead)
				break;
			else
				out << " ";
		}
	}

#endif
};


class INTList
{
private:
	class Node {
		friend class INTList;
		SIDLList integer;
		Node *prev;
		Node *next;

		Node( string signInteger );
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

#endif	/* _SIDLLIST_HPP_ */
