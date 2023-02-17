#pragma once
/********
정렬 알고리즘의 구현 및 성능 분석 과제입니다. 책에 소개된 내부정렬 알고리즘들을 구
현하고 임의의 리스트에 대한 실행시간을 측정하여 각 알고리즘의 복잡도를 분석하고
이해하는 것이 목표입니다. 구현할 정렬 알고리즘은 다음과 같습니다.
(1) 삽입 정렬(Insertion Sort)
(2) 빠른 정렬(Quick Sort)
(3) 내츄럴 합병 정렬(Natural Merge Sort)
(4) 힙 정렬(Heap Sort)
*********/

#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

template <class T>
void Insert(const T& e, T* list, int i)
{
	list[0] = e;
	while (e < list[i]) {
		list[i + 1] = list[i];
		i--;
	}
	list[i + 1] = e;
}

template <class T>
void insertion_sort(T* list, int n)
{
	// list[1:n]`을 삽입 정렬로 정렬한다.`
	for (int i = 2; i <= n; i++) {
		T temp = list[i];
		Insert(temp, list, i - 1);
	}
}

template <class T>
int medianIdx(T* list, int a, int b, int c)
{
	if (list[a] < list[b]) {
		if (list[b] < list[c]) return b;
		else if (list[a] < list[c]) return c;
		else return a;
	}
	else {
		if (list[a] < list[c]) return a;
		else if (list[b] < list[c]) return c;
		else return b;
	}
}

// quick sort
template <class T>
void quick_sort(T* list, const int left, const int right)
{
	if (left < right) {
		int low = left, high = right + 1;
		// 현재 서브리스트에 있는 첫 번째, 중앙, 마지막 키 중에서 메디안을 선택
		int pivotIdx = medianIdx(list, left, (left + right) / 2, right);
		
		// 이렇게 스왑해주면 교재에서 사용한 기존 코드가 그대로 유지된다.
		swap(list[left], list[pivotIdx]);
		
		T pivot = list[left];
		do {
			do low++; while (list[low] < pivot);
			do high--; while (list[high] > pivot);
			if (low < high) swap(list[low], list[high]);
		} while (low < high);
		swap(list[left], list[high]); // 피벗을 기준선 위치로 이동한다.

		quick_sort(list, left, high - 1);
		quick_sort(list, high + 1, right);
	}
}

// natural_merge_sort
template <class T>
void Merge(T* initList, T* mergedList, const int l, const int m, const int n)
{
	// initList[l:m]과 initList[m+1:n]는 정렬된 리스트
	// 이들은 정렬된 리스트 mergedList[l:n]로 병합된다.
	int i1 = l, i2 = m + 1, iResult = l;
	for (i1 = l, iResult = l, i2 = m + 1;
		i1 <= m && i2 <= n; iResult++) {
		if (initList[i1] <= initList[i2])
			mergedList[iResult] = initList[i1++];
		else
			mergedList[iResult] = initList[i2++];
	}

	// 첫 번째 리스트의 나머지 레코드(가 만약 있다면) 복사
	copy(initList + i1, initList + m + 1, mergedList + iResult);

	// 두 번째 리스트의 나머지 레코드(가 만약 있다면) 복사
	copy(initList + i2, initList + n + 1, mergedList + iResult);
}

// print partition list
template <class T>
void print_partition_list(const pair<T*, int>& partition)
{
	cout << "Partition: ";
	print_list(partition.first, partition.second);
	cout << "size: " << partition.second << endl;
}

// print all  partition list
template <class T>
void print_all_partition_list(const vector<pair<T*, int> >& partition_list)
{
	for (int i = 0; i < partition_list.size(); i++)
		print_partition_list(partition_list[i]);
}

template <class T>
void natural_merge_sort(T* list, const int n)
{
	const int right = n - 1; // 맨 오른쪽 원소 인덱스
	int left = 0; // 맨 왼쪽 원소 인덱스
	T* mergedList = new T[n]; // 병합된 리스트
	bool sorted = false;
	static int count = 0;
	// resultList에 list 복사
	copy(list, list + n, list);

	// 정렬되지 않았을 때 계속 merge sort를 실행한다.
	do {
		sorted = true;

		// left 부터 l까지, l+1부터 right까지 덩어리들을 만든다.
		// 각 덩어리는 이미 정렬된 상태

		// 덩어리들을 담을 벡터, T* list와 list의 크기인 size의 페어로 이루어져 있다.
		vector<pair<T*, int>> partition;

		// 덩어리들을 만든다.
		for (int i = left; i <= right; i++) {
			int l = i;
			int r = l;
			while (r < right && list[r] <= list[r + 1]) {
				r++;
			}
			int sublist_size = r - l + 1;
			T* sublist = new T[sublist_size];
			copy(list + l, list + r + 1, sublist);

			// 덩어리들을 벡터에 담는다.
			partition.push_back(make_pair(sublist, sublist_size));
			i = r;
		}
		int append_index = 0; // resultList의 append_index
		// 각 덩어리를 merge에 넣는다.
		for (int i = 0; i + 1 < partition.size(); i += 2) {
			T* leftList = partition[i].first;
			T* rightList = partition[i + 1].first;
			int leftSize = partition[i].second;
			int rightSize = partition[i + 1].second;

			// 두 덩어리를 그대로 이어붙인 sumList를 만든다.
			int sumSize = leftSize + rightSize;
			T* sumList = new T[sumSize];
			copy(leftList, leftList + leftSize, sumList);
			copy(rightList, rightList + rightSize, sumList + leftSize);

			Merge(sumList, mergedList, 0, leftSize - 1, leftSize + rightSize - 1);
			// mergedList를 그대로 resultList에 append한다.
			copy(mergedList, mergedList + sumSize, list + append_index);
			append_index += sumSize;
			sorted = false;


			delete[] leftList;
			delete[] rightList;
			delete[] sumList;
		}
		// 만약 덩어리가 홀수개라면 마지막 덩어리를 그대로 resultList에 append한다.
		if (partition.size() % 2 == 1) {
			T* lastList = partition[partition.size() - 1].first;
			int lastSize = partition[partition.size() - 1].second;
			copy(lastList, lastList + lastSize, list + append_index);
		}
	} while (!sorted);
}

// heap sort
template <class T>
void Adjust(T* list, const int root, const int n)
{
	// root를 루트 노드로 하는 이진 트리가 히프 성질을 만족하도록 조정
	// root의 왼쪽과 오른쪽 서브트리는 이미 히프 성질을 만족하고 있다. 노드 인덱스는 n보다 크지 않다.
	T e = list[root];
	// e에 대한 적절한 위치를 탐색
	int j = root * 2;
	for (; j <= n; j *= 2) {
		if (j < n && list[j] < list[j + 1]) j++;
		if (e >= list[j]) break;
		list[j / 2] = list[j];
	}
	list[j / 2] = e;
}

template <class T>
void heap_sort(T* list, const int n)
{
	// list[1:n]을 비감소 순으로 정렬한다.
	for (int i = n / 2; i >= 1; i--) // 히프로 조정
		Adjust(list, i, n);
	
	for (int i = n - 1; i >= 1; i--) { // sort
		swap(list[1], list[i + 1]); // 현 히프의 처음과 마지막을 교환
		Adjust(list, 1, i); // 히프로 조정
	}
}

// print list
// list[1:n]을 출력한다.
template <class T>
void print_list(T* list, const int n)
{
	for (int i = 1; i <= n; i++)
		cout << list[i] << " ";
	cout << endl;
}