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
	int rand_column = rand() % columns;
	int rand_row = rand() % rows;
	int R;

	void setgrid(int,int);
	void creategrid();
	int update(int state,int dir);
	int reward(int new_state);
};

void Grid::setgrid(int columns, int rows) {  //creates a double pointer array. This will be the "grid".
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
}

int Grid::update(int state, int dir) {
	if (dir = 0) {
		pawn = grid_arr[++c][r];
	}
	else if (dir = 1){
		pawn = grid_arr[--c][r];
	}
	else if (dir = 2) {
		pawn = grid_arr[c][r++];
	}
	else if (dir = 3) {
		pawn = grid_arr[c][r--];
	}
	return pawn;
}

int Grid::reward(int new_pawn) {
	int R = 0; //sets the default reward to zero
	if (new_pawn == goal) {
		R = R + 100;
	}
	return R;
}

class Agent {
public:
	int **Q;
	int alpha = 0.1;
	int epsilon = 0.1;
	int gamma = 0.9;
	int num_action = 4;
	int response = 0;
	int curr_pos = 0;
	double curr_R = 0;
	int pre_pos = 0;
	double pre_R = 0;
	int left = 0;
	int right = 1;
	int up = 2;
	int down = 3;

	void setQ(int); //create the 2-D array for the Q table. states X 4
	void update_position();
	void update_state();
	void Q_Table(int); //stores and updates the Q values for each state
	void action();
	void decide();
};

void Agent::setQ(int size) {  //creates a double pointer array. This will be the q_table array
	Q = new int*[4]();
	int count = 0;
	for (int i = 0; i < 4; i++) {
		Q[i] = new int[size]();
	}
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < 4; i++) {
			Q[i][j] = count++;
		}
	}
}

void Agent::Q_Table(int R) {

}

void Agent::action() {
	int index;
	int R;
	while (pawn != goal) {
		curr_pos = 
	}
}

void Agent::greedy_action() {
	double check = 0;
	good = 0;
	for (int i = 0; i < 4; i++)
	{

		if (Q > check)
		{

			good = i;
			check = Q.at(i);
		}
	}

	return good;
}

void Agent::decide() {
	double x = (double)rand() / RAND_MAX;

	if (x < epsilon) {

		response = rand_action();
	}
	else {

		response = greedy_action();
	}

	return response;
}

int main() {
	int columns = 10;
	int rows = 10;
	int size = columns * rows;
	Grid Fidget;
	Fidget.setgrid(columns,rows)
	Agent Luna;
	
	return 0;
}