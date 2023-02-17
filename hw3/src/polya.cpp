#include <iostream>
#include "polya.h"
using namespace std;
istream& operator>> (istream& is, Polynomial& p) {
	// #terms and (coefficoent, exponent)�� pair���� �о���δ�.
	// ���������� �׺��� ����Ǿ��ٰ� �����Ѵ�.
	int noofterms; float coef; int exp;
	is >> noofterms;
	for (int i = 0; i < noofterms; i++) {
		is >> coef >> exp; // ����� ���� pair�� �о���δ�.
		p.NewTerm(coef, exp);
	}
	return is;
}
ostream& operator<< (ostream& os, Polynomial& p)
{
	// ��ºκ�
	// ����ؾ� �� �κ� -> ��ȣ ���(��, ù ��° ���� +�̸� �ƹ��͵� ��� �� ��)
	// ���(coef)�� 1, -1 -> ��� ��� �� �Ѵ�. ��, -1�̸� - ��ȣ�� ���
	// �װ� �ƴ϶�� ����� ���񰪸� �������.
	// ����(exp)�� 1, -1 -> ������ �ƹ��͵� �� ���δ�.
	// ����(exp)�� 0�̸� ���, �׷��� ����� ����Ѵ�.
	const double ERROR = 0.000001;
	
	// ���� ���ٸ�? -> os���� �ƹ��͵� ������� �ʴ´�.
	if (p.terms <= 0) {
		os << endl;
		return os; 
	}



	
	for (int aPos = 0; aPos < p.terms; aPos++) {
		//��ȣ ���
		if (p.termArray[aPos].coef < 0) { // ��ȣ ���� �� - ���
			os << " -";
		}
		else if (aPos != 0) { // ��ȣ ��� ��, ù ��° ���� �ƴ� ���� + ���
			os << " +";
		}

		//����� ���� ���
		if (p.termArray[aPos].coef != -1 && p.termArray[aPos].coef != 1) {
			os << fabs(p.termArray[aPos].coef);
		}

		//x�� ���� ���
		if (p.termArray[aPos].exp != 0) { // ������ 0�� �ƴ� ���� x �κ� ���
			if (p.termArray[aPos].exp != -1 && p.termArray[aPos].exp != 1) { // ������ 1�� -1�� �ƴ� ���� 
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
		capacity *= 2; // ���� ���ִ� data ��(terms)�� capacity�� ������ capacity �� ��� �ؼ� �迭 ����
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
	//���׽��� ������ ���ִ� �Լ�
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

// ���׽� ���� �Լ�
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