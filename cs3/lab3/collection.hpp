
#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include <iostream>
#include "list.hpp"

//template made so it can add any type variable 
template <typename T> 
class Collection {
public:
	//function prototypes
	Collection() : nodeH (nullptr) {};
	void add(const T& x);
	void remove(const T& x);
	T last();
	void print();
	template <typename U>
	friend bool equal(const Collection<U>& x, const Collection<U>& y);
private:
	node <T>* nodeH;
};

//Will add a node into the list, generic so it can add anything
template<typename T>
inline void Collection<T>::add(const T& x)
{
	node<T>* addNode = new node<T>;
	addNode->setData(x);
	addNode->setNext(NULL);
	if (nodeH == NULL) {
		nodeH = addNode;
	}
	else {
		node<T>* traverseNode = nodeH;
		while (traverseNode->getNext() != NULL) {
			traverseNode = traverseNode->getNext();
		}
		traverseNode->setNext(addNode);
	}

}

//Removes all instances of any element
template<typename T>
inline void Collection<T>::remove(const T& x)
{
	//two node objects with one made to hold and the other to move
	node<T>* nodePointOne = nullptr;
	node<T>* nodePointTwo = nodeH;

	//mover used for second node to move onto
	node<T>* mover = nodePointTwo;
	//goes through the entire list
	while (mover != nullptr) {
		//moves to next item in list
		mover = nodePointTwo->getNext();
		if (nodePointTwo->getData() == x) {
			//deletes the instance of the item within the list
			nodePointOne->setNext(nodePointTwo->getNext());
			delete nodePointTwo;
		}
		//sets new values for the nodes
		nodePointOne = nodePointTwo;
		nodePointTwo = mover;
	}
}

//Will give the last item that was added within the list
template<typename T>
inline T Collection<T>::last()
{
	node<T>* getter = nodeH;
	//goes through it until it reaches nullptr
	while (getter != nullptr) {
		getter = getter->getNext();
		if (getter->getNext() == nullptr) {
			return getter->getData();
		}
	}
	return getter->getData();
}

//Prints out the entire list
template<typename T>
inline void Collection<T>::print()
{
	//Starts printer at the head
	node<T>* printer = nodeH;
	//Will repeat printing until the end
	while (printer->getNext() != NULL) {
		std::cout << printer->getData() << "\n";
		printer = printer->getNext();
	}
	//Prints out once more to get the last item in list
	std::cout << printer->getData() << "\n";
}

//Checks if two lists are equal
template<typename U>
inline bool equal(const Collection<U>& x, const Collection<U>& y)
{
	//Sets two variables equal to the head of two lists
	auto nodePointOne = x.nodeH;
	auto nodePointTwo = y.nodeH;
	//goes thru the entirety of each list 
	while (nodePointOne->getNext() != NULL && nodePointTwo->getNext() != NULL) {
		//returns false whenever it spots a difference
		if (nodePointOne->getData() != nodePointTwo->getData()) {
			return false;
		}
		//Will move to the next item within the list if they are equal
		if (nodePointOne->getData() == nodePointTwo->getData()) {
			nodePointOne = nodePointOne->getNext();
			nodePointTwo = nodePointTwo->getNext();
		}
	}
	//will return true if it got through everything
	return true;
}

#endif //COLLECTION_HPP_
