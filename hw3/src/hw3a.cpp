#include <iostream>
using namespace std;
#include "polya.h"
int main() {
	Polynomial p1, p2;
	cin >> p1 >> p2; // 2개의 다항식을 읽어 들인다.
	Polynomial p3 = p1 + p2;
	Polynomial p4 = p1 * p2;
	cout << p1 << p2 << p3 << p4;

	return 0;
}