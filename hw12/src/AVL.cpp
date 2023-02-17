#include "AVL.h"
void bstree::visit(Node* ptr) {
	cout << ptr->GetData() << "\t";
	if (ptr->GetLeftChild()) { cout << "left : " << ptr->GetLeftChild()->GetData() << '\t'; }
	else { cout << "left : empty\t"; }
	if (ptr->GetRightChild()) { cout << "right : " << ptr->GetRightChild()->GetData() << '\t'; }
	else { cout << "right : empty\t"; }
	cout << endl;
}

inline void bstree::changeLeftSubtree(Node* main, Node* sub) {
	main->SetLeftChild(sub);
	if (main->GetLeftChild()) {
		main->GetLeftChild()->SetParent(main);
	}
}

inline void bstree::changeRightSubtree(Node* main, Node* sub) {
	main->SetRightChild(sub);
	if (main->GetRightChild()) {
		main->GetRightChild()->SetParent(main);
	}
}

void bstree::ResetBF(Node*& root) {
	if (root) {
		root->SetBF(GetBalanceFactor(root));
		ResetBF(root->GetLeftChildRef());
		ResetBF(root->GetRightChildRef());
	}
}

int bstree::GetHeight(Node* root) {
	if (root == NULL) { return 0; }
	else {
		int leftHeight = GetHeight(root->GetLeftChild());
		int rightHeight = GetHeight(root->GetRightChild());
		return max(leftHeight, rightHeight) + 1;
	}
}

int bstree::GetBalanceFactor(Node* root) {
	if (root == NULL) { return 0; }
	else {
		int leftHeight = GetHeight(root->GetLeftChild());
		int rightHeight = GetHeight(root->GetRightChild());
		return leftHeight - rightHeight;
	}
}

Node* bstree::rotateLL(Node* root) {
	Node* parentNode = root;
	Node* childNode = root->GetLeftChild();

	changeLeftSubtree(parentNode, childNode->GetRightChild());
	changeRightSubtree(childNode, parentNode);

	return childNode;
}

Node* bstree::rotateRR(Node* root) {
	Node* parentNode = root;
	Node* childNode = root->GetRightChild();

	changeRightSubtree(parentNode, childNode->GetLeftChild());
	changeLeftSubtree(childNode, parentNode);

	return childNode;
}

Node* bstree::rotateLR(Node* root) {
	Node* parentNode = root;
	Node* childNode = root->GetLeftChild();

	changeLeftSubtree(parentNode, rotateRR(childNode)); // �κ��� RR ȸ��

	return rotateLL(parentNode); // LL ȸ��
}

Node* bstree::rotateRL(Node* root) {
	Node* parentNode = root;
	Node* childNode = root->GetRightChild();

	changeRightSubtree(parentNode, rotateLL(childNode)); // �κ��� LL ȸ��

	return rotateRR(parentNode); // RR ȸ��
}

void bstree::Rebalance(Node*& root) {
	if (root == NULL) return;
	ResetBF(root);
	int balanceFactor = root->GetBF();

	// BF�� 2 �̻��̸� LL, LR ȸ��
	if (balanceFactor > 1) {
		if (root->GetLeftChild()->GetBF() > 0) { // LL
			root = rotateLL(root);
		}
		else { // LR
			root = rotateLR(root);
		}
	}
	else if (balanceFactor < -1) { // ������ ����Ʈ���� �� ���� ��
		if (root->GetRightChild()->GetBF() < 0) { // RR
			root = rotateRR(root);
		}
		else { // RL
			root = rotateRL(root);
		}
	}
	ResetParent(root);
	ResetBF(root);
}

Node* bstree::findMin(Node* root)
{
	if (root == NULL) return NULL;
	while (root->GetLeftChild() != NULL) {
		root = root->GetLeftChild();
	}

	return root;
}

Node* bstree::InorderSucc(Node* current)
{
	if (current == NULL) return NULL;

	// case 1: has right subtree -> then find the min value from right subtree
	if (current->GetRightChild() != NULL) {
		return findMin(current->GetRightChild());
	}
	 
	// case 2: no right subtree -> then find the first ancestor that is greater than current node
	else {
		// current has no right subtree, and no parent -> then current is the root node
		if (current->GetParent() == NULL) return NULL;
		
		Node* ancestor = current->GetParent();
		Node* successor = current;
		while (ancestor != NULL && successor == ancestor->GetRightChild()) {
			successor = ancestor;
			ancestor = ancestor->GetParent();
		}
		return ancestor;
	}
}

Node* bstree::findNode(const int key, Node* root)
{
	Node* currentNode = root; // current node

	while (currentNode != NULL) {
		int currentValue = currentNode->GetData();

		// key�� ã���� ��
		if (key == currentValue) {
			return currentNode;
		}

		// ã���� �ϴ� key�� ���� �˻� ���� value���� ū ���
		else if (key < currentValue) {
			currentNode = currentNode->GetLeftChild();
		}
		// ���� ���
		else {
			currentNode = currentNode->GetRightChild();
		}
	}
	return NULL;
}

// root ����� ��� parent�� �ùٸ��� �����Ѵ�.
void bstree::ResetParent(Node*& root)
{
	if (root == NULL) return;

	// root�� ���� ����Ʈ���� ������ ��
	if (root->GetLeftChild() != NULL) {
		root->GetLeftChild()->SetParent(root);
		ResetParent(root->GetLeftChildRef());
	}

	// root�� ������ ����Ʈ���� ������ ��
	if (root->GetRightChild() != NULL) {
		root->GetRightChild()->SetParent(root);
		ResetParent(root->GetRightChildRef());
	}
}

void bstree::recursive_balance_checker(Node*& root)
{
	if (root) {
		recursive_balance_checker(root->GetLeftChildRef());
		recursive_balance_checker(root->GetRightChildRef());
		if (root->GetBF() > 1 || root->GetBF() < -1) {
			Rebalance(root);
		}
	}
}

void bstree::remove(const int key, Node*& root)
{
	if (root == NULL) return;

	// ���� 1: ���� key�� �ش��ϴ� Node ã��
	Node* target = findNode(key, root);
	// if target is null
	if (target == NULL) {
		cout << "I cannot delete " << key << " as " << key << " is not exist in tree" << endl;
		return;
	}
	
	Node* parent = target->GetParent();

	// If target is root node, then make virtual_parent.
	Node* virtual_parent = NULL;
	if (parent == NULL) {
		virtual_parent = new Node(root->GetData() - 1, 0, NULL, root, NULL);
		root->SetParent(virtual_parent);
		parent = virtual_parent;
	}

	// ���� 2: �����
	// case 1 : target is leaf node
	if (target->GetRightChild() == NULL && target->GetLeftChild() == NULL) {
		bool isTargetEqualsRoot = (target == root);
		// if target is leftnode of parent node
		if (parent->GetLeftChild() == target) {
			parent->SetLeftChild(NULL);
			delete target;
		}
		else {
			parent->SetRightChild(NULL);
			delete target;
		}

		if (isTargetEqualsRoot) {
			root = NULL;
		}
		else {
			ResetBF(root);
			ResetParent(root);
			Node* cur = parent;
			while (cur != NULL && cur != root) {
				if (cur->GetBF() > 1 || cur->GetBF() < -1) {
					Rebalance(cur);
					ResetBF(root);
					ResetParent(root);
				}
				cur = cur->GetParent();
			}
		}
	}
	// case 2 : target has one child
	else if (target->GetLeftChild() == NULL || target->GetRightChild() == NULL) {
		Node* target_child = NULL; // child of target

		if (target->GetLeftChild() != NULL)
			target_child = target->GetLeftChild();
		else
			target_child = target->GetRightChild();

		// �� ��� target_child�� �ǻ� successor�� �ȴ�.
		// successor�� target->parent���� ��������ָ� �ȴ�.
		if (parent->GetLeftChild() == target) {
			parent->SetLeftChild(target_child);
			target_child->SetParent(parent);
			delete target;
		}
		else {
			parent->SetRightChild(target_child);
			target_child->SetParent(parent);
			delete target;
		}

		// �ǻ� successor�� target_child�κ��� root���� balance �˻�
		ResetBF(root);
		ResetParent(root);
		Node* cur = target_child;
		while (cur != NULL && cur != root) {
			if (cur->GetBF() > 1 || cur->GetBF() < -1) {
				Rebalance(cur);
				ResetBF(root);
				ResetParent(root);
			}
			cur = cur->GetParent();
		}
	}
	// case 3 : target has two child
	else {
		Node* successor = InorderSucc(target);
		target->SetData(successor->GetData());
		
		Node* cur_parent = successor->GetParent(); // parent of successor
		
		// ���� successor�� �����Ѵ�.
		remove(successor->GetData(), successor);

		// �����Ǿ��� successor���� root���� �ö󰡸鼭 �뷱���� �����.
		ResetBF(root);
		ResetParent(root);
		while (cur_parent != NULL && cur_parent != root) {
			if (cur_parent->GetBF() > 1 || cur_parent->GetBF() < -1) {
				Rebalance(cur_parent);
				ResetBF(root);
				ResetParent(root); 
			}
			cur_parent = cur_parent->GetParent();
		}
	}
	
	// ������ ��尡 ��Ʈ ����� ��쿡 ���� ó��
	if (virtual_parent != NULL) {
		root = virtual_parent->GetRightChild();
		if (root != NULL) root->SetParent(NULL);
		delete virtual_parent;
		return;
	}

	if (root) {
		Rebalance(root);
	}
}

void bstree::insert(const int value, Node*& root)
{
	if (root == NULL) {
		root = new Node(value);
		root->SetParent(root->GetParent());
		root->SetBF(GetBalanceFactor(root));
	}
	else if (value < root->GetData()) {
		insert(value, root->GetLeftChildRef());
		Rebalance(root);

	}
	else if (value > root->GetData()) {
		insert(value, root->GetRightChildRef());
		Rebalance(root);
	}
}

// bfs ������� ��ȸ
void bstree::Showresult(Node* root) {
	if (root == NULL) return;
	queue<Node*> q; ///> ��ȸ�ϰ��� �ϴ� Node���� ��� ť
	q.push(root);
	while (!q.empty()) {
		Node* current = q.front(); q.pop();
		visit(current);
		if (current->GetLeftChild() != NULL) {
			q.push(current->GetLeftChild());
		}
		if (current->GetRightChild() != NULL) {
			q.push(current->GetRightChild());
		}
	}
};
bool bstree::Search(const int key, Node* root) {
	Node* currentNode = root; // current node
	queue<Node*> path; // current node���� �絵�� �� ����
	path.push(currentNode);

	while (currentNode != NULL) {
		int currentValue = currentNode->GetData();

		// key�� ã���� ��
		if (key == currentValue) {
			while (!path.empty()) {
				Node* temp = path.front(); path.pop();
				cout << temp->GetData();
				if (!path.empty()) {
					cout << " -> ";
				}
			}
			cout << endl;
			return true;
		}
		
		// ã���� �ϴ� key�� ���� �˻� ���� value���� ū ���
		else if (key < currentValue) {
			currentNode = currentNode->GetLeftChild();
			path.push(currentNode);
		}
		else {
			currentNode = currentNode->GetRightChild();
			path.push(currentNode);
		}
	}
	cout << "I cannot find " << key << "in tree" << endl;
	return false;
}
void bstree::del(const int key, Node*& root) {
	remove(key, root);

	if (root == NULL) return;
	// recursive�ϰ� successor�� �����ϴ���, successor�� parent�� �ٽ� �������־�� �Ѵ�.
	root->SetParent(NULL);
	ResetParent(root);
	ResetBF(root);
	
	// ���������� ��� ��� ��ȸ�ϸ鼭 �뷱�� üũ
	recursive_balance_checker(root);
}

bool bstree::rotation(Node* start, Node* end) {
	return true;
}

// delete all subtree
void bstree::clear(Node*& root) {
	if (root == NULL) return;
	clear(root->GetLeftChildRef());
	clear(root->GetRightChildRef());
	delete root;
	root = NULL;
}

int Node::GetBF()
{
	return bf;
}

int Node::GetData()
{
	return data;
}

void Node::SetData(const int data)
{
	this->data = data;
}

void Node::SetBF(const int bf)
{
	this->bf = bf;
}

Node* Node::GetLeftChild()
{
	return leftChild;
}

Node* Node::GetRightChild()
{
	return rightChild;
}

Node* Node::GetParent()
{
	return parent;
}

void Node::SetLeftChild(Node* leftChild)
{
	this->leftChild = leftChild;
}

void Node::SetRightChild(Node* rightChild)
{
	this->rightChild = rightChild;
}

void Node::SetParent(Node* parent)
{
	this->parent = parent;
}

Node::Node(int _value, int _bf, Node* _leftChild, Node* _rightChild, Node* _parent)
	: data(_value), bf(_bf), leftChild(_leftChild), rightChild(_rightChild), parent(_parent) {}

Node::Node(int _value)
	: data(_value) 
{
	bf = 0;
	leftChild = NULL;
	rightChild = NULL;
	parent = NULL;
}

Node::Node() 
{
	data = 0;
	bf = 0;
	leftChild = NULL;
	rightChild = NULL;
	parent = NULL;
}

Node*& Node::GetLeftChildRef()
{
	return leftChild;
}

Node*& Node::GetRightChildRef()
{
	return rightChild;
}

Node*& Node::GetParentRef()
{
	return parent;
}
