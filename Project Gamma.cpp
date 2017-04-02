//Kenneth Allison
//Project Gamma
//Recieved help from Scott Fourer during the review session 3/16/2017

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
	double fitness;
	double total_distance_traveled;
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
	int pop_size = 4;
	int num_cities = 10;
	int num_swaps = 2;
	int half_pop = pop_size / 2; //creates half of the population for binary tournament

	void init();
	void create_city_locations();
	void calc_distance_to_cities();
    void calc_total_distance();
	void fitness();
	int binary_tournament();
	void down_select();
	void mutate(Policy &M);
	void replicate();
	void restart();
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
	calc_distance_to_cities();
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

	calc_total_distance();
	fitness();

	for (int p = 0; p < pop_size; p++) {
		cout << "Path  " << p << "\t" << "Fitness  " << indiv.at(0).path.at(p).fitness << "\t" << endl << endl;
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

void EA::calc_distance_to_cities() {
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

void EA::fitness() {
		for (int p = 0; p < pop_size; p++){
			indiv.at(0).path.at(0).fitness = 0;
			indiv.at(0).path.at(p).fitness = indiv.at(0).path.at(p).total_distance_traveled;
	}
}

void EA::calc_total_distance() {
	for (int p = 0; p < pop_size; p++) {
		double hold = 0;
		for (int c = 0; c < indiv.at(0).path.at(p).town.size() - 1; c++) {
			hold += indiv.at(0).path.at(p).town.at(c).distance_to_cities.at(indiv.at(0).path.at(p).town.at(c + 1).location);
		}
		indiv.at(0).path.at(p).total_distance_traveled = hold;
	}
}

int EA::binary_tournament() {
	//picks two random paths and compares the fitness of both. Looks for the better fitness.
	int first = rand() % indiv.at(0).path.size();
	int second = rand() % indiv.at(0).path.size();
	int die;
	while (first == second) {
		second = rand() % indiv.at(0).path.size();
	}
	if (indiv.at(0).path.at(first).fitness < indiv.at(0).path.at(first).fitness) {
		die = first;
	}
	else {
		die = second;
	}
	return die;
}

void EA::down_select() {
	for (int k = 0; k < half_pop; k++) {
		int die = 0;
		die = binary_tournament();
		indiv.at(0).path.erase(indiv.at(0).path.begin() + die);

	}
}

void EA::mutate(Policy &p) {
	Policy mutation;
	mutation = p;

	for (int i = 0; i < num_swaps; i++) {
		double j = rand() / RAND_MAX;
		if (j <= 0.5) {
			int city1 = rand() % indiv.at(0).path.at(0).town.size();
			if (city1 == 0) {
				city1 = rand() % indiv.at(0).path.at(0).town.size();
			}
			int city2 = rand() % indiv.at(0).path.at(0).town.size();
			if (city2 == 0 || city1 == city2) {
				city2 = rand() % indiv.at(0).path.at(0).town.size();
			}

			swap(p.town.at(city1), p.town.at(city2));
		}
	}
}

void EA::replicate() {

	for (int i = 0; i < half_pop; i++) {
		Policy p;
		int locate = rand() % indiv.at(0).path.size();
		p = indiv.at(0).path.at(locate);
		mutate(p);
		indiv.at(0).path.push_back(p);
	}
}

void EA::restart() {
	for (int p = 0; p < pop_size; p++) {
		random_shuffle(indiv.at(0).path.at(p).town.begin() + 1, indiv.at(0).path.at(0).town.end());
	}
}

int main(){
srand(time(NULL));
EA Luna;
Luna.init();

return 0;
}

