// ConsoleApplication1.cpp : Defines the entry point for the console application.
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

class Human {
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

	void set_up_array();
	void create_grid();
	void move();
};

void Human::set_up_array() {  //creates a double pointer array. This will be the "grid".
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

void Human::create_grid(){
	int rand_column = rand() % columns;
	int rand_row = rand() % rows;

	pawn = grid_arr[c][r];
	goal = grid_arr[rand_column][rand_row];

	if (goal == pawn) {
		goal = grid_arr[rand_column][rand_row];
	}
}

void Human::move() {
	cout << "Pawn = " << pawn << "\nGoal = " << goal << endl;
	cout << "Instructions: " << "Use numbers 1, 2, 3, 4 for right, left, down, and up. Respectively." << endl;
	while(pawn != goal){

		cin >> movement;
		cout << endl;
	switch (movement) {
	case 1:
		if (c + 1 <= columns - 1) {
			pawn = grid_arr[c++][r];
			cout << grid_arr[c][r] << endl; 
			cout << "Current Postion: " << pawn;
		}
		break;
	case 2:
		if (c - 1 >= 0) {
			pawn = grid_arr[c--][r];
			cout << grid_arr[c][r] << endl;
			cout << "Current Postion: " << pawn;
		}
		break;
	case 3:
		if (r + 1 <= rows - 1) {
			pawn = grid_arr[c][r++];
			cout << grid_arr[c][r] << endl;
			cout << "Current Postion: " << pawn;
		}
		break;
	case 4:
		if (r - 1 >= 0) {
			pawn = grid_arr[c][r--];
			cout << grid_arr[c][r] << endl;
			cout << "Current Postion: " << pawn;
		}
		break;
	}
			if (pawn == goal) {
				cout << "You have reached the goal" << endl;
				break;
			}

	}
}

int main()
{
	srand(time(NULL));
	Human Fidget;
	Fidget.set_up_array();
	Fidget.create_grid();
	Fidget.move();
    return 0;
}

