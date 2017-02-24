/*
 * Signed Integer Double Linked-List
 */

#ifndef	_SIDLLIST_HPP_
#define	_SIDLLIST_HPP_

#include <string>
#include <iostream>		/* std::cout, std::endl */


/* To store singed infinite integer */
class SIDLList {

protected:
	class Node {
		friend class SIDLList;
		friend std::ostream &operator<<(std::ostream &out, const SIDLList &L);

		std::string value;
		Node *prev;
		Node *next;
	};

	Node	*pHead;

	int	digit_per_node;	/* How many digits per Node */
	int	sign;		/* POSITIVE (1) or NEGATIVE (-1) */
	int	list_len;	/* List length (Number of Node) */

public:
	/* Constructor / Destructor */
	SIDLList ( int sign, std::string num, int digits );
	SIDLList ( std::string num, int digits )
		: SIDLList( 1, num, digits )
		{};
	SIDLList ( std::string num )
		: SIDLList( num, 1 )
		{};
	SIDLList ()
		: SIDLList( std::string("0"), 1 )
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
		while (list_len--) {
			pNode = pHead;
			pHead = pHead->next;
			delete pNode;
		};
	}

	/* Element(Node) operation */
	int insertAtTail( std::string value );
	int insertAtHead( std::string value );
	int insertAtTail( int value );
	int insertAtHead( int value );

	/* List operation */
	bool isEmpty() {
		return (pHead == NULL);
	}
#if 0
	friend ostream& operator<<(ostream& out, const SIDLList& list);
#else
	friend std::ostream &operator<<(std::ostream &out, const SIDLList &L) {
		Node *pNode = L.pHead;

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
	}

#endif
};

#if 0	/* Replaced by <list> class */
class INTList
{
private:
	class Node {
		friend class INTList;
		SIDLList integer;
		Node *prev;
		Node *next;

		Node( std::string signInteger );
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

	int	addInteger( std::string integer );	/* Append to tail */

};
#endif


#endif	/* _SIDLLIST_HPP_ */
