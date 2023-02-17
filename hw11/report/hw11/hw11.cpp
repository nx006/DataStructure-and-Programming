#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <sys/time.h>
#include "sort.h"
using namespace std;
//argc는 인수의 개수(argument count), argv는 argument vector로 '문자열'의 리스트라고 생각하시면 됩니다.
//예) argv[0]: ./hw12 , argv[1]: 10 , argv[2]: t50.txt ..

// 0:insertion, 1:quick, 2:natural merge, 3:heap
enum { INSERTION, QUICK, NATURAL_MERGE, HEAP };

int main(int argc, char* argv[]) {

	if (argc < 3) { //명령 인수를 잘못 넣은 경우 (예외 처리)
		cerr << "wrong argument count" << endl;
		return 1;
	}

	int T = atoi(argv[1]); // 테스트 케이스의 수. atoi는 문자열을 integer로 변환하는 함수입니다. 
	
	
	cout << "T=" << T << endl;
	int N; // 각 테스트 케이스 별 레코드의 길이. (ex. t50.txt => N=50)
	int i; // iterator
	double sum_result[4]; // 각 알고리즘 별 총 실행시간(결과값) 저장.
	double result[4]; //각 알고리즘 별 실행시간(결과값) 저장(평균값) 저장
	// initialize sum_result and result 
	// 리눅스 C++ 업데이트좀 해주세요.. 이니셜라이징 문법이 안 먹혀서 하나하나 초기화해야 합니다...
	for (i = 0; i < 4; i++) {
		sum_result[i] = 0;
		result[i] = 0;
	}
	
	/*
	result[0]: insertion sort의 실행 시간
	result[1]: quick sort의 실행 시간
	result[2]: natural merge sort의 실행 시간
	result[3]: heap sort의 실행 시간
	*/
	struct timeval start_t , end_t; // start_t: 시작 시각, end_t: 종료 시각
	double diff_t; // end_t - start_t (종료 시각 - 시작 시각)
	int num; // 리스트에 값을 입력하기 위한 변수.

	cout << "--INS--|--QUICK--|--NATMG--|--HEAP--|" << endl;
	for (i = 2; i < T + 2; i++) {
		// i번째 인자의 파일을 읽습니다. 
		// 각 정렬 알고리즘에 맞는 자료구조 생성 및 데이터 입력.

		// 각 정렬 알고리즘의 리스트를 정의하였습니다.
		int* i_list; //insertion sort의 리스트
		int* q_list; //quick sort의 리스트
		int* nm_list; //natural merge sort의 리스트
		int* h_list; //heap sort의 리스트

		//파일의 이름을 저장한 후, ifstream 변수 선언을 통해 해당 파일을 read합니다.
		char* file_name = argv[i]; //argv[2]: t50.txt, argv[3]: t100.txt ..
		ifstream file(file_name); //파일명으로 file_name을 갖는 ifstream 변수 file을 선언합니다.

		//file을 열어 줍니다.
		if (file.is_open()) {
			file >> N; // 파일의 첫 번째 문자(열)을 읽어서 N에 저장을 해 줍니다. 
			// 이때 N은 int로 정의되어 있기에 int값으로 저장이 됩니다.
			// N 값은 레코드의 길이(숫자의 개수)를 의미합니다.

 // 파일에 있는 숫자의 개수(N)를 읽어서 배열 크기를 정합니다.
 // 교재를 참고해서 배열 크기를 할당하였으며, 편하신 대로 수정하셔도 괜찮습니다.
			i_list = new int[N + 2]; //교재의 삽입정렬은 최소 N+2개의 크기가 필요.
			q_list = new int[N + 1];
			nm_list = new int[N];
			h_list = new int[N + 1];

			//리스트에 값을 입력합니다.
			for (int k = 0; k < N; k++) { //N개의 숫자 만큼 반복
				file >> num; //file로부터 다음 문자(열)을 읽어서 num에 저장을 합니다.

				// 교재의 정렬은 [1:N]의 인덱스를 갖는 리스트를 정렬합니다.
				// 따라서 [0]은 스킵하고 [1]부터 저장을 했습니다.
				// 단 natural_merge_sort는 교재에 없는 함수를 구현하였기에,
				// [0:N-1]의 인덱스를 갖는 리스트를 정렬합니다. 때문에 0번 째 요소부터 채워넣었습니다.
				i_list[k + 1] = num;
				q_list[k + 1] = num;
				nm_list[k] = num;
				h_list[k + 1] = num;
			}
			// [0]에는 null값이 들어 있으니 이 부분은 유의하시기 바랍니다.
		}
		//file이 열리지 않을 경우의 예외 처리를 해 줍니다.
		else {
			cerr << file_name << " is not opend" << endl;
			exit(1);
		}

		//file을 닫아 줍니다.
		file.close();

		int rerunNumber = 5;

		
		// 각 4개의 리스트에 대해 동일한 5개 리스트를 만듭니다.
		int** i_list_array = new int* [rerunNumber];
		int** q_list_array = new int* [rerunNumber];
		int** nm_list_array = new int* [rerunNumber];
		int** h_list_array = new int* [rerunNumber];
		
		//각 리스트에 대해 5개의 리스트를 만들어 줍니다.
		for (int i = 0; i < rerunNumber; i++) {
			i_list_array[i] = new int[N + 2];
			q_list_array[i] = new int[N + 1];
			nm_list_array[i] = new int[N];
			h_list_array[i] = new int[N + 1];
		}
		
		// 복사합니다.
		for (int k = 0; k < rerunNumber; k++) {
			for (int i = 0; i < N + 2; i++) {
				i_list_array[k][i] = i_list[i];
			}
			for (int i = 0; i < N + 1; i++) {
				q_list_array[k][i] = q_list[i];
			}
			for (int i = 0; i < N; i++) {
				nm_list_array[k][i] = nm_list[i];
			}
			for (int i = 0; i < N + 1; i++) {
				h_list_array[k][i] = h_list[i];
			}
		}

		// 알고리즘 별 정렬 수행 및 시간 측정.
		// 정렬은 같은 파일에 대해 5번 반복하여, 그 5번의 평균 실행 시간을 저장한다
		for (int k = 0; k < rerunNumber; k++) {
			for (int x = 0; x < 4; x++) { // 0:insertion, 1:quick, 2:natural merge, 3:heap

				// 정렬 시작
				gettimeofday(&start_t, NULL); // 정렬을 수행하기 전에 시작 시각을 저장합니다.

				//정렬 수행
				switch (x) { //x의 값에 맞춰서 정렬의 종류를 선택합니다.
				case INSERTION:
					insertion_sort(i_list_array[k], N);
					break;
				case QUICK:
					quick_sort(q_list_array[k], 1, N);
					break;
				case NATURAL_MERGE:
					natural_merge_sort(nm_list_array[k], N - 1);
					break;
				case HEAP:
					heap_sort(h_list_array[k], N);
					break;
				default:
					cout << "? ERROR ?" << endl;
					exit(1);
					break;
				}

				// 정렬 끝
				gettimeofday(&end_t, NULL); // 정렬이 끝나면 종료 시각을 저장합니다.

				// 마이크로 단위로 시간을 저장해 줍니다.
				// diff_t = 정수 단위 + 소숫점 단위 (ex. 1.123456초 = 1.0 + 0.123456)
				diff_t = (double)(end_t.tv_sec - start_t.tv_sec) + ((double)(end_t.tv_usec - start_t.tv_usec) / 1000000);

				// 0:insertion, 1:quick, 2:natural merge, 3:heap
				// 각 알고리즘 별 실행 시간 축적
				sum_result[x] += diff_t;
			}
		}

		// 평균 실행 시간을 저장
		for (int x = 0; x < 4; x++) {
			result[x] = sum_result[x] / rerunNumber;
		}

		// 0:insertion, 1:quick, 2:natural merge, 3:heap
		ofstream result_file;
		result_file.open("result.txt", ios::app);
		result_file << result[0] << "," << result[1] << "," << result[2] << "," << result[3] << endl;
		result_file.close();

		//결과 출력, 이 부분은 수정하시면 안됩니다.
		cout.precision(5);
		cout << fixed;
		for (int j = 0; j < 4; j++) {
			cout << result[j] << '|';
		}
		cout << "N=" << N << endl;

		//할당되었던 해당 테스트 케이스의 리스트 삭제.
		delete[] i_list;
		delete[] q_list;
		delete[] nm_list;
		delete[] h_list;

		// 할당되었던 테스트 케이스의 리스트 배열 삭제.
		for (int k = 0; k < rerunNumber; k++) {
			delete[] i_list_array[k];
			delete[] q_list_array[k];
			delete[] nm_list_array[k];
			delete[] h_list_array[k];
		}

		delete[] i_list_array;
		delete[] q_list_array;
		delete[] nm_list_array;
		delete[] h_list_array;
	}

}
