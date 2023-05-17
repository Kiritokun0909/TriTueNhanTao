/*
    @author:    Dau Van Hoang(N20DCCN017), 
                Ho Duc Hoang(N20DCCN018), 
                Nguyen Duc Huy(N20DCCN021)
    *Subject: A* algorithm for 8-puzzle
*/

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using namespace std;

const string INPUT_FILE_PATH = "test.txt";
const string OUTPUT_FILE_PATH = "answer.txt";

const int MATRIX_SIZE = 3;


struct Coordinate{
	int x;
	int y;
};
unordered_map<string, Coordinate> goal_position;

struct Node{
	//Current state of the node's puzzle
	vector<vector<string>> puzzle;
	//g(n): the cost to get from start_puzzle to curr_puzzle
	int g;
	//h(n): the estimated cost of the cheapest path from n to the goal
	int h;
	//f(n): estimated cost of the cheapest solution through n
	int f;
	//Key to verify curr_state
	string key;
};
Node start, goal;

struct Solution{
	vector<Node> path;
	int total_cost = 0;
};

int herStic(Node curr_state){
	int h = 0;	//Best way to goal_puzzle
	for(int i = 0; i < MATRIX_SIZE; i++){
		for(int j = 0; j < MATRIX_SIZE; j++){
			string curr_piece = curr_state.puzzle[i][j];
			h += abs(i - goal_position[curr_piece].x) +
				abs(j - goal_position[curr_piece].y);
		}
	}
	return h;
}

string generate_key(vector<vector<string>> &puzzle){
	string key = "";
	for(int i = 0; i < MATRIX_SIZE; i++){
		for(int j = 0; j < MATRIX_SIZE; j++){
			key += puzzle[i][j];
		}
	}
	return key;
}

void initPuzzle() {
	ifstream input; input.open(INPUT_FILE_PATH, ios::in);
	string s;
	input >> s;
	for (int i = 0; i < MATRIX_SIZE; i++){
		vector<string> vt;
		for (int j = 0; j < MATRIX_SIZE; j++) {
			input >> s;
			vt.push_back(s);
		}
		start.puzzle.push_back(vt);
	}
	start.key = generate_key(start.puzzle);
	start.g = 0;
	
	input >> s;
	for (int i = 0; i < MATRIX_SIZE; i++){
		vector<string> vt;
		for (int j = 0; j < MATRIX_SIZE; j++) {
			input >> s;
			goal_position.insert({s, {i, j}});
			vt.push_back(s);
		}
		goal.puzzle.push_back(vt);
	}
	goal.key = generate_key(goal.puzzle);
	goal.g = 0;
	input.close();
}

bool isFinished(Node curr_state){
	if(goal.key.compare(curr_state.key) == 0) return true;
	return false;
}

bool operator < (const Node& a, const Node& b){
	return a.f > b.f;
}

Coordinate find_x(vector<vector<string>> &puzzle){
	Coordinate coordinate;
	for (int i = 0; i < MATRIX_SIZE; i++){
		for (int j = 0; j < MATRIX_SIZE; j++) {
			if(puzzle[i][j] == "x"){
				coordinate.x = i;
				coordinate.y = j;
				return coordinate;
			}
		}
	}
	return coordinate;
}

vector<Node> expand_node(Node curr_node){
	Coordinate X = find_x(curr_node.puzzle);
	vector<Node> expanded;
	
	cout << "Curent Node: ";
	cout << curr_node.key << ", f(n)=" << curr_node.f << '\n';

	Node tmp;
	int h = 0;
	//Can move up
	if(X.x > 0){
		tmp = curr_node;
		swap(tmp.puzzle[X.x][X.y], tmp.puzzle[X.x - 1][X.y]);
		tmp.key = generate_key(tmp.puzzle);
		tmp.g++;
		tmp.h = herStic(tmp);
		tmp.f = tmp.g + tmp.h;

		cout << "Expend Node: ";
		cout << tmp.key << ", g(n)=" << tmp.g;
		cout << ", h(n)=" << tmp.h;
		cout << ", f(n)=" << tmp.f << '\n';

		expanded.push_back(tmp);
	}
	//Can move down
	if(X.x < MATRIX_SIZE - 1){
		tmp = curr_node;
		swap(tmp.puzzle[X.x][X.y], tmp.puzzle[X.x + 1][X.y]);
		tmp.key = generate_key(tmp.puzzle);
		tmp.g++;
		tmp.h = herStic(tmp);
		tmp.f = tmp.g + tmp.h;

		cout << "Expend Node: ";
		cout << tmp.key << ", g(n)=" << tmp.g;
		cout << ", h(n)=" << tmp.h;
		cout << ", f(n)=" << tmp.f << '\n';
		
		expanded.push_back(tmp);
	}
	//Can move left
	if(X.y > 0){
		tmp = curr_node;
		swap(tmp.puzzle[X.x][X.y], tmp.puzzle[X.x][X.y - 1]);
		tmp.key = generate_key(tmp.puzzle);
		tmp.g++;
		tmp.h = herStic(tmp);
		tmp.f = tmp.g + tmp.h;

		cout << "Expend Node: ";
		cout << tmp.key << ", g(n)=" << tmp.g;
		cout << ", h(n)=" << tmp.h;
		cout << ", f(n)=" << tmp.f << '\n';

		expanded.push_back(tmp);
	}
	//Can move right
	if(X.y < MATRIX_SIZE - 1){
		tmp = curr_node;
		swap(tmp.puzzle[X.x][X.y], tmp.puzzle[X.x][X.y + 1]);
		tmp.key = generate_key(tmp.puzzle);
		tmp.g++;
		tmp.h = herStic(tmp);
		tmp.f = tmp.g + tmp.h;

		cout << "Expend Node: ";
		cout << tmp.key << ", g(n)=" << tmp.g;
		cout << ", h(n)=" << tmp.h;
		cout << ", f(n)=" << tmp.f << '\n';

		expanded.push_back(tmp);
	}

	return expanded;
}

Solution Astar(){
	Solution sol;

	priority_queue<Node> frontier;
	frontier.push(start);

	unordered_map<string, Node> trace;
	trace.insert({start.key, start});

	while(!frontier.empty()){
		Node curr_node = frontier.top();
		frontier.pop();

		if(isFinished(curr_node)){
			int cost = 0;
			while(curr_node.key != start.key){
				sol.path.push_back(curr_node);
				curr_node = trace[curr_node.key];
				cost++;
			}
			sol.path.push_back(curr_node);
			sol.total_cost = trace[goal.key].g;
			reverse(sol.path.begin(), sol.path.end());
			return sol;
		}

		vector<Node> expanded = expand_node(curr_node);
		for(const Node& node:expanded) {
            if(trace.count(node.key) == 0 || node.g < trace[node.key].g) {
                frontier.push(node);
                trace[node.key] = curr_node;
            }
        }
	}

	return sol;
}

void write_solution_to_file(Solution sol){
	ofstream outFile; outFile.open(OUTPUT_FILE_PATH, ios::trunc);

	for (int k = 0; k < sol.path.size(); k++) {
		outFile << "Trang thai " << k + 1 << "(g=" << sol.path[k].g << ",h=" << herStic(sol.path[k]) << "):\n";
		vector<vector<string>> puzzle = sol.path[k].puzzle;
		for(int i = 0; i < MATRIX_SIZE; i++){
			for(int j = 0; j < MATRIX_SIZE; j++){
				outFile << puzzle[i][j] << " ";
			}
			outFile << "\n";
		}
		outFile << "\n";
	}

	outFile << "Total step: " << sol.path.size() - 1;

	outFile.close();
}

int main(){
	initPuzzle();
	
	Solution sol = Astar();
	write_solution_to_file(sol);


	return 0;
}