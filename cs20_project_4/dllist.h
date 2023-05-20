#ifndef S23_CS20A_LlST_H
#define S23_CS20A_LlST_H

#include<iostream>
#include<assert.h>
// Linked DLList object that maintains both front and rear pointers
// and the size of the list.  Note that you have to keep the front,
// rear and size consistent with the intended state of the DLList 
// otherwise very bad things happen. 
template<typename Type>
class DLList {
public:

	DLList();

	DLList(const DLList<Type>& other);

	DLList<Type>& operator=(const DLList<Type>& other);

	~DLList();

	void	print() const;
	bool	empty() const;

	void	insert_front(const Type &value);
	void	insert_rear(const Type &value);
	void	insert(int idx, const Type &value);

	// Note that the user must front ensure the list is not empty
	// prior to calling these functions. 
	Type	peek_front() const;
	Type	peek_rear() const;
	Type	peek(int idx) const;

	int		count() const;
	int		count_value(const Type &value) const;

	int		find(const Type &value) const;

	bool	remove_front();
	bool	remove_rear();
	bool	remove_index(int idx);
	int		remove_value(const Type &value);

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	// Forward declare the nodes for our DLList.
	// Will be implemented along with list
	// member functions
	class DLNode;

	// We'll have both front and rear points for 
	// Fast insertion/deletion from both ends.
	DLNode*	front;
	DLNode*	rear;

	// Keep track of number of nodes in the list
	int		size;
};


/* DLList Implementation
//
//  Since DLList is a template class (which is not an actual
//  class yet, not until we actually instantiate the list)
//  we need to keep the implementation together with
//  the definition.  There are ways to simulate having
//  separate "implementation/definition" with templates,
//  but they aren't necessary and can be confusing.
*/

/* DLNode definition
//		Already implemented, nothing to do here but to use it.
*/
template<typename Type>
class DLList<Type>::DLNode {
public:
	DLNode() :nxt(nullptr), prv(nullptr) {}
	DLNode(Type v, DLNode* p, DLNode* n) : val(v), nxt(n), prv(p) {}

	DLNode * next() const { return nxt; }
	void next(DLNode *n) { nxt = n; }

	DLNode * prev() const { return prv; }
	void prev(DLNode *p) { prv = p; }

	Type value() const { return val; }
	void value(const Type &v) { val = v; }

private:
	Type  val;
	DLNode * nxt;
	DLNode * prv;
};



/* DLList default constructor
//		Set front and rear pointer to point to nothing, size is zero
//		Already implemented, nothing to do.
*/
template<typename Type>
DLList<Type>::DLList() :front(nullptr), rear(nullptr), size(0) {
	/*    COMPLETE    */
}


/* Copy constructor
*/
template<typename Type>
DLList<Type>::DLList(const DLList<Type>& other) {

	if (other.empty()) {
		size = 0;
		front = nullptr;
		rear = nullptr;
	}

	if (other.count() == 1) {
		DLNode* ptr = other.front;
		
		insert_front(ptr->value());
		rear = front;
	}

	if (other.count() > 1) {

		DLNode* ptr = other.front;
		for (int i = 0; i < other.count(); i++) {

			insert_rear(ptr->value());
			ptr = ptr->next();
		}

	}

}
/* Overloaded assignment operator
*/
template<typename Type>
DLList<Type>& DLList<Type>::operator=(const DLList<Type>& other) {

	if (this == &other) {
		return *this;
	}

	while (front) {
		remove_front();
	}

	DLNode* ptr = other.front;
	while (ptr) {
		insert_rear(ptr->value());
		ptr = ptr->next();
	}

	return *this;
}


/* DLList destructor
*/
template<typename Type>
DLList<Type>::~DLList() {
	
	if (size == 0) {

	}

	if (size == 1) {
		delete front;
	}

	if (size > 1) {
		DLNode* ptr1 = front;
		DLNode* ptr2 = front->next();
		for (int i = 0; i < size - 1; i++) {
			delete ptr1;
			ptr1 = ptr2;
			ptr2 = ptr2->next();

		}
		delete ptr1;
		delete ptr2;
	}
}

/* DLList print
*/
template<typename Type>
void DLList<Type>::print() const {

	if (!empty()) {
		DLNode* ptr = front;

		while (ptr != nullptr) {
			std::cout << ptr->value();
			if (ptr->next() != nullptr) {
				std::cout << " ";
			}
			ptr = ptr->next();
		}
	}
}

/* DLList empty
*/
template<typename Type>
bool DLList<Type>::empty() const {


	return (front == nullptr);
	
}


/* DLList insert_front
*/
template<typename Type>
void DLList<Type>::insert_front(const Type &value) {

	DLNode* p = new DLNode(value, nullptr, front);

	if (empty()) {
		rear = p;
	}

	front = p;
	size++;

	if (size > 1) {
		p->next()->prev(front);
	}
}

/* DLList insert_rear
*/
template<typename Type>
void DLList<Type>::insert_rear(const Type &value) {


	DLNode* p = new DLNode(value, rear, nullptr);

	if (empty()) {
		front = p;
	}

	rear = p;
	size++;

	if (size > 1) {
		p->prev()->next(rear);
	}
}

/* DLList insert
*/
template<typename Type>
void DLList<Type>::insert(int idx, const Type &value) {

	if (idx <= 0) {
		//add node to front
		DLNode* p = new DLNode(value, nullptr, front);

		if (empty()) {
			rear = p;
		}

		front = p;
		size++;

		if (size > 1) {
			p->next()->prev(front);
		}
	}

	else if ((idx > 0) && (idx < size)) {
		//use indexing


		DLNode* ptr = front;

		for (int i = 1; i < idx; i++) {
			ptr = ptr->next();
		}

		DLNode* p = new DLNode(value, ptr, ptr->next());
		
		ptr->next(p);
		p->next()->prev(p);


		size++;
	}

	else if (idx >= size) {
		//add to rear
		DLNode* p = new DLNode(value, rear, nullptr);

		if (empty()) {
			front = p;
		}

		rear = p;
		size++;

		if (size > 1) {
			p->prev()->next(rear);
		}
	}
}

/*  DLList peek_front
*/
template<typename Type>
Type DLList<Type>::peek_front() const {
	// Force check that the front is pointing to a DLNode
	// Typical solution for deployement code is to throw
	// exceptions, but since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(front != nullptr);

	return front->value();

}

/* DLList peek_rear
*/
template<typename Type>
Type DLList<Type>::peek_rear() const {

	// Force check that the rear is pointing to a DLNode
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(rear != nullptr);

	return rear->value();
}

/* DLList peek
*/
template<typename Type>
Type DLList<Type>::peek(int idx) const {

	// Force index to be correct before getting the Type
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(idx >= 0 && idx < size);


	DLNode* ptr = front;

	for (int i = 0; i < idx; i++) {
		ptr = ptr->next();
	}

	return ptr->value();
}

/* DLList count
*/
template<typename Type>
int DLList<Type>::count() const {
	return size;

}

/* DLList count_value
*/
template<typename Type>
int DLList<Type>::count_value(const Type &value) const {

	DLNode* ptr = front;
	int count = 0;

	for (int i = 0; i < size; i++) {
		if (ptr->value() == value) {
			count++;
		}
		ptr = ptr->next();
	}

	return count;

}
/* DLList find
*/
template<typename Type>
int DLList<Type>::find(const Type &value) const {

	DLNode* ptr = front;
	int index = -1;
	int count = 0;

	for (int i = 0; i < size; i++) {
		if (ptr->value() == value) {
			index = count;
		}
		count++;
		ptr = ptr->next();
	}

	return index;

}

/* DLList remove_front
*/
template<typename Type>
bool DLList<Type>::remove_front() {

	if (empty()) {
		return false;
	}

	if (size == 1) {
		delete front;
		rear = nullptr;
		front = nullptr;
		size--;
		return true;
	}

	if (size > 1) {
		DLNode* ptr = front->next();
		delete front;
		front = ptr;
		size--;
		front->prev(nullptr);
		return true;

	}

}
/* DLList remove_rear
*/
template<typename Type>
bool DLList<Type>::remove_rear() {

	if (empty()) {
		return false;
	}

	if (size == 1) {
		delete front;
		rear = nullptr;
		front = nullptr;
		size--;
		return true;
	}

	if (size > 1) {
		DLNode* ptr = rear->prev();
		delete rear;
		rear = ptr;
		size--;
		rear->next(nullptr);
		return true;

	}
}

/* DLList remove_index
*/
template<typename Type>
bool DLList<Type>::remove_index(int idx) {

	if (idx < 0) {
		return false;
	}

	else  if (idx == 0) {
		//remove from front
		if (empty()) {
			return false;
		}

		if (size == 1) {
			delete front;
			rear = nullptr;
			front = nullptr;
			size--;
			return true;
		}

		if (size > 1) {
			DLNode* ptr = front->next();
			delete front;
			front = ptr;
			size--;
			front->prev(nullptr);
			return true;

		}
		
	}

	else if ((idx > 0) && (idx < size)) {
		//use indexing
		if (empty()) {
			return false;
		}

		if (size == 1) {
			delete front;
			rear = nullptr;
			front = nullptr;
			size--;
			return true;
		}

		if ((size-1) == (idx)) {
			DLNode* ptr = rear->prev();
			delete rear;
			rear = ptr;
			size--;
			rear->next(nullptr);
			return true;
		}

		if ((size > 1)) {
			DLNode* ptr = front;

			for (int i = 0; i < idx; i++) {
				ptr = ptr->next();
			}

			DLNode* ptrprev = ptr->prev();
			DLNode* ptrnext = ptr->next();
			delete ptr;
			ptrprev->next(ptrnext);
			ptrnext->prev(ptrprev);

			size--;
			return true;

		}
	}

	else if (idx >= size) {
		//remove from rear
		
		return false;
		
	}
}

/* DLList remove_index on value
*/
template<typename Type>
int DLList<Type>::remove_value(const Type &value) {

	DLNode* ptr = front;
	int index = -1;
	int count = 0;

	for (int i = 0; i < size; i++) {
		if (ptr->value() == value) {
			index = count;
		}
		count++;
		ptr = ptr->next();
	}

	remove_index(index);
	
	
	return index;
}

#endif







































































































// _X_XMXMXI