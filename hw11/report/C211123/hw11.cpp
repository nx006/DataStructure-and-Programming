#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <sys/time.h>
#include "sort.h"
using namespace std;
//argc�� �μ��� ����(argument count), argv�� argument vector�� '���ڿ�'�� ����Ʈ��� �����Ͻø� �˴ϴ�.
//��) argv[0]: ./hw12 , argv[1]: 10 , argv[2]: t50.txt ..

// 0:insertion, 1:quick, 2:natural merge, 3:heap
enum { INSERTION, QUICK, NATURAL_MERGE, HEAP };

int main(int argc, char* argv[]) {

	if (argc < 3) { //��� �μ��� �߸� ���� ��� (���� ó��)
		cerr << "wrong argument count" << endl;
		return 1;
	}

	int T = atoi(argv[1]); // �׽�Ʈ ���̽��� ��. atoi�� ���ڿ��� integer�� ��ȯ�ϴ� �Լ��Դϴ�. 
	
	
	cout << "T=" << T << endl;
	int N; // �� �׽�Ʈ ���̽� �� ���ڵ��� ����. (ex. t50.txt => N=50)
	int i; // iterator
	double sum_result[4]; // �� �˰��� �� �� ����ð�(�����) ����.
	double result[4]; //�� �˰��� �� ����ð�(�����) ����(��հ�) ����
	// initialize sum_result and result 
	// ������ C++ ������Ʈ�� ���ּ���.. �̴ϼȶ���¡ ������ �� ������ �ϳ��ϳ� �ʱ�ȭ�ؾ� �մϴ�...
	for (i = 0; i < 4; i++) {
		sum_result[i] = 0;
		result[i] = 0;
	}
	
	/*
	result[0]: insertion sort�� ���� �ð�
	result[1]: quick sort�� ���� �ð�
	result[2]: natural merge sort�� ���� �ð�
	result[3]: heap sort�� ���� �ð�
	*/
	struct timeval start_t , end_t; // start_t: ���� �ð�, end_t: ���� �ð�
	double diff_t; // end_t - start_t (���� �ð� - ���� �ð�)
	int num; // ����Ʈ�� ���� �Է��ϱ� ���� ����.

	cout << "--INS--|--QUICK--|--NATMG--|--HEAP--|" << endl;
	for (i = 2; i < T + 2; i++) {
		// i��° ������ ������ �н��ϴ�. 
		// �� ���� �˰��� �´� �ڷᱸ�� ���� �� ������ �Է�.

		// �� ���� �˰����� ����Ʈ�� �����Ͽ����ϴ�.
		int* i_list; //insertion sort�� ����Ʈ
		int* q_list; //quick sort�� ����Ʈ
		int* nm_list; //natural merge sort�� ����Ʈ
		int* h_list; //heap sort�� ����Ʈ

		//������ �̸��� ������ ��, ifstream ���� ������ ���� �ش� ������ read�մϴ�.
		char* file_name = argv[i]; //argv[2]: t50.txt, argv[3]: t100.txt ..
		ifstream file(file_name); //���ϸ����� file_name�� ���� ifstream ���� file�� �����մϴ�.

		//file�� ���� �ݴϴ�.
		if (file.is_open()) {
			file >> N; // ������ ù ��° ����(��)�� �о N�� ������ �� �ݴϴ�. 
			// �̶� N�� int�� ���ǵǾ� �ֱ⿡ int������ ������ �˴ϴ�.
			// N ���� ���ڵ��� ����(������ ����)�� �ǹ��մϴ�.

 // ���Ͽ� �ִ� ������ ����(N)�� �о �迭 ũ�⸦ ���մϴ�.
 // ���縦 �����ؼ� �迭 ũ�⸦ �Ҵ��Ͽ�����, ���Ͻ� ��� �����ϼŵ� �������ϴ�.
			i_list = new int[N + 2]; //������ ���������� �ּ� N+2���� ũ�Ⱑ �ʿ�.
			q_list = new int[N + 1];
			nm_list = new int[N];
			h_list = new int[N + 1];

			//����Ʈ�� ���� �Է��մϴ�.
			for (int k = 0; k < N; k++) { //N���� ���� ��ŭ �ݺ�
				file >> num; //file�κ��� ���� ����(��)�� �о num�� ������ �մϴ�.

				// ������ ������ [1:N]�� �ε����� ���� ����Ʈ�� �����մϴ�.
				// ���� [0]�� ��ŵ�ϰ� [1]���� ������ �߽��ϴ�.
				// �� natural_merge_sort�� ���翡 ���� �Լ��� �����Ͽ��⿡,
				// [0:N-1]�� �ε����� ���� ����Ʈ�� �����մϴ�. ������ 0�� ° ��Һ��� ä���־����ϴ�.
				i_list[k + 1] = num;
				q_list[k + 1] = num;
				nm_list[k] = num;
				h_list[k + 1] = num;
			}
			// [0]���� null���� ��� ������ �� �κ��� �����Ͻñ� �ٶ��ϴ�.
		}
		//file�� ������ ���� ����� ���� ó���� �� �ݴϴ�.
		else {
			cerr << file_name << " is not opend" << endl;
			exit(1);
		}

		//file�� �ݾ� �ݴϴ�.
		file.close();

		int rerunNumber = 5;

		
		// �� 4���� ����Ʈ�� ���� ������ 5�� ����Ʈ�� ����ϴ�.
		int** i_list_array = new int* [rerunNumber];
		int** q_list_array = new int* [rerunNumber];
		int** nm_list_array = new int* [rerunNumber];
		int** h_list_array = new int* [rerunNumber];
		
		//�� ����Ʈ�� ���� 5���� ����Ʈ�� ����� �ݴϴ�.
		for (int i = 0; i < rerunNumber; i++) {
			i_list_array[i] = new int[N + 2];
			q_list_array[i] = new int[N + 1];
			nm_list_array[i] = new int[N];
			h_list_array[i] = new int[N + 1];
		}
		
		// �����մϴ�.
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

		// �˰��� �� ���� ���� �� �ð� ����.
		// ������ ���� ���Ͽ� ���� 5�� �ݺ��Ͽ�, �� 5���� ��� ���� �ð��� �����Ѵ�
		for (int k = 0; k < rerunNumber; k++) {
			for (int x = 0; x < 4; x++) { // 0:insertion, 1:quick, 2:natural merge, 3:heap

				// ���� ����
				gettimeofday(&start_t, NULL); // ������ �����ϱ� ���� ���� �ð��� �����մϴ�.

				//���� ����
				switch (x) { //x�� ���� ���缭 ������ ������ �����մϴ�.
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

				// ���� ��
				gettimeofday(&end_t, NULL); // ������ ������ ���� �ð��� �����մϴ�.

				// ����ũ�� ������ �ð��� ������ �ݴϴ�.
				// diff_t = ���� ���� + �Ҽ��� ���� (ex. 1.123456�� = 1.0 + 0.123456)
				diff_t = (double)(end_t.tv_sec - start_t.tv_sec) + ((double)(end_t.tv_usec - start_t.tv_usec) / 1000000);

				// 0:insertion, 1:quick, 2:natural merge, 3:heap
				// �� �˰��� �� ���� �ð� ����
				sum_result[x] += diff_t;
			}
		}

		// ��� ���� �ð��� ����
		for (int x = 0; x < 4; x++) {
			result[x] = sum_result[x] / rerunNumber;
		}

		// 0:insertion, 1:quick, 2:natural merge, 3:heap
		ofstream result_file;
		result_file.open("result.txt", ios::app);
		result_file << result[0] << "," << result[1] << "," << result[2] << "," << result[3] << endl;
		result_file.close();

		//��� ���, �� �κ��� �����Ͻø� �ȵ˴ϴ�.
		cout.precision(5);
		cout << fixed;
		for (int j = 0; j < 4; j++) {
			cout << result[j] << '|';
		}
		cout << "N=" << N << endl;

		//�Ҵ�Ǿ��� �ش� �׽�Ʈ ���̽��� ����Ʈ ����.
		delete[] i_list;
		delete[] q_list;
		delete[] nm_list;
		delete[] h_list;

		// �Ҵ�Ǿ��� �׽�Ʈ ���̽��� ����Ʈ �迭 ����.
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
