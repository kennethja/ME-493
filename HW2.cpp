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
	int rand_column = rand() % columns;
	int rand_row = rand() % rows;

	void set_up_array();
	void create_grid();
	void move();
	void TestC();
	void drawing();
	void TestA();
	void TestB();
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
	pawn = grid_arr[c][r];
	goal = grid_arr[rand_column][rand_row];

	if (goal == pawn) {
		goal = grid_arr[rand_column][rand_row];
	}
}

void Human::move() {
	cout << "Grid Size: " << columns << "X" << rows << endl;
	cout << "Pawn = " << c << "X" << r << "\nGoal = " << rand_column << "X" << rand_row << endl;
	cout << "Instructions: " << "Use numbers 1, 2, 3, 4 for right, left, down, and up. Respectively." << endl;
	while(pawn != goal){

		cin >> movement;
		cout << endl;

	switch (movement) {
	case 1:
		if (c + 1 <= columns - 1) {
			pawn = grid_arr[++c][r];

			/*code for 3d graph
			for (int i = 1; i <= columns; i++)
			{
				for (int j = 1; j <= rows; j++)
				{
					if (i == c && j == r)
					{
						cout << " A ";
					}
					else if (i == rand_column && j == rand_row)
					{
						cout << " F ";
					}
					else
						cout << " + ";
				}
				cout << endl;
			}
			*/

			cout << "Current Postion: " << c << "X" << r << endl;
		}
		else {
			cout << "You hit a wall, choose a different move." << endl;
		}
		TestA();
		break;
	case 2:
		if (c - 1 >= 0) {
			pawn = grid_arr[--c][r];
			cout << "Current Postion: " << c << "X" << r << endl;
		}
		else {
			cout << "You hit a wall, choose a different move." << endl;
		}
		TestA();
		break;
	case 3:
		if (r + 1 <= rows - 1) {
			pawn = grid_arr[c][++r];
			cout << "Current Postion: " << c << "X" << r << endl;
		}
		else {
			cout << "You hit a wall, choose a different move." << endl;
		}
		TestA();
		break;
	case 4:
		if (r - 1 >= 0) {
			pawn = grid_arr[c][--r];
			cout << "Current Postion: " << c << "X" << r << endl;
		}
		else {
			cout << "You hit a wall, choose a different move." << endl;
		}
		TestA();
		break;
	}
			if (pawn == goal) {
				cout << "You have reached the goal. Yay!" << endl;
				break;
			}
		
	}
}

void Human::TestA() {
	//if (pawn != grid_arr[0][0]) {
		//pawn = grid_arr[0][0];
	//}
	assert(c <= 13 && r <= 13 && c >= 0 && r >= 0);
}

void Human::TestB() {
	move();
	assert(pawn == goal);
}

void Human::TestC() {
	cout << "The goal was: " << rand_column << "X" << rand_row << endl;
	for (int i = 0; i < columns; i++) {
		pawn = grid_arr[c][r];
		c++;
		if (c == rand_column) {
			break;
		}
	}
	for (int i = 0; i < rows; i++) {
		pawn = grid_arr[c][r];
		r++;
		if (r == rand_row + 1) {
			break;
		}
	}
	r = r - 1; // this cancels out the 1 added onto r in the for loop, required for +1 error.
	cout << "The pawn has reached: " << c << "X" << r << endl;
	assert(c == rand_column && r == rand_row);
}

int main()
{
	srand(time(NULL));
	Human Fidget;
	Fidget.set_up_array();
	Fidget.create_grid();
	int user;
	cout << "Enter 1 for Human Controller, Enter 2 for Rule-of-Thumb\n";
	cin >> user;
	cout << endl;
	if (user == 1) {
		Fidget.TestB();
	}
	else {
		Fidget.TestC();
	}
	
    return 0;
}

