// Project Gamma.cpp : Defines the entry point for the console application.
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
#include "algorithm";
using namespace std;

class City {
public:
	double x;
	double y;
	int location;
	vector<double> distance_to_cities;
};

class Policy {
public:
	vector<City> town;
	

};

class Agent {
public:
	vector<Policy> path;
};

class EA {
public:
	vector<Agent> indiv;
	int x_max = 10;
	int y_max = 10;
	int pop_size = 2;
	int num_cities = 4;

	void init();
	void create_city_locations();
	void get_distance_to_cities();
};

void EA::init() {
	Agent A;
	indiv.push_back(A);
	for (int p = 0; p < pop_size; p++) {
		Policy P;
		indiv.at(0).path.push_back(P);
		for (int c = 0; c < num_cities; c++) {
			City C;
			indiv.at(0).path.at(p).town.push_back(C);
		}
	}
	create_city_locations();
	get_distance_to_cities();
	for (int p = 0; p < pop_size; p++) {
		random_shuffle(indiv.at(0).path.at(p).town.begin() + 1, indiv.at(0).path.at(p).town.end());
	}

	for (int p = 0; p < pop_size; p++){
		cout << "path" << "\t" << p << endl;
		for (int c = 0; c < num_cities; c++) {
			cout << indiv.at(0).path.at(p).town.at(c).location << "\t" << endl;
		}
		cout << endl;
	}
	cout << endl;

	for (int p = 0; p < pop_size; p++) {
		cout << "Path " << "\t" << p << endl;
		for (int c = 0; c < num_cities; c++) {
			cout << "Location " << "\t" << indiv.at(0).path.at(p).town.at(c).location << endl;
			cout << "X " << "\t" << indiv.at(0).path.at(p).town.at(c).x << endl;
			cout << "Y " << "\t" << indiv.at(0).path.at(p).town.at(c).y << endl;
		}
		cout << endl;
	}
	cout << endl;

	for (int p = 0; p < pop_size; p++){
		cout << "PATH" << "\t" << p << endl;
		for (int c = 0; c < num_cities; c++) {
			cout << "Stop" << "\t" << c << "\t" << "city" << "\t" << indiv.at(0).path.at(p).town.at(c).location << endl;
			for (int cc = 0; cc < num_cities; cc++) {
				cout << "distance to city" << "\t" << cc << "\t" << indiv.at(0).path.at(p).town.at(c).distance_to_cities.at(cc) << endl;
			}
			cout << endl;
		}
	}
	cout << endl;
}

void EA::create_city_locations() {
	for (int c = 0; c < num_cities; c++) {
		indiv.at(0).path.at(0).town.at(c).location = c;
		indiv.at(0).path.at(0).town.at(c).x = abs(((double)rand() / RAND_MAX)*x_max);
		indiv.at(0).path.at(0).town.at(c).y = abs(((double)rand() / RAND_MAX)*y_max);
}
	for (int p = 1; p < pop_size; p++) {
		for (int c = 0; c < num_cities; c++) {
			indiv.at(0).path.at(p).town.at(c).location = indiv.at(0).path.at(0).town.at(c).location;
			indiv.at(0).path.at(p).town.at(c).x = indiv.at(0).path.at(0).town.at(c).x;
			indiv.at(0).path.at(p).town.at(c).y = indiv.at(0).path.at(0).town.at(c).y;
		}
	}
}

void EA::get_distance_to_cities() {
	for (int c = 0; c < num_cities; c++) {
		for (int cc = 0; cc < num_cities; cc++) {
			double dist = 0;
			double x = 0;
			double y = 0;
			x = (indiv.at(0).path.at(0).town.at(c).x - indiv.at(0).path.at(0).town.at(cc).x);
			y = (indiv.at(0).path.at(0).town.at(c).y - indiv.at(0).path.at(0).town.at(cc).y);
			dist = sqrt((x*x) + (y*y));
			indiv.at(0).path.at(0).town.at(c).distance_to_cities.push_back(dist);
	}
	}
	
	for (int p = 1; p < pop_size; p++) {
		for (int c = 0; c < num_cities; c++) {
			for (int cc = 0; cc < num_cities; cc++) {
				indiv.at(0).path.at(p).town.at(c).distance_to_cities.push_back(indiv.at(0).path.at(0).town.at(c).distance_to_cities.at(cc));
			}
		}
	}
	
}

int main(){
srand(time(NULL));
EA Luna;
Luna.init();


return 0;
}

