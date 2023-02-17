#include <iostream>
#include <stack>
#include <map>
#include <queue>
using namespace std;
const int MAXSIZE = 100; // up to 100 by 100 maze allowed
bool maze[MAXSIZE + 2][MAXSIZE + 2];
bool mark[MAXSIZE + 1][MAXSIZE + 1] = { 0 };
enum directions { N, NE, E, SE, S, SW, W, NW };

struct offsets {
	int a, b;
} movea[8] = { {-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1} };

/*********
* 현재 자기가 어디에 있고, 다음에 어디에 갈 건지를 포함하는 구조체
* x, y좌표가 있고
* dir은 다음에 갈 방향
* 왔던 길은 push, 만약에 막혀서 돌아가는 거면 pop을 한다.
********/
struct Position {
	Position(int xx = 0, int yy = 0, int dd = 0) : x(xx), y(yy), dir(dd) {}
	int x, y, dir;

	// map의 key값으로 이용할 거라서, 정렬을 위한 비교 연산자 정의해줘야 함.
	bool operator< (const Position& tem) const {
		if (x > tem.x) {
			return true;
		}
		else if (x < tem.x) {
			return false;
		}
		else {
			if (y > tem.y) {
				return true;
			}
			else if (y < tem.y) {
				return false;
			}
			else return false;
		}
	}
};

template <class T>
ostream& operator<< (ostream& os, stack<T>& s) {
	// 스택의 내용을 역순으로 출력
	// 구현방법=내용을 하나씩 꺼내 다른 임시 스택에 넣어 저장한 후,
	// 최종적으로 그 임시 스택에서 하나씩 꺼내 출력하면 됨
	stack<T> temp; // 역으로 출력하기 위해 임시 스택 temp 이용

	// s에서 하나씩 꺼내서, temp에 저장한다.
	while (!s.empty()) {
		temp.push(s.top());
		s.pop();
	}

	// temp에서 하나씩 꺼내서, os에 저장한다.
	while (!temp.empty()) {
		os << temp.top();
		temp.pop();
	}
	
	return os;
}
template <class T>
ostream& operator<< (ostream& os, queue<T>& q) {
	while (!q.empty()) {
		os << q.front();
		q.pop();
	}
	return os;
}
ostream& operator<<(ostream& os, Position& item)
{
	// 5개의 Items가 출력될 때마다 줄바꾸기위해
	static int count = 0;
	os << " -> " << "(" << item.x << "," << item.y << ")";
	count++;
	if ((count % 5) == 0) cout << endl;
	return os;
}
bool operator==(const Position& a, const Position& b) {
	if (a.x == b.x && a.y == b.y) {
		return true;
	}
	else return false;
}
bool operator!=(const Position& a, const Position& b) {
	if (a.x != b.x || a.y != b.y) {
		return true;
	}
	else return false;
}

void Path(const int m, const int p)
{
	const int numberOfEntireNode = m * p; ///> 전체 노드 개수
	int visitedNode = 0; ///> 방문한 노드 개수
	mark[1][1] = 1;
	stack<Position> stack;
	Position temp(1, 1, E); // 시작 위치
	stack.push(temp);
	
	while (!stack.empty()) {
		temp = stack.top(); stack.pop(); // unstack
		int i = temp.x; int j = temp.y; int d = temp.dir; ///> i, j, d는 각각 현재 위치 정보

		// move forward
		while (d < 8) {
			int g = i + movea[d].a; int h = j + movea[d].b; ///> g, h는 다음 이동 정보
			if ((g == m) && (h == p)) { // reached exit
				cout << stack;
				temp.x = i; temp.y = j; cout << temp;
				temp.x = m; temp.y = p; cout << temp << endl;
				visitedNode++; // 마지막 방문 노드 개수까지 추가함
				cout << endl << "#nodes visited = " << visitedNode << " out of " << numberOfEntireNode << endl;
				return;
			}

			if ((!maze[g][h]) && (!mark[g][h])) { // new position
				mark[g][h] = 1; // 방문한 적이 있다고 표시
				temp.x = i; temp.y = j; temp.dir = d + 1; // 현 위치와 실패 시 다음에 시도할 방향 저장
				stack.push(temp); // stack it
				i = g; j = h; d = N; // 새로운 위치 정보 갱신
				visitedNode++;
			}
			else d++; // 새로운 위치를 찾지 못 했을 경우 방향을 바꿔가며 찾음
		}
	}

	cout << "No path in maze." << endl;
}

/***********
* 이동을 담당하는 함수, cur에다가 현재 위치, dir에 방향을 담으면
* 그 방향으로 한 칸 이동한 위치가 반환됨
***********/
Position moveDir(const Position& cur, int dir) {
	Position forward;
	forward.x = cur.x + movea[dir].a;
	forward.y = cur.y + movea[dir].b;
	forward.dir = dir;

	return forward;
}

void backtracking(map<Position, Position>& moves, Position& trackStartPoint) {
	stack<Position> list; ///> map 값을 역순으로 출력해야 하므로, stack에 넣었다가 다시 뺀다.
	Position cur = trackStartPoint;
	while (cur != moves[cur]) {
		list.push(cur);
		cur = moves[cur];
	}
	list.push(cur); // 시작 위치까지 집어넣어줌

	while (!list.empty()) {
		cout << list.top(); list.pop();
	}
}
/*********
* function : ShortestPath
* parameters
*	m : 행
*	p : 열
* description :
*	m * n 미로의 최단 경로를 찾는다. queue를 이용하고, bfs 알고리즘을 사용한다.
*	현재 위치에서 움직일 수 있는 모든 경우의 수를 inqueue한다.
*	queue를 차례로 dequeue하여, 각 dequeue한 position마다 새로이 다시금 움직일 수 있는 모든 경우의 수를 inqueue한다.
*	이를 반복하다가, exit()에 reach하면, 그것이 shortest path가 된다.
*	상세한 설명은 latex 참조
*	https://youtu.be/ZuHW4fS60pc <- 해당 영상 알고리즘을 참고하였음.
********/
void ShortestPath(const int m, const int p)
{
	const int numberOfEntireNode = m * p; ///> 전체 노드 개수
	int visitedNode = 0; ///> 방문한 노드 개수
	// stack 안에다가 큰 배열 사용 시 C6262 overflow error를 일으킬 가능성이 있어서, 배열은 엥간하면 동적 배열로 heap에다가 저장.
	bool** mark_short = new bool* [MAXSIZE + 1]; ///> 그냥 mark는 path함수가 사용했으므로, Shortest가 사용할 mark_short 표시
	for (int i = 0; i < MAXSIZE + 1; i++) {
		mark_short[i] = new bool[MAXSIZE + 1]();
	}

	bool** mark_frontier = new bool* [MAXSIZE + 1]; ///> frontier queue에 이미 추가된 장소를 표시. 이미 queue에 대기중이라면 애써 또 추가할 필요 없다
	for (int i = 0; i < MAXSIZE + 1; i++) {
		mark_frontier[i] = new bool[MAXSIZE + 1]();
	}

	// 함수 구현에 3가지가 필요함. 
	// current -> 현재 위치
	Position current(1, 1, E); ///> 시작 위치로 초기화한 현재 위치

	// queue 형태의 frontier
	queue<Position> frontier; ///> 여기에 다음으로 이동할 정보들을 담을 거임

	// 방문한 노드들 visited, 이때 visited는 방문 노드와, 그 노드 바로 직전 노드들의 정보를 담는 mapping 형식으로 저장할 거임
	map<Position, Position> visited; ///> 여기에 방문 장소, 그리고 바로 직전 노드들의 정보 담을 거임

	// 앞으로 전진하였을 때 거기가 유효한 장소인지를 판별하는 등의, 전진 위치정보를 담는 position
	Position forward = current;

	map<Position, Position> mothers; ///> frontier를 추가할 때, 추가된 forward에 대한 mother=current 로 지정
	mothers.insert(make_pair(current, current)); // 시작 위치에 대한 mother value는 시작 위치 그 자신

	// 탈출구 노드
	Position exit(m, p, N);

	// 알고리즘 시작
	// 1. place start in Frontier
	frontier.push(current);

	while (!frontier.empty()) {
		current = frontier.front();
		visitedNode++; // 방문 노드 추가
		mark_short[current.x][current.y] = 1; // 노드 방문했다고 표시남김

		// if reached exit
		if (current == exit) {
			visited.insert(make_pair(current, mothers[current]));
			backtracking(visited, current);
			cout << endl << "#nodes visited = " << visitedNode << " out of " << numberOfEntireNode << endl;

			// 끝내기 전에 동적 할당 해제
			for (int i = 0; i < MAXSIZE + 1; i++) {
				delete[] mark_short[i];
				delete[] mark_frontier[i];
			}

			delete[] mark_short; delete[] mark_frontier;
			return;
		}

		for (int d = 0; d < 8; d++) { // N방향부터 8개 방위를 전부 검사함
			forward = moveDir(current, d);

			if (maze[forward.x][forward.y] != 1 && mark_short[forward.x][forward.y] != 1) {
				// 갈 수 있는 길이며, 이전에 가지 않은 길일 때

				// frontier queue에 없을 때
				if (mark_frontier[forward.x][forward.y] != 1) {
					frontier.push(forward);
					mark_frontier[forward.x][forward.y] = 1; // frontier에 추가되었으므로, 추가되었다고 표시함
					mothers.insert(make_pair(forward, current)); // forward의 mother=current를 저장
				}
			}
		}

		// place current cell in visited
		visited.insert(make_pair(current, mothers[current]));

		// dequeue the frontier
		frontier.pop();
	}

	cout << "No path in maze." << endl;
	// 끝내기 전에 동적 할당 해제
	for (int i = 0; i < MAXSIZE + 1; i++) {
		delete[] mark_short[i];
		delete[] mark_frontier[i];
	}

	delete[] mark_short; delete[] mark_frontier;
}


void getdata(istream& is, int& m, int& p)
{ // 자료화일을 읽어들여 maze에 저장한다.
	is >> m >> p;
	for (int i = 0; i < m + 2; i++) { 
		maze[i][0] = 1; maze[i][p + 1] = 1; 
	}
	for (int j = 1; j <= p; j++) { 
		maze[0][j] = 1; maze[m + 1][j] = 1; 
	}
	for (int i = 1; i <= m; i++)
		for (int j = 1; j <= p; j++)
			is >> maze[i][j];
}