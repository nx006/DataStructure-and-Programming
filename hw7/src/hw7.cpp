#include <iostream>
#include "bt.h"
using namespace std;
int main()
{
	Tree<double> tree;
	double dval;
	cout << "Enter doubles:\n";
	// while (cin >> dval) tree.Insert(dval);
	double arr[] = { 35.3, 15.7, 81.5, 4.5, 66.7, 91.2, 2.3, 5.2, 88.2, 94.5 };
	for (auto n : arr) {
		tree.Insert(n);
	}
	cout << endl << "Preorder traversal: "; tree.Preorder();
	cout << endl << "Inorder traversal: "; tree.Inorder();
	cout << endl << "Postorder traversal: "; tree.Postorder();
	cout << endl << "Levelorder traversal: "; tree.Levelorder();
	cout << endl << "Stack Preorder traversal: "; tree.StackPreorder();
	cout << endl << "Stack Postorder traversal: "; tree.StackPostorder();
	cout << endl;

	return 0;
}