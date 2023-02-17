#pragma once
/********
���� �˰����� ���� �� ���� �м� �����Դϴ�. å�� �Ұ��� �������� �˰������ ��
���ϰ� ������ ����Ʈ�� ���� ����ð��� �����Ͽ� �� �˰����� ���⵵�� �м��ϰ�
�����ϴ� ���� ��ǥ�Դϴ�. ������ ���� �˰����� ������ �����ϴ�.
(1) ���� ����(Insertion Sort)
(2) ���� ����(Quick Sort)
(3) ���� �պ� ����(Natural Merge Sort)
(4) �� ����(Heap Sort)
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
	// list[1:n]`�� ���� ���ķ� �����Ѵ�.`
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
		// ���� ���긮��Ʈ�� �ִ� ù ��°, �߾�, ������ Ű �߿��� �޵���� ����
		int pivotIdx = medianIdx(list, left, (left + right) / 2, right);
		
		// �̷��� �������ָ� ���翡�� ����� ���� �ڵ尡 �״�� �����ȴ�.
		swap(list[left], list[pivotIdx]);
		
		T pivot = list[left];
		do {
			do low++; while (list[low] < pivot);
			do high--; while (list[high] > pivot);
			if (low < high) swap(list[low], list[high]);
		} while (low < high);
		swap(list[left], list[high]); // �ǹ��� ���ؼ� ��ġ�� �̵��Ѵ�.

		quick_sort(list, left, high - 1);
		quick_sort(list, high + 1, right);
	}
}

// natural_merge_sort
template <class T>
void Merge(T* initList, T* mergedList, const int l, const int m, const int n)
{
	// initList[l:m]�� initList[m+1:n]�� ���ĵ� ����Ʈ
	// �̵��� ���ĵ� ����Ʈ mergedList[l:n]�� ���յȴ�.
	int i1 = l, i2 = m + 1, iResult = l;
	for (i1 = l, iResult = l, i2 = m + 1;
		i1 <= m && i2 <= n; iResult++) {
		if (initList[i1] <= initList[i2])
			mergedList[iResult] = initList[i1++];
		else
			mergedList[iResult] = initList[i2++];
	}

	// ù ��° ����Ʈ�� ������ ���ڵ�(�� ���� �ִٸ�) ����
	copy(initList + i1, initList + m + 1, mergedList + iResult);

	// �� ��° ����Ʈ�� ������ ���ڵ�(�� ���� �ִٸ�) ����
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
	const int right = n - 1; // �� ������ ���� �ε���
	int left = 0; // �� ���� ���� �ε���
	T* mergedList = new T[n]; // ���յ� ����Ʈ
	bool sorted = false;
	static int count = 0;
	// resultList�� list ����
	copy(list, list + n, list);

	// ���ĵ��� �ʾ��� �� ��� merge sort�� �����Ѵ�.
	do {
		sorted = true;

		// left ���� l����, l+1���� right���� ������� �����.
		// �� ����� �̹� ���ĵ� ����

		// ������� ���� ����, T* list�� list�� ũ���� size�� ���� �̷���� �ִ�.
		vector<pair<T*, int>> partition;

		// ������� �����.
		for (int i = left; i <= right; i++) {
			int l = i;
			int r = l;
			while (r < right && list[r] <= list[r + 1]) {
				r++;
			}
			int sublist_size = r - l + 1;
			T* sublist = new T[sublist_size];
			copy(list + l, list + r + 1, sublist);

			// ������� ���Ϳ� ��´�.
			partition.push_back(make_pair(sublist, sublist_size));
			i = r;
		}
		int append_index = 0; // resultList�� append_index
		// �� ����� merge�� �ִ´�.
		for (int i = 0; i + 1 < partition.size(); i += 2) {
			T* leftList = partition[i].first;
			T* rightList = partition[i + 1].first;
			int leftSize = partition[i].second;
			int rightSize = partition[i + 1].second;

			// �� ����� �״�� �̾���� sumList�� �����.
			int sumSize = leftSize + rightSize;
			T* sumList = new T[sumSize];
			copy(leftList, leftList + leftSize, sumList);
			copy(rightList, rightList + rightSize, sumList + leftSize);

			Merge(sumList, mergedList, 0, leftSize - 1, leftSize + rightSize - 1);
			// mergedList�� �״�� resultList�� append�Ѵ�.
			copy(mergedList, mergedList + sumSize, list + append_index);
			append_index += sumSize;
			sorted = false;


			delete[] leftList;
			delete[] rightList;
			delete[] sumList;
		}
		// ���� ����� Ȧ������� ������ ����� �״�� resultList�� append�Ѵ�.
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
	// root�� ��Ʈ ���� �ϴ� ���� Ʈ���� ���� ������ �����ϵ��� ����
	// root�� ���ʰ� ������ ����Ʈ���� �̹� ���� ������ �����ϰ� �ִ�. ��� �ε����� n���� ũ�� �ʴ�.
	T e = list[root];
	// e�� ���� ������ ��ġ�� Ž��
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
	// list[1:n]�� �񰨼� ������ �����Ѵ�.
	for (int i = n / 2; i >= 1; i--) // ������ ����
		Adjust(list, i, n);
	
	for (int i = n - 1; i >= 1; i--) { // sort
		swap(list[1], list[i + 1]); // �� ������ ó���� �������� ��ȯ
		Adjust(list, 1, i); // ������ ����
	}
}

// print list
// list[1:n]�� ����Ѵ�.
template <class T>
void print_list(T* list, const int n)
{
	for (int i = 1; i <= n; i++)
		cout << list[i] << " ";
	cout << endl;
}