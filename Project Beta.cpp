//Kenneth Allison
//ME 493
//Project Beta
//Worked with Scott Fourer and Bryant Clouse

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
	int **new_rep;
	int pawn = 0;
	int goal = 0;
	int movement = 0;
	int c = 0;
	int r = 0;
	int rand_column = 8;//rand() % 15;
	int rand_row = 8;//rand() % 15;
	int R;
	int columns = 15;
	int rows = 15;
	int size = columns * rows;
	int move_count;
	int counter = 0;
	int testGstate = 0;

	void setgrid();
	void creategrid();
	int update(int,int);
	int reward(int new_state);
	void TestE(int,int);
	void TestF(int);
	void TestG();
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
		//cout << "Current Location: " << c << " X " << r << "    Current State: " << state << endl;
		move_count++;
		return pawn;
}

int Grid::reward(int new_pawn) {
	int R = -1; //sets the default reward
	if (new_pawn == goal) {
		R = R + 100;
		new_pawn = pawn = c = r = 0;
		TestE(new_pawn,pawn); 
		
	}
	return R;
}

//ensures that the agent(new_pawn) location on the grid is reset to zero 
//once the goal is reached in the reward function.
void Grid::TestE(int new_pawn, int pawn) {
	assert(new_pawn == pawn == c == r);
}

//ensures that the final number of moes the agent takes 
//is within resonable bounds of the optimal number of steps
void Grid::TestF(int j) {
	int op = abs(rand_column - 0) + abs(rand_row - 0);
	if (move_count <= op*1.5) {
		counter += 1;
	}
	if (j = 499) {
		assert(counter >= 0.75 * 30);
	}
}

//TestG creates a second state represantation that could be subsituted for the original state representation
void Grid::TestG() {
	if (c < rand_column && r < rand_row) {
		testGstate = 1;
	}
	else if (c == rand_column && r < rand_row) {
		testGstate = 2;
	}
	else if (c > rand_column && r < rand_row) {
		testGstate = 3;
	}
	else if (c < rand_column && r == rand_row) {
		testGstate = 4;
	}
	else if (c == rand_column && r == rand_row) {
		testGstate = 5;
	}
	else if (c > rand_column && r == rand_row) {
		testGstate = 6;
	}
	else if (c < rand_column && r > rand_row) {
		testGstate = 7;
	}
	else if (c == rand_column && r > rand_row) {
		testGstate = 8;
	}
	else if (c > rand_column && r > rand_row) {
		testGstate = 9;
	}
	else {
		c = r = 0;
	}

	int decide;
	if (testGstate == 1) {
		decide = rand() % 2;
		if (decide <= 1) {
			pawn = grid_arr[++r][c];
		}
		else {
			pawn = grid_arr[r][++c];
		}
		}
	else if (testGstate == 2) {
		decide = rand() % 2;
		if (decide <= 1.5) {
			pawn = grid_arr[++r][c];
		}
		else {
			pawn = grid_arr[r][++c];
		}
	}
	else if (testGstate == 3) {
		decide = rand() % 2;
		if (decide <= 1) {
			pawn = grid_arr[++r][c];
		}
		else {
			pawn = grid_arr[r][--c];
		}
	}
	else if (testGstate == 4) {
		decide = rand() % 2;
		if (decide <= 1.5) {
			pawn = grid_arr[r][++c];
		}
		else {
			pawn = grid_arr[++r][c];
		}
	}
	else if (testGstate == 5) {
			pawn = grid_arr[r][c];
	}
	else if (testGstate == 6) {
		decide = rand() % 2;
		if (decide <= 1.5) {
			pawn = grid_arr[r][--c];
		}
		else {
			pawn = grid_arr[++r][c];
		}
	}
	else if (testGstate == 7) {
		decide = rand() % 2;
		if (decide <= 1) {
			pawn = grid_arr[--r][c];
		}
		else {
			pawn = grid_arr[r][++c];
		}
	}
	else if (testGstate == 8) {
		decide = rand() % 2;
		if (decide <= 1.5) {
			pawn = grid_arr[--r][c];
		}
		else {
			pawn = grid_arr[r][++c];
		}
	}
	else if (testGstate == 9) {
		decide = rand() % 2;
		if (decide <= 1) {
			pawn = grid_arr[--r][c];
		}
		else {
			pawn = grid_arr[r][--c];
		}
	}
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
	int t[500]={0};
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
		myfile << t[i]/30 << endl;
	}

	myfile.close();
	return 0;
}
