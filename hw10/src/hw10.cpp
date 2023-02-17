#include "subway.h"
#include <iostream>
#include<fstream>
int main(int argc, char* argv[])
{
	int numLine;
	int line1, line2;
	string src, dst;
	if (argc != 3) {
		cerr << "Argument Count is" << argc << endl << "Argument must be " <<
			argc << endl;
		return 1;
	}
	fstream fin(argv[1]);
	if (!fin) {
		cerr << argv[1] << " open failed" << endl;
		return 1;
	}
	fin >> numLine;
	
	Subway subway;
	// read file and insert station
	for (int i = 0; i < numLine; i++) {
		int line1, line2; // line1 = line of src, line2 =line of dst
		string src, dst;
		fin >> line1 >> src >> line2 >> dst;

		subway.make_station(src, line1, dst, line2);
	}
	fin.close();

	// input1.txt 시작역과 도착역 정보 읽기
	fstream fin2(argv[2]);
	if (!fin2) {
		cerr << argv[2] << " open failed" << endl;
		return 1;
	}
	fin2 >> line1 >> src; // start(source) station
	fin2 >> line2 >> dst; // destination station
	fin2.close();
	
	subway.find_shortest_path(src, dst, line1);
	cout << endl;
	
	subway.find_middle_station(src, dst);
	
	return 0;
}