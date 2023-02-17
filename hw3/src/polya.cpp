#include <iostream>
#include "polya.h"
using namespace std;
istream& operator>> (istream& is, Polynomial& p) {
	// #terms and (coefficoent, exponent)의 pair들을 읽어들인다.
	// 높은차수의 항부터 저장되었다고 가정한다.
	int noofterms; float coef; int exp;
	is >> noofterms;
	for (int i = 0; i < noofterms; i++) {
		is >> coef >> exp; // 계수와 지수 pair를 읽어들인다.
		p.NewTerm(coef, exp);
	}
	return is;
}
ostream& operator<< (ostream& os, Polynomial& p)
{
	// 출력부분
	// 고려해야 할 부분 -> 부호 출력(단, 첫 번째 항이 +이면 아무것도 출력 안 함)
	// 계수(coef)가 1, -1 -> 계수 출력 안 한다. 단, -1이면 - 부호만 출력
	// 그게 아니라면 계수의 절댓값를 출력하자.
	// 지수(exp)가 1, -1 -> 지수에 아무것도 안 붙인다.
	// 지수(exp)가 0이면 상수, 그러면 계수만 출력한다.
	const double ERROR = 0.000001;
	
	// 항이 없다면? -> os에는 아무것도 출력하지 않는다.
	if (p.terms <= 0) {
		os << endl;
		return os; 
	}



	
	for (int aPos = 0; aPos < p.terms; aPos++) {
		//부호 출력
		if (p.termArray[aPos].coef < 0) { // 부호 음수 시 - 출력
			os << " -";
		}
		else if (aPos != 0) { // 부호 양수 시, 첫 번째 항이 아닐 때만 + 출력
			os << " +";
		}

		//계수의 절댓값 출력
		if (p.termArray[aPos].coef != -1 && p.termArray[aPos].coef != 1) {
			os << fabs(p.termArray[aPos].coef);
		}

		//x와 지수 출력
		if (p.termArray[aPos].exp != 0) { // 지수가 0이 아닐 때만 x 부분 출력
			if (p.termArray[aPos].exp != -1 && p.termArray[aPos].exp != 1) { // 지수가 1과 -1이 아닐 때만 
				os << "x^" << p.termArray[aPos].exp;
			}
			else {
				os << "x";
			}
		}
		else if (p.termArray[aPos].coef == 1 || p.termArray[aPos].coef == -1) {
			os << fabs(p.termArray[aPos].coef);
		}
	}
	os << endl;
	return os;
}
Polynomial::Polynomial() :capacity(1), terms(0) {
	termArray = new Term[capacity];
}
void Polynomial::NewTerm(const float theCoeff, const int theExp)
{
	if (terms == capacity) {
		capacity *= 2; // 현재 차있는 data 수(terms)와 capacity가 같으면 capacity 두 배로 해서 배열 생성
		Term* temp = new Term[capacity];
		copy(termArray, termArray + terms, temp);
		delete[] termArray;
		termArray = temp;
	}
	termArray[terms].coef = theCoeff;
	termArray[terms++].exp = theExp;
}
Polynomial Polynomial::operator+(Polynomial& b)
{
	//다항식의 덧셈을 해주는 함수
	Polynomial c;
	int aPos = 0; int bPos = 0;
	while ((aPos < terms) && (bPos < b.terms)) {
		if (termArray[aPos].exp == b.termArray[bPos].exp) {
			float t = termArray[aPos].coef + b.termArray[bPos].coef;
			if (t) {
				c.NewTerm(t, termArray[aPos].exp);
				aPos++; bPos++;
			}
		}
		else if (termArray[aPos].exp < b.termArray[bPos].exp) {
			c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
			bPos++;
		}
		else {
			c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
			aPos++;
		}
	}
	for (; aPos < terms; aPos++) {
		c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
	}
	for (; bPos < b.terms; bPos++) {
		c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
	}

	return c;
}

// 다항식 곱셈 함수
Polynomial Polynomial::operator*(Polynomial& b) {

	Polynomial c;
	int at = terms, bt = b.terms;
	for (int aPos = 0; aPos < at; aPos++) {
		for (int j = 0; j < bt; j++) {
			float t = termArray[aPos].coef * b.termArray[j].coef;
			int k = termArray[aPos].exp + b.termArray[j].exp;
			Polynomial temp;
			temp.NewTerm(t, k);
			c = c + temp;
		}
	}
	return c;
}