#include <iostream>
using namespace std;

template <typename T>
T add(T x, T y) {
	return x + y;
}

int main() {
	int intX = 1, intY = 2;
	double dbX = 3.3, dbY = 4.4;
	float fX = 09.24, fY = 10.07;
	cout << add(intX, intY) << endl;
	cout << add(dbX, dbY) << endl;
	cout << add(fX, fY) << endl;
	return 0;
}