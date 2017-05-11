//Kenneth Allison
//ME 493
//Final Project

#include "stdafx.h";
#include "iostream";
#include "assert.h";
#include "string";
#include "random";
#include "time.h";
#include "vector";
#include "fstream";
#include "cmath";
#include "limits";

using namespace std;

class Grid {
public:
	int **grid_arr;
	int **grid_arr1;
	int **new_rep;
	int pawn = 0;
	int pawn1 = 0;
	int goal = 0;
	int goal1 = 0;
	int movement = 0;
	int c = 0;
	int c1 = 0;
	int r = 0;
	int r1 = 0;
	int rand_column = 8;//rand() % 15;
	int rand_row = 8;//rand() % 15;
	int rand_column1 = 8;
	int rand_row1 = 8;
	int R;
	int columns = 15;
	int columns1 = 15;
	int rows = 15;
	int rows1 = 15;
	int size = columns * rows;
	int move_count;
	int counter = 0;
	int testGstate = 0;

	void setgrid();
	void setgrid1();
	void creategrid();
	void creategrid1();
	int update(int, int);
	int update1(int, int);
	int reward(int new_state);
	int reward1(int new_state1);
};

void Grid::setgrid() {  //creates a double pointer array. This will be the "grid".
	grid_arr = new int*[columns]();
	int count = 0;
	for (int i = 0; i < columns; i++) {
		grid_arr[i] = new int[rows]();
	}
	for (int j = 0; j < rows; j++) {
		for (int i = 0; i < columns; i++) {
			grid_arr[i][j] = count++;
		}
	}
}

void Grid::setgrid1() {  //creates a double pointer array. This will be the "grid".
	grid_arr1 = new int*[columns1]();
	int count = 0;
	for (int i = 0; i < columns1; i++) {
		grid_arr1[i] = new int[rows1]();
	}
	for (int j = 0; j < rows1; j++) {
		for (int i = 0; i < columns1; i++) {
			grid_arr1[i][j] = count++;
		}
	}
}

void Grid::creategrid() {
	pawn = grid_arr[c][r];
	goal = grid_arr[rand_column][rand_row];

	if (goal == pawn) {
		goal = grid_arr[rand_column][rand_row];
	}

	cout << "The agent begins at: " << c << " X " << r << endl;
	cout << "The goal is located at: " << rand_column << " X " << rand_row << endl;
}

void Grid::creategrid1() {
	pawn1 = grid_arr1[c1][r1];
	goal1 = grid_arr1[rand_column1][rand_row1];

	if (goal1 == pawn1) {
		goal1 = grid_arr1[rand_column1][rand_row1];
	}

	cout << "The agent begins at: " << c1 << " X " << r1 << endl;
	cout << "The goal is located at: " << rand_column1 << " X " << rand_row1 << endl;
}

int Grid::update(int state, int dir) {
	if (dir == 0) {
		if (c + 1 <= columns - 1) {
			pawn = grid_arr[++c][r];
		}
	}
	else if (dir == 1) {
		if (c - 1 >= 0) {
			pawn = grid_arr[--c][r];
		}
	}
	else if (dir == 2) {
		if (r + 1 <= rows - 1) {
			pawn = grid_arr[c][++r];
		}
	}
	else if (dir == 3) {
		if (r - 1 >= 0) {
			pawn = grid_arr[c][--r];
		}
	}
	//cout << "Current Location: " << c << " X " << r << "    Current State: " << state << endl;
	move_count++;
	return pawn;
}
int Grid::update1(int state, int dir) {
	if (dir == 0) {
		if (c1 + 1 <= columns1 - 1) {
			pawn1 = grid_arr1[++c1][r1];
		}
	}
	else if (dir == 1) {
		if (c1 - 1 >= 0) {
			pawn1 = grid_arr1[--c1][r1];
		}
	}
	else if (dir == 2) {
		if (r1 + 1 <= rows1 - 1) {
			pawn1 = grid_arr1[c1][++r1];
		}
	}
	else if (dir == 3) {
		if (r1 - 1 >= 0) {
			pawn1 = grid_arr1[c1][--r1];
		}
	}
	//cout << "Current Location: " << c << " X " << r << "    Current State: " << state << endl;
	move_count++;
	return pawn;
}

int Grid::reward(int new_pawn) {
	int R = -1; //sets the default reward
	if (new_pawn == goal) {
		R = R + 100;
		new_pawn = pawn = c = r = 0;

	}
	return R;
}

int Grid::reward1(int new_pawn) {
	int R = -1; //sets the default reward
	if (new_pawn == goal1) {
		R = R + 100;
		new_pawn = pawn1 = c1 = r1 = 0;

	}
	return R;
}


class Agent {
public:
	double **Q;
	double alpha = 0.1;
	double epsilon = 0.1;
	double gamma = 0.4;
	int size = 225;
	int response = 0;
	int state = 0;
	int new_state = 0;
	double new_Reward = 0;
	int left = 0;
	int right = 1;
	int up = 2;
	int down = 3;
	int Q_max = 0;
	double Q_calc = 0.0;
	Grid *world;

	Agent(Grid*);
	void setQ(); //create the 2-D array for the Q table. states X 4
	void update_Q(int); //stores and updates the Q values for each state
	void action();
	int greedy_action(double *row);
	int rand_action();
	int decide();
	int setQ_max();
	void TestD(int);
};

Agent::Agent(Grid*grid) {
	world = grid;
}

void Agent::setQ() {  //creates a double pointer array. This will be the q_table array
	Q = new double*[size]();
	int count = 0;
	for (int i = 0; i < size; i++) {
		Q[i] = new double[4]();
	}
}

int Agent::setQ_max() {
	Q_max = Q[new_state][greedy_action(Q[new_state])];
	return Q_max;
}

void Agent::update_Q(int movement) {
	Q_max = setQ_max();
	Q[state][movement] += alpha * (new_Reward + gamma*Q_max - Q[state][movement]);
	TestD(movement);
	state = new_state;
}

void Agent::action() {
	int movement;
	while (1) {
		movement = decide();
		new_state = world->update(state, movement);
		new_Reward = world->reward(new_state);
		update_Q(movement);
		if (new_state == world->goal) {
			//cout << "The computer has successfully located the goal. Run away!\n";
			state = new_state = new_Reward = response = 0;
			break;
		}
	}
}

int Agent::decide() {
	double x = (double)rand() / RAND_MAX;

	if (x < epsilon) {

		response = rand_action(); //create a random action thingy
	}
	else {

		response = greedy_action(Q[state]); //create a greedy action thingy
	}

	return response;
}

int Agent::rand_action() {
	int x = rand() % 4;
	return x;
}

int Agent::greedy_action(double *row) {
	vector<int> max;
	max.push_back(0);

	for (int i = 1; i < 4; i++) {
		if (row[max[0]] < row[i]) {
			max.clear();
			max.push_back(i);
		}
		else if (row[max[0]] == row[i]) {
			max.push_back(i);
		}
	}
	return max[rand() % max.size()];
}

//TestD is included above in the update_Q function. 
//This assert runs on each Q value calculated and placed in the table.
void Agent::TestD(int movement) {
	assert(Q[state][movement] <= 100);
}

int main() {
	srand(time(NULL));

	ofstream myfile;
	myfile.clear();
	myfile.open("LearningCurve.csv");
	//myfile << "Stat_Run" << "\t" << "Episode" << "\t" << "Move_Number" << endl;

	Grid Fidget;
	Fidget.setgrid();
	Fidget.creategrid();

	Agent Luna(&Fidget);
	int t[500] = { 0 };
	for (int i = 0; i < 500; i++) {
		t[i] = 0;
	}

	//statistical runs
	for (int i = 0; i < 30; i++) {
		Luna.setQ();
		for (int j = 0; j < 500; j++) {
			Luna.action();
			t[j] += Fidget.move_count;
			//cout << Fidget.move_count << endl;
			//cout << i << "\t" << j << "\t" << Fidget.move_count << endl;
			//Fidget.TestF(j);
			Fidget.move_count = 0;
		}
		cout << "Statistical Run: " << i << endl;
	}

	for (int i = 0; i < 500; i++) {
		myfile << t[i] / 30 << endl;
	}

	myfile.close();
	return 0;
}
