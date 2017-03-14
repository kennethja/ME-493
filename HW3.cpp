// HW3.cpp : Defines the entry point for the console application.
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
	int columns = 13;
	int rows = 13;
	int pawn = 0;
	int goal = 0;
	int movement = 0;
	int c = 0;
	int r = 0;
	int curr = 0;
	int rand_column = rand() % columns;
	int rand_row = rand() % rows;

	void set_up_array();
	void create_grid();
	int update(int,int);
	void rot();
	void drawing();
	int create_walls(int);
	int reward(int);
	void TestA(int, int, int, int);
};

void Grid::set_up_array() {  //creates a double pointer array. This will be the "grid".
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

void Grid::create_grid() {
	pawn = grid_arr[c][r];
	goal = grid_arr[rand_column][rand_row];

	if (goal == pawn) {
		goal = grid_arr[rand_column][rand_row];
	}
}

int Grid::create_walls(int pawn) {
	int tell = 0;
	int wall1 = grid_arr[rand_column - 1][rand_row];
	int wall2 = grid_arr[rand_column][rand_row - 1];
	if (pawn == wall1 || pawn == wall2) {
		tell = 1;
	}
	return tell;
}

int Grid::update(int state, int movement) {
		if (c < rand_column) {
			pawn = grid_arr[r][++c];
		}
		else if (c > rand_column) {
			pawn = grid_arr[r][--c];
		}
		else if (c == rand_column) {
			cout << "Goal column has been reached." << endl;
			
		}

		if (r < rand_row) {
			pawn = grid_arr[++r][c];
			
		}
		else if (r > rand_row) {
			pawn = grid_arr[--r][c];
		}
		else if (r == rand_row) {
			cout << "Goal row has been reached." << endl;
			
	}
		return pawn;

}

void Grid::rot() {
	int tell = 0;
	cout << "The goal was: " << rand_column << "X" << rand_row << endl;
	while (1) {
		if (c < rand_column) {
			tell = create_walls(pawn);
			if (tell == 1) {
				pawn = grid_arr[r][--c];
			}
			else {
				pawn = grid_arr[r][++c];
			}
			
		}
		else if (c > rand_column) {
			tell = create_walls(pawn);
			if (tell == 1) {
				pawn = grid_arr[r][++c];
			}
			else {
				pawn = grid_arr[r][--c];
			}
		}
		else if (c == rand_column) {
			cout << "Goal column has been reached." << endl;
			break;
		}
	}
	while (1) {
		if (r < rand_row) {
			tell = create_walls(pawn);
			if (tell == 1) {
				pawn = grid_arr[--r][c];
			}
			else {
				pawn = grid_arr[++r][c];
			}
	}
		else if (r > rand_row) {
			tell = create_walls(pawn);
			if (tell == 1) {
				pawn = grid_arr[r][--c];
			}
			else {
				pawn = grid_arr[r][++c];
			}
	}
		else if (r == rand_row) {
			cout << "Goal row has been reached." << endl;
			break;
	}
		TestA(c, r, rand_column, rand_row);
}	
}

int Grid::reward(int new_pawn) {
	int R = -1; //sets the default reward
	if (new_pawn == goal) {
		R = R + 100;
		new_pawn = pawn = c = r = 0;

	}
	return R;
}

void Grid::TestA(int c, int r, int rand_col, int rand_r) {
	assert(c == rand_col && r == rand_r);
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


int main()
{
	srand(time(NULL));
	Grid Fidget;
	Agent Luna(&Fidget);
	Fidget.set_up_array();
	Fidget.create_grid();
	int user;
	cout << "Enter 1 for Rule-of-Thumb, Enter 2 for Q-Learner.\n";
	cin >> user;
	cout << endl;
	if (user == 1) {
		Fidget.rot();
	}
	//else if (user == 2) {
		//Luna.action();
	//}

	return 0;
}
