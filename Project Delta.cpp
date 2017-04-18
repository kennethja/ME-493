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
	double goal_x;
	double goal_2x;
	double goal_2y;
	double goal_y;
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
	double pop_size = 1;
	double gen_max = 2;
	int num_inputs = 1;
	int num_hidden = 3;
	int num_outputs = 1;
	int num_weights = 2;
	double v = 3.0; //distance per second
	double dt = 0.2; //seconds
	double T = 5.0;
	double u_max = 15.0;
	double u_min = -15.0;
	double x_max = 100.0;
	double y_max = 100.0;
	vector<Agent> indiv;

	void init();
	void set_Agent();
	void set_Goal();
	void set_Weights();
	void evaluate();
	void run_simulation();
	void get_fitness();
	
};

class Simulator {
public:
	double theta = 0;
	double w = 0;
	double u = 0;
	double vel = 3;
	double dt = 0.2;
	double T = 5.0;

	void Simulate(Policy *pP, double x_max, double y_max);
	void Agent_setup(Policy *pP);
	void Display(Policy *pP, double time);
	void check_map(Policy *pP, double x_max, double y_max);
	void calc_newx(Policy *pP);
	void calc_newy(Policy *pP);
	void calc_newtheta(Policy *pP);
	void calc_newomega(Policy *pP);
};


void EA::init() {  //initializes the vectors containing the policies and wieghts for the neural network
	Agent A;
	indiv.push_back(A);
	for (int p = 0; p < pop_size; p++) {
		Policy P;
		indiv.at(0).pawn.push_back(P);
		for (int w = 0; w < num_weights;w++) {
			indiv.at(0).pawn.at(p).weights.push_back(0);
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

void EA::set_Goal() { //creating the goal as a line, not a point
	indiv.at(0).pawn.at(0).goal_x = 75; //abs(((double)rand() / RAND_MAX)*x_max);
	indiv.at(0).pawn.at(0).goal_2x = indiv.at(0).pawn.at(0).goal_x - 5;

	indiv.at(0).pawn.at(0).goal_y = 75;//abs(((double)rand() / RAND_MAX)*y_max);
	indiv.at(0).pawn.at(0).goal_2y = indiv.at(0).pawn.at(0).goal_y - 5;
}

void EA::set_Weights() {
	for (int p = 0; p < pop_size; p++) {
		cout << "Policy" << "\t" << p << endl;
		for (int w = 0; w < num_weights; w++) {
			double r = 2 * ((double)rand() / RAND_MAX) + (-1);
			indiv.at(0).pawn.at(p).weights.at(w) = r;
		}
		for (int w = 0; w < num_weights; w++) {
			cout << "Weights" << "\t" << indiv.at(0).pawn.at(p).weights.at(w) << endl;
		}
	}
}

void EA::evaluate() {
	run_simulation();
	//get_fitness();
}

void EA::run_simulation() {
	for (int p = 0; p < pop_size; p++) {
		Simulator S;
		Policy *pP;
		pP = &indiv.at(0).pawn.at(p);
		S.Simulate(pP,y_max,x_max);
	}
}

void EA::get_fitness() {

}

void Simulator::Simulate(Policy *pP,double x_max,double y_max) {
	double time = 0;
	int tstep = 0;
	pP->inmap = 0;
	while (time < 100) {
		if (time == 0) {
			Agent_setup(pP);
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

void Simulator::Agent_setup(Policy *pP) {
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

int main()
{
	srand(time(NULL));
	EA E;
	E.init();
	E.set_Weights();
	for (int gen = 0; gen < E.gen_max; gen++) {
		if (gen < E.gen_max - 1) {
			E.set_Agent();
			E.set_Goal();
			E.evaluate();
		}
	}


    return 0;
}

