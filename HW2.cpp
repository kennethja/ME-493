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

	void set_up_array();
	void create();
};

void Human::set_up_array() {  //creates a double pointer array. This will be the "grid".
	grid_arr = new int*[columns]();
	for (int i = 0; i < columns; i++) {
		grid_arr[i] = new int[rows]();
	}
}

void Human::create(){
	int rand_column = rand() % columns;
	int rand_row = rand() % rows;

	pawn = grid_arr[0][0];
	goal = grid_arr[rand_column][rand_row];

	if (goal == pawn) {
		goal = grid_arr[rand_column][rand_row];
	}

	for (int i = 0; i <= columns; i++) {
		for (int j = 0; j <= rows; j++) {

		}
	}
}

int main()
{
    return 0;
}

