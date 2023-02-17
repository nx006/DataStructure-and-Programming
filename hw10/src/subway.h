#pragma once
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <map>

using namespace std;

class Connector;

class Station {
public:
	string name;
	vector<Connector*> connections;
	int max_time;
	Station(string name);
	friend ostream& operator<<(ostream& os, const Station& station);
	void printStationInfo();
	~Station();
};

class Connector {
public:
	Station* dst;
	int line;
	int distance;
	Connector(Station* dst, int line, int distance);
	void printConnectorInfo();
};

class Subway {
private:
	map<string, Station*> station_list;
	// vector<Station*> 의 벡터 리스트와 int 시간 정보의 페어
	vector< pair<vector<Station*>, int> > dir_list;
	// 최소 경로와 시간
	vector<Station*> min_path;
	int min_time;

	void set_shortest_path();
	void find_shortest_path(vector<Station*> station_dir, string current, string dst, int current_line, int time);
	void print_path(vector<Station*> path);
	void insert_station(string name);
	void add_connection(string src, string dst, int line1, int line2);
public:
	void make_station(string src, int line1, string dst, int line2);
	void print_shortest_path();
	void find_shortest_path(const string src, const string dst, const int line);

	// src와 dst 사이에 걸리는 시간의 차이가 최소가 되는 middle station을 찾는 함수
	void find_middle_station(string src, string dst);

	void print_station_info(string name);
	void printAllStations();
	~Subway();
};