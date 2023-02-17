#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <map>
#include <stack>
#include "subway.h"
using namespace std;

// Station functions
Station::Station(string name) {
	this->name = name;
	max_time = 10000; // 10000 is a large number that will be replaced by the actual max distance
}

void Station::printStationInfo()
{
	cout << "Station name: " << name << endl;
	for (int i = 0; i < connections.size(); i++) {
		cout << "\t";
		connections[i]->printConnectorInfo();
	}
}

Station::~Station()
{
	for (int i = 0; i < connections.size(); i++) {
		delete connections[i];
	}
}

// operator <<
ostream& operator<<(ostream& os, const Station& station) {
	os << station.name;
	return os;
}

// Connector functions
Connector::Connector(Station* station, int line, int distance) {
	this->dst = station;
	this->line = line;
	this->distance = distance;
}

void Connector::printConnectorInfo()
{
	cout << "dst: " << dst->name << " line: " << line << " distance: " << distance << endl;
}
// class Subway
// function that insert a station into the station_list
void Subway::insert_station(string name) {
	Station* station = new Station(name);
	station_list.insert(pair<string, Station*>(name, station));
}

// function that add a connection between two stations
void Subway::add_connection(string src, string dst, int line1, int line2) {
	
	Station* station1 = station_list[src];
	Station* station2 = station_list[dst];
	int distance = 60; // basic distance

	// when trasfering, distance = 30
	if (src == dst && line1 != line2) {
		distance = 30;
	}
	else if (src == dst && line1 == line2) {
		string error = "Error: " + src + " and " + dst + " are the same station, but the same line";
		throw error;
	}
	
	
	Connector* connector1 = new Connector(station2, line1, distance);
	Connector* connector2 = new Connector(station1, line2, distance);
	station1->connections.push_back(connector1);
	station2->connections.push_back(connector2);
}

Subway::~Subway()
{
	// delete all stations and Connectors
	for (map<string, Station*>::iterator it = station_list.begin(); it != station_list.end(); it++) {
		delete it->second;
	}
}

void Subway::make_station(string src, int line1, string dst, int line2)
{
	insert_station(src);
	insert_station(dst);

	// add connection
	add_connection(src, dst, line1, line2);
}

// dir_list 중 가장 짧은 시간을 가진 경로를 출력
void Subway::print_shortest_path()
{
	set_shortest_path();
	
	std::cout << "Shortest path : " << endl;
	print_path(min_path);

	int minute = min_time / 60;
	int seconds = min_time % 60;
	std::cout << "Time : " << minute << " min " << seconds << " sec" << " (" << min_time << " seconds)" << endl;
}

void Subway::find_shortest_path(const string src, const string dst, const int line)
{
	// if src or dst are not in the station_list, throw an error
	if (station_list.find(src) == station_list.end()) {
		string error = "Error: " + src + "is not in the station_list";
		throw error;
	}
	else if (station_list.find(dst) == station_list.end()) {
		string error = "Error: " + dst + "is not in the station_list";
		throw error;
	}
	
	vector<Station*> path;
	try {
		find_shortest_path(path, src, dst, line, 0);
	}
	catch (string error) {
		std::cout << error << std::endl;
	}
	print_shortest_path();
}


// min_path의 역 중에서, src와 dst까지의 거리의 차가 가장 짧은, 중간 지점의 역을 찾아서 출력
void Subway::find_middle_station(string src, string dst)
{
	if (min_path.size() == 0) {
		std::cout << "ERROR : There is no path between " << src << " and " << dst << endl;
		return;
	}
	
	Station* src_station = station_list[src];
	Station* dst_station = station_list[dst];
	int min_distance = 10000;
	Station* min_station = min_path[0];
	
	int time_src = 0;
	int time_dst = 0;
	
	for (int i = 0; i < min_path.size(); i++) {
		if (min_path[i] == src_station || min_path[i] == dst_station) {
			continue;
		}
		
		// find stations that has the smallest absolete value of the difference 
		// between the distance from src and dst
		int distance = abs(abs(min_path[i]->max_time - src_station->max_time) 
			- abs(min_path[i]->max_time - dst_station->max_time));
		if (distance < min_distance) {
			min_distance = distance;
			min_station = min_path[i];
			time_src = abs(min_path[i]->max_time - src_station->max_time);
			time_dst = abs(min_path[i]->max_time - dst_station->max_time);
		}
	}
	

	std::cout << "Middle station : " << min_station->name << endl;
	std::cout << src << " - " << min_station->name << " time : " 
		<< time_src / 60 << " min " << time_src % 60 << " sec (" 
		<< time_src << " seconds)" << endl;
	std::cout << dst << " - " << min_station->name << " time : " 
		<< time_dst / 60 << " min " << time_dst % 60 << " sec (" 
		<< time_dst << " seconds)" << endl;
	std::cout << "The difference in time taken : " << abs(time_src - time_dst) << " seconds" << endl;
}

void Subway::print_station_info(string name)
{
	// if name is not in station_list, throw an error
	if (station_list.find(name) == station_list.end()) {
		string error = "Error: " + name + "is not in the station_list";
		throw error;
	}
	// print station info
	Station* station = station_list[name];
	station->printStationInfo();
}

void Subway::printAllStations()
{
	for (map<string, Station*>::iterator it = station_list.begin(); it != station_list.end(); it++) {
		it->second->printStationInfo();
	}
}

void Subway::print_path(vector<Station*> path)
{
	int count = 0;
	for (int i = 0; i < path.size(); i++) {
		if (count++ == 5) {
			std::cout << endl;
			count = 0;
		}
		std::cout << path[i]->name;
		if (i != path.size() - 1) {
			std::cout << " -> ";
		}
	}
	std::cout << endl;
}

void Subway::set_shortest_path()
{
	int min_time = 10000;
	int min_index = 0;
	for (int i = 0; i < dir_list.size(); i++) {
		if (dir_list[i].second < min_time) {
			min_time = dir_list[i].second;
			min_index = i;
		}
	}

	// min_path에 최소 경로 저장
	min_path = dir_list[min_index].first;
	this->min_time = dir_list[min_index].second;
}

// station_dir : 현재까지 지나온 경로
// current : 현재 위치
// dst : 목적지(destination)
// current_line : 현재 경유중인 노선 (line)
// time : 현재까지 걸린 시간 (기본은 0)
void Subway::find_shortest_path(vector<Station*> station_dir, string current, string dst, int current_line, int time = 0)
{
	// If the time that comes in is smaller than the max_time of the current, change the max_time of the current station to time.
	if (time < station_list[current]->max_time) {
		station_list[current]->max_time = time;
		// add current station to station_dir
		station_dir.push_back(station_list[current]);
	}
	else {
		// 새로 들어온 time이 current의 max_time보다 크거나 같은 경우, 더 이상 진행하지 않는다.
		return;
	}
	
	// 만약 current가 dst와 같은 경우, station_dir와 time을 dir_list에 추가한다.
	if (current == dst) {
		dir_list.push_back(make_pair(station_dir, time));
		return;
	}
	
	for (int i = 0; i < station_list[current]->connections.size(); i++) {
		Connector* station = station_list[current]->connections[i];
		// transfer time : basically 0, but when transfering, it takes 30 seconds
		// when the line of current and next station is different, it is a transfer station
		int transfer_time = 0;
		if (current_line != station->line) {
			transfer_time = 30;
		}
		
		string next_station = station->dst->name;
		int next_line = station->line;
		int totalTime = time + station->distance + transfer_time;

		// 현재 위치에서 갈 수 있는 모든 station에 대해 재귀적으로 find_shortest_path를 호출한다.
		// we have to pass station_dir by deep copy
		find_shortest_path(station_dir, next_station, dst, next_line, totalTime);
	}
}


