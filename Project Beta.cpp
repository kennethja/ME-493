// Project Beta.cpp : Defines the entry point for the console application.
//

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
	int pawn = 0;
	int goal = 0;
	int movement = 0;
	int c = 0;
	int r = 0;
	int rand_column = rand() % 10;
	int rand_row = rand() % 10;
	int R;
	int columns = 10;
	int rows = 10;
	int size = columns * rows;

	void setgrid();
	void creategrid();
	int update(int,int);
	int reward(int new_state);
	void TestE(int,int);
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

void Grid::creategrid() {
	pawn = grid_arr[c][r];
	goal = grid_arr[rand_column][rand_row];

	if (goal == pawn) {
		goal = grid_arr[rand_column][rand_row];
	}

	cout << "The agent begins at: " << c << " X " << r << endl;
	cout << "The goal is located at: " << rand_column << " X " << rand_row << endl;
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
		cout << c << " X " << r << endl;
		return pawn;
}

int Grid::reward(int new_pawn) {
	int R = -1; //sets the default reward to zero
	if (new_pawn == goal) {
		R = R + 100;
		new_pawn = pawn = 0;
		TestE(new_pawn,pawn); 
	}
	return R;
}

//ensures that the agent(new_pawn) location on the grid is reset to zero 
//once the goal is reached in the reward function.
void Grid::TestE(int new_pawn, int pawn) {
	int test;
	assert(new_pawn == pawn);
}

class Agent {
public:
	double **Q;
	double alpha = 0.1;
	double epsilon = 0.1;
	double gamma = 0.9;
	int size = 100;
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
	void TestF();
	void TestG();
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
			cout << "The computer has successfully located the goal. Run away!\n";
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

	for (int i = 1;i < 4; i++) {
		if (row[max[0]] < row[i]) {
			max.clear();
			max.push_back(i);
		} 
		else if (row[max[0]] == row[i]){
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

void Agent::TestF() {

}

int main() {
	srand(time(NULL));
	
	Grid Fidget;
	Fidget.setgrid();
	Fidget.creategrid();

	Agent Luna(&Fidget);
	Luna.setQ();
	Luna.action();
	
	return 0;
}
