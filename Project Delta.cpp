// Project Delta.cpp : Defines the entry point for the console application.

//Kenneth Allison
//Project Delta
//4/6/2017

#include "stdafx.h";
#include "iostream";
#include "assert.h";
#include "string";
#include "random";
#include "time.h";
#include "vector";
#include "iomanip";
#include "fstream";
#include "cassert";
#include "cmath";
#include "ctime";
#include "limits";
#include "algorithm";
#include "LY_NN.h";

using namespace std;

class Policy {
public:
	vector<double> weights;
	double fitness;
	double x;
	double y;
	double theta;
	double omega;
	double u;
	double new_x;
	double new_y;
	double new_theta;
	double new_omega;
	double new_u;
	double goal_leftx;
	double goal_lefty;
	double goal_rightx;
	double goal_righty;
	double goal_topx;
	double goal_topy;
	double goal_botx;
	double goal_boty;
	double goal_range;
	double d2g;
	int inmap = 0;
};

class Agent {
public:
	vector<Policy> pawn;
};

class EA {

	friend class Simulator;

public:
	double pop_size = 10;
	double half_pop = pop_size / 2;
	double gen_max = 2;
	int num_inputs = 1;
	int num_hidden = 3;
	int num_outputs = 1;
	int num_weights = 3;
	int num_swaps = 3;
	double v = 3.0; //distance per second
	double dt = 0.2; //seconds
	double T = 5.0;
	double u_max = 15.0;
	double u_min = -15.0;
	double x_max = 100.0;
	double y_max = 100.0;
	double goal_angle = 0.0;
	int generations = 50;
	vector<Agent> indiv;
	vector<double> lgoal;
	vector<double> mgoal;
	vector<double> bgoal;

	void init(); //includes population initialization and weight initialization
	void set_Agent();
	void set_Goal(Policy* pP);
	void set_Goal_Area();
	void evaluate();
	void run_simulation();
	void fitness();
	int binary_tournament();
	void down_select();
	void mutate(Policy &M);
	void replicate();
	void restart();
	void calculate();
	
};

class Simulator {
public:
	double theta = 0;
	double w = 0;
	double u = 0;
	double vel = 3;
	double dt = 0.2;
	double T = 5.0;
	double max_time = 100.0;
	double max_thrust = 0.0;

	void Simulate(Policy *pP, double x_max, double y_max);
	void Agent_Setup(Policy *pP);
	void Display(Policy *pP, double time);
	void check_map(Policy *pP, double x_max, double y_max);
	int check_Goal(Policy* pP);
	void calc_newx(Policy *pP);
	void calc_newy(Policy *pP);
	void calc_newtheta(Policy *pP);
	void calc_newomega(Policy *pP);
	void output(Policy* pP, double time, int tstep);

};


void EA::init() {  //initializes the vectors containing the policies and wieghts for the neural network
	Agent A;
	indiv.push_back(A);
	for (int p = 0; p < pop_size; p++) {
		Policy P;
		indiv.at(0).pawn.push_back(P);
		for (int w = 0; w < num_weights;w++) {
			double a = 2 * ((double)rand() / RAND_MAX) + (-1);
			indiv.at(0).pawn.at(p).weights.push_back(a);
		}
		assert(indiv.at(0).pawn.at(p).weights.size() == num_weights);
	}
	assert(indiv.at(0).pawn.size() == pop_size);
}

void EA::set_Agent() { //not sure if starting at random locations

	for (int p = 0; p < pop_size; p++) {
		indiv.at(0).pawn.at(p).x = 0;//abs(((double)rand() / RAND_MAX)*x_max);
		indiv.at(0).pawn.at(p).y = 0;//abs(((double)rand() / RAND_MAX)*y_max);
		indiv.at(0).pawn.at(p).theta = 0;
		indiv.at(0).pawn.at(p).omega = 0;
		indiv.at(0).pawn.at(p).u = 0;
	}
}

void EA::set_Goal(Policy* pP) { //creating the goal as a line, not a point
	indiv.at(0).pawn.at(0).goal_leftx = 75;
	indiv.at(0).pawn.at(0).goal_lefty = 75;
	indiv.at(0).pawn.at(0).goal_rightx = 85;
	indiv.at(0).pawn.at(0).goal_righty = 75;
	indiv.at(0).pawn.at(0).goal_topx = 80;
	indiv.at(0).pawn.at(0).goal_topy = 80;
	indiv.at(0).pawn.at(0).goal_botx = 80;
	indiv.at(0).pawn.at(0).goal_boty = 70;
}

void EA::evaluate() {
	run_simulation();
	//fitness();
}

void EA::run_simulation() {
	for (int p = 0; p < pop_size; p++) {
		Simulator S;
		Policy *pP;
		pP = &indiv.at(0).pawn.at(p);
		S.Simulate(pP,y_max,x_max);
	}
}

void EA::fitness() { //using the distance from the goal as fitness
	int distance = 0;
	int distance_sum = 0;
	for (int p = 0; p < indiv.at(0).pawn.size(); p++) {
		indiv.at(0).pawn.at(p).fitness = 0;
	}
	for (int p = 0; p < indiv.at(0).pawn.size(); p++) {
		indiv.at(0).pawn.at(p).fitness = distance_sum;
	}
	cout << "Population Fitness" << endl;
	for (int p = 0; p < indiv.at(0).pawn.size(); p++) {
		cout << "Policy" << "\t" << indiv.at(0).pawn.at(p).fitness << "\t";
	}
	cout << endl;
}

int EA::binary_tournament() {  //should work for any type of fitness
	int first = rand() % indiv.at(0).pawn.size();
	int second = rand() % indiv.at(0).pawn.size();
	int die;
	while (first == second) {
		second = rand() % indiv.at(0).pawn.size();
	}
	if (indiv.at(0).pawn.at(first).fitness < indiv.at(0).pawn.at(second).fitness) {
		die = second;
		assert(die == second);
	}
	else {
		die = first;
		assert(die == first);
	}
	return die;
}

void EA::down_select() {
	for (int k = 0; k < half_pop; k++) {
		int die = 0;
		die = binary_tournament();
		indiv.at(0).pawn.erase(indiv.at(0).pawn.begin() + die);

	}
}

void EA::mutate(Policy &M) {
	Policy mutation;
	mutation = M;

	for (int i = 0; i < num_swaps; i++) {
		double j = rand() / RAND_MAX;
		if (j <= 0.5) {
			int city1 = rand() % indiv.at(0).pawn.size();
			if (city1 == 0) {
				city1 = rand() % indiv.at(0).pawn.size();
			}
			int city2 = rand() % indiv.at(0).pawn.size();
			if (city2 == 0 || city1 == city2) {
				city2 = rand() % indiv.at(0).pawn.size();
			}
			swap(M.pawn.at(city1), M.pawn.at(city2));
		}

	}
}

void EA::replicate() {
	for (int i = 0; i < half_pop; i++) {
		Policy p;
		int locate = rand() % indiv.at(0).pawn.size();
		p = indiv.at(0).pawn.at(locate);
		mutate(p);
		indiv.at(0).pawn.push_back(p);
	}
}

void Simulator::Simulate(Policy *pP,double x_max,double y_max) {
	double time = 0;
	int tstep = 0;
	pP->inmap = 0;
	while (time < max_time) {
		if (time == 0) {
			Agent_Setup(pP);
			//Calc_Goal_Distance(p);
			Display(pP,time);
			check_map(pP,x_max,y_max);
		}
		if (time > 0) {
			calc_newx(pP);
			calc_newy(pP);
			calc_newtheta(pP);
			calc_newomega(pP);
			Display(pP, time);
			check_map(pP, x_max, y_max);
		}
		if (pP->inmap == 1) {
			break;
		}
		tstep += 1;
		time += dt;
		//cout << "cp" << endl;
	}

}

void Simulator::Agent_Setup(Policy *pP) {
	pP->new_x = pP->x;
	pP->new_y = pP->y;
	pP->new_theta = pP->theta;
	pP->new_omega = pP->omega;
	pP->new_u = pP->u;
}

void Simulator::Display(Policy *pP,double time) {
	cout << "Current Time" << "\t" << time << endl;
	cout << "New X" << "\t" << pP->new_x << endl << "New Y" << "\t" << pP->new_y << endl << "New Theta" << "\t" << pP->new_theta << endl << "New Omega" << "\t" << pP->new_omega << endl << "New U" << "\t" << pP->new_u << endl;
}

void Simulator::check_map(Policy *pP,double x_max,double y_max) {
	//cout << x_max << endl;
	if (pP->new_x < 0 || pP->new_y < 0 || pP->new_x > x_max || pP->new_y > y_max) {
		cout << "Out of map space" << endl;
		pP->inmap = 1;
	}
}

void Simulator::calc_newx(Policy *pP) {
	pP->x = pP->new_x;
	pP->new_x = pP->x + vel*sin(theta)*dt;
}

void Simulator::calc_newy(Policy *pP) {
	pP->y = pP->new_y;
	pP->new_y = pP->y + vel*cos(theta)*dt;
}

void Simulator::calc_newtheta(Policy *pP) {
	pP->theta = pP->new_theta;
	pP->new_theta = pP->theta + pP->omega*dt;
}

void Simulator::calc_newomega(Policy *pP) {
	pP->omega = pP->new_omega;
	pP->u = pP->new_u;
	pP->new_omega = pP->omega + (pP->u - pP->omega)*dt/T;
}

int Simulator::check_Goal(Policy* pP) {
	EA E;
	if ((pP->x > 75) && (pP->x < 85) && (pP->y > 70) && (pP->y < 80)) {
		return 1;
	}
	else 
		return 0;
	
	}

int main()
{
	srand(time(NULL));
	Policy P;
	EA E;
	neural_network NN;
	Simulator S;
	NN.setup(2, 5, 1);

	NN.set_in_min_max(-5, 105); /// Y
	NN.set_in_min_max(-20, S.max_thrust);
	E.init();
	for (int gen = 0; gen < E.gen_max; gen++) {
		if (gen < E.gen_max - 1) {
			E.set_Agent();
			E.set_Goal();
			E.evaluate();
		}
	}

	ofstream File1;
	//ofstream File2;
	//ofstream File3;

	File1.open("Fitness.txt", ios_base::app);
	//File2.open("Maxumum_Fitness.txt", ios_base::app);
	//File3.open("Average_Fitness.txt", ios_base::app);

	for (int i = 0; i < E.generations; i++) {
		File1 << E.fitness.at(i) << "\t" << endl;
	}
	File1 << endl;
	File1.close();

    return 0;
}

