#pragma once
#define TREE_H
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;
template <class T>
struct Node {
	Node(T d, Node<T>* left = 0, Node<T>* right = 0)
		: data(d), leftChild(left), rightChild(right) { }
	Node<T>* leftChild;
	T data;
	Node<T>* rightChild;
};
template <class T>
class Tree {
public:
	Tree() { root = 0; } // empty tree
	void Insert(T& value) { Insert(root, value); }
	void Preorder() { Preorder(root); }
	void Inorder() { Inorder(root); }
	void Postorder() { Postorder(root); }
	void StackPreorder();
	void StackPostorder();
	void Levelorder();
private: // helper 함수들
	void Visit(Node<T>*);
	void Insert(Node<T>*&, T&);
	void Preorder(Node<T>*);
	void Inorder(Node<T>*);
	void Postorder(Node<T>*);
	Node<T>* root;
};
// stand-alone function who do not use helper function, but also recursive way
template<class T>
void Tree<T>::StackPreorder()
{
	stack<Node<T>*> stk;
	Node<T>* current = this->root;
	if (current == NULL) return;
	stk.push(current);
	while (!stk.empty()) {
		current = stk.top(); stk.pop();
		Visit(current);
		if (current->rightChild) {
			stk.push(current->rightChild);
		}
		if (current->leftChild) {
			stk.push(current->leftChild);
		}
	}
}

template<class T>
void Tree<T>::StackPostorder()
{
	Node<T>* current = root;
	if (current == NULL) { return; }
	stack<Node<T>*> stk;
	stk.push(current);
	Node<T>* prev = NULL;
	while (!stk.empty()) {
		current = stk.top();
		if (prev == NULL || prev->leftChild == current || prev->rightChild == current) {
			if (current->leftChild) {
				stk.push(current->leftChild);
			}
			else if (current->rightChild) {
				stk.push(current->rightChild);
			}
			else {
				stk.pop();
				Visit(current);
			}
		}
		else if (current->leftChild == prev) {
			if (current->rightChild) {
				stk.push(current->rightChild);
			}
			else {
				stk.pop();
				Visit(current);
			}
		}
		else if (current->rightChild == prev) {
			stk.pop();
			Visit(current);
		}
		prev = current;
	}
}


template<class T>
void Tree<T>::Levelorder()
{
	queue<Node<T>*> que;
	Node<T>* current = this->root;
	while (current) {
		Visit(current);
		if (current->leftChild) {
			que.push(current->leftChild);
		}
		if (current->rightChild) {
			que.push(current->rightChild);
		}
		if (que.empty()) { return; }
		current = que.front();
		que.pop();
	}
}

template <class T>
void Tree<T>::Visit(Node<T>* ptr) { cout << ptr->data << " "; }
template <class T>
void Tree<T>::Insert(Node<T>*& ptr, T& value) { //Insert 의 helper 함수
	if (ptr == 0) ptr = new Node<T>(value);
	else if (value < ptr->data) Insert(ptr->leftChild, value);
	else if (value > ptr->data) Insert(ptr->rightChild, value);
	else cout << endl << "Duplicate value " << value << " ignored\n";
}

template<class T>
void Tree<T>::Preorder(Node<T>* currentNode)
{// Preorder Traversal of Binary Tree
	if (currentNode) {
		Visit(currentNode);
		Preorder(currentNode->leftChild);
		Preorder(currentNode->rightChild);
	}
}

template <class T>
void Tree<T>::Inorder(Node<T>* currentNode)
{// Workhorse traverses the subtree rooted at currentNode.
// The workhorse is declared
// as a private member function of Tree.
	if (currentNode) {
		Inorder(currentNode->leftChild);
		Visit(currentNode);
		Inorder(currentNode->rightChild);
	}
}

template<class T>
void Tree<T>::Postorder(Node<T>* currentNode)
{
	if (currentNode) {
		Postorder(currentNode->leftChild);
		Postorder(currentNode->rightChild);
		Visit(currentNode);
	}
}