#ifndef AVL_H
#define AVL_H
#include <queue>
#include <iostream>
using namespace std;

class Node {
public:
	int GetBF();
	int GetData();
	void SetData(const int data);
	void SetBF(const int bf);
	Node* GetLeftChild();
	Node* GetRightChild();
	Node* GetParent();
	void SetLeftChild(Node* leftChild);
	void SetRightChild(Node* rightChild);
	void SetParent(Node* parent);
	Node(int data, int bf, Node* leftChild, Node* rightChild, Node* parent);
	Node(int data);
	Node();

	Node*& GetLeftChildRef();
	Node*& GetRightChildRef();
	Node*& GetParentRef();
private:
	int data;
	int bf;
	Node* leftChild;
	Node* rightChild;
	Node* parent;
};

class bstree {
private:
	bool rotation(Node* start, Node* end);
	inline void changeLeftSubtree(Node* main, Node* sub);
	inline void changeRightSubtree(Node* main, Node* sub);
	int GetHeight(Node* root);
	int GetBalanceFactor(Node* root);
	Node* rotateLL(Node* root);
	Node* rotateRR(Node* root);
	Node* rotateLR(Node* root);
	Node* rotateRL(Node* root);
	void Rebalance(Node*& root);
	Node* findMin(Node* root);
	Node* findNode(const int key, Node* root);
	Node* InorderSucc(Node* current);
	void ResetParent(Node*& root);
	void ResetBF(Node*& root);
	void remove(const int key, Node*& root);
	void recursive_balance_checker(Node*& root);
public:
	void visit(Node* ptr);
	void insert(const int value, Node*& root);
	void Showresult(Node* root);
	bool Search(const int key, Node* root);
	void del(const int key, Node*& root);
	void clear(Node*& root);
};
#endif
