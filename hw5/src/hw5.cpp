#include <iostream>
#include <string>
#include "post.h"

using namespace std;
void Postfix(Expression);
int main() {
	char line[MAXLEN];
	cout << "C211123 LEE JUNSEON, 이준선" << endl;
	while (cin.getline(line, MAXLEN)) {
		Expression e(line); // line 버퍼를 이용하여 Expression을 읽음
		try {
			Postfix(e);
		}
		catch (char const* msg) {
			cerr << "Exception: " << msg << endl;
		}
	}
	return 0;
}