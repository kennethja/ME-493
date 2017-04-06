//Kenneth Allison
//Project Gamma
//Recieved help from Scott Fourer during the review session 3/16/2017.

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
	int x_max = 100;
	int y_max = 100;
	int pop_size = 100;
	int num_cities = 25;
	int num_swaps = 3;
	int generations = 400;
	int half_pop = pop_size / 2; //creates half of the population for binary tournament
	double start_fit;
	double end_fit;
	vector<double> fitness_min;
	vector<double> fitness_max;
	vector<double> fitness_average;

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
	//struct sort_fitness;
	void calculate();

	void LR4(int,int);
	void LR5();
	void LR6();
	void LR7();
	void LR8();

	void MR1();
	void MR2(int);
	void MR3();
	void MR4();
	void MR5();

	void HR1();
	void HR2();
	void HR3();
	void HR4();
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
	/*
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
	*/
	calc_total_distance();
	fitness();

	//for (int p = 0; p < pop_size; p++) {
		//cout << "Path  " << p << "\t" << "Fitness  " << indiv.at(0).path.at(p).fitness << "\t" << endl << endl;
	//}

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
			indiv.at(0).path.at(p).fitness = 0;
			indiv.at(0).path.at(p).fitness = indiv.at(0).path.at(p).total_distance_traveled;
			MR2(p);
	}
}

void EA::MR2(int p) {
	assert(indiv.at(0).path.at(p).fitness = indiv.at(0).path.at(p).total_distance_traveled);
}

void EA::calc_total_distance() {
	//cout << indiv.at(0).path.size() << endl;
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
	if (indiv.at(0).path.at(first).fitness < indiv.at(0).path.at(second).fitness) {
		die = second;
	}
	else {
		die = first;
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
				LR4(city1, city2);
			}
			
			swap(p.town.at(city1), p.town.at(city2));
		}
		
	}
}

void EA::LR4(int city1, int city2) {
	int City1 = 1;
	int City2 = 2;
	assert(City1 != City2);
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
		random_shuffle(indiv.at(0).path.at(p).town.begin() + 1, indiv.at(0).path.at(p).town.end());
	}
}

//struct EA::sort_fitness {
	//inline bool operator() (const Policy& one, const Policy& two) {
		//return (one.fitness < two.fitness);
	//}
//};

void EA::calculate() {
	for (int p = 1; p < pop_size; p++) {
		if (indiv.at(0).path.at(p).fitness < indiv.at(0).path.at(0).fitness) {
			{
				swap(indiv.at(0).path.at(p), indiv.at(0).path.at(0));
			}
		}
	}
	//cout << indiv.at(0).path.at(0).fitness << endl;
	fitness_min.push_back(indiv.at(0).path.at(0).fitness);
	/*
	double mean_sum = 0.00;

	for (int p = 0; p < pop_size; p++) {
		mean_sum += indiv.at(0).path.at(p).fitness;
	}
	fitness_max.push_back(indiv.at(0).path.at(pop_size - 1).fitness);
	fitness_average.push_back(mean_sum / pop_size);
	*/
}

void EA::MR1() {
	init();
}


int main(){
srand(time(NULL)); //for rand()
EA Luna; //calling class EA as all evolutionary algorithim functions are within this class

Luna.MR1();

/*
if (remove("Maximum_Fitness.txt") != 0)
perror("Error deleting file");
else
puts("File successfully deleted");

if (remove("Average_Fitness.txt") != 0)
perror("Error deleting file");
else
puts("File successfully deleted");
*/
cout << endl;


for (int runs = 0; runs < 1; runs++) {
	cout << "Statistical Run" << "\t" << runs << endl;
	if (runs > 0) {
		Luna.fitness_min.clear();
		//Luna.fitness_max.clear();
		//Luna.fitness_average.clear();
		Luna.restart();
	}
	for (int i = 0; i < Luna.generations; i++) {
		if (i < Luna.generations - 1) {
			Luna.calc_total_distance(); //calculates total distance
			Luna.fitness(); //calcuates total fitness, which in this case, is total distance. Fitness can be something other than distance if so desired
	
			//Luna.sort_fitness();
			//sort(Luna.indiv.at(0).path.begin(), Luna.indiv.at(0).path.end(), Luna.sort_fitness());

			Luna.calculate();
			Luna.down_select();
			Luna.replicate();
		}
		else if (i == Luna.generations - 1) {
			Luna.calc_total_distance();
			Luna.fitness();
			Luna.calculate();
		}
}
	
	ofstream File1;
	//ofstream File2;
	//ofstream File3;

	File1.open("Minumum_Fitness.txt", ios_base::app);
	//File2.open("Maxumum_Fitness.txt", ios_base::app);
	//File3.open("Average_Fitness.txt", ios_base::app);
	
	for (int i = 0; i < Luna.generations; i++) {
		File1 << Luna.fitness_min.at(i) << "\t" << endl;
	}
	File1 << endl;
	File1.close();
	//for (int i = 0; i < Luna.generations; i++) {
		//File2 << Luna.fitness_max.at(i) << "\t" << endl;
	//}
	//File2.close();
	//for (int i = 0; i < Luna.generations; i++) {
		//File3 << Luna.fitness_average.at(i) << "\t" << endl;
	//}
	//File3.close();
}

return 0;
}

