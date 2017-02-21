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

	void set_up_array();
	void create_grid();
	void move();
};

void Human::set_up_array() {  //creates a double pointer array. This will be the "grid".
	grid_arr = new int*[columns]();
	for (int i = 0; i < columns; i++) {
		grid_arr[i] = new int[rows]();
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
	cout << "Instructions: " << "Use numbers 1, 2, 3, 4 for right, left, down, and up. Respectively." << endl;
	while (pawn != goal){

	cin >> movement;
	switch (movement) {
	case 1:
		pawn = grid_arr[c + 1][r];
		cout << "Current Postion" << pawn;
	case 2:
		pawn = grid_arr[c - 1][r];
		cout << "Current Postion" << pawn;
	case 3:
		pawn = grid_arr[c][r + 1];
		cout << "Current Postion" << pawn;
	case 4:
		pawn = grid_arr[c][r - 1];
		cout << "Current Postion" << pawn;
	}
			if (pawn = goal) {
				cout << "You have reached the goal" << endl;
				break;
			}

	}
}

int main()
{
	Human Fidget;
	Fidget.set_up_array();
	Fidget.create_grid();
	Fidget.move();
    return 0;
}

