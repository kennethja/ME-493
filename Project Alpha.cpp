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

#define PI_ (double) 3.1415
#define zero_to_one (double) rand()/RAND_MAX

vector<double> stored;

// class for the multi armed bandit
class MAB {
	double deviation;
	double average;
public:
	MAB();
	void setValues(double, double);
	void display();
	double gamble();
	void TestA(double);
};

MAB::MAB() {
	average = 1;
	deviation = 0;
}

void MAB::setValues(double m, double d) {
	average = m;
	deviation = d;
		
}

void MAB::display() {
	cout << "MEAN : " << average << "\tSTD : " << deviation << endl;
}

double MAB::gamble(){
	const double e = 0;
	double output = 0;
	double U1 = 0;
	double U2 = 0;
	while (U1 <= e) {
		U1 = zero_to_one;
		U2 = zero_to_one;
	}
	output = (sqrt((double)-2 * log(U1))*sin(2 * PI_*U2))*deviation + average;
	return output;

}

void MAB::TestA(double alpha) {  //test A pulls the bandit manually, so many iterations to test for u convergence.
	double avg = 0;
	for (int i = 0; i < 1000; i++) {
		avg = gamble() * alpha + avg*(1 - alpha);
	}
	assert(avg >= average - 1 && avg <= average + 1);
}



//learner class

class Senteince{
	public:
	vector<double> Q;
	int arm_chosen = 0;
	int good = 0;
	int x = 0;
	int tries = 1000;
	int response = 0;
	int options = 3;
	double alpha = 0.1;
	double epsilon = 0.1;
	double reward = 0;
	int **action_arr;


	void learner(bool);
	int decide();
	int greedy_action();
	int rand_action();
	void learned();
	void action(vector<MAB>);
	void TestB(int);
	void set_up_array();
};

void Senteince::learner(bool first) { //creates the vector for each option
	for (int i = 0; i < options; i++) {
		if (first) {
			Q.push_back((double)0);
		}
		else {
			Q[i] = 0;
		}
	}

}

int Senteince::decide() { //picks the multi-armed bandit, gets random value from that MAB = reward
		double x = (double)rand() / RAND_MAX;
		
		if (x < epsilon) {
			
			response = rand_action();
		}
		else {
			
			response = greedy_action();
		}
		
	return response;
}

int Senteince::greedy_action() { 
	double check = 0;
	good = 0;
	for (int i = 0; i < options; i++)
	{
		
		if (Q.at(i) > check)
		{
			
			good = i;
			check = Q.at(i);
		}
	}

	return good;
}

int Senteince::rand_action() {
	x = rand() % options;
	return x;
}

void Senteince::learned() {
	for (int i = 0; i < options; i++) {
		cout << Q.at(i) << "  ";
	}
	cout << endl;
	cout << greedy_action() << endl;

}

void Senteince::action(vector <MAB> list1) {
	int index;
	double R;
	for (int i = 0; i < tries; i++) {
		
		index = decide();
		action_arr[index][i]++;
		R = list1[index].gamble();
		stored.push_back(R);
		Q[index] = R * alpha + Q[index] * (1 - alpha);

	}
	learned();
}

void Senteince::TestB(int index) {
	for (int i = 0; i < options; i++) {
		//if (!(action_arr[index][1000] >= action_arr[i][1000])) {
			//cout << action_arr[index][1000] << " vs. " << action_arr[i][1000];
		//}
		assert(action_arr[index][1000] >= action_arr[i][1000]);
	}
	delete [] action_arr;
	}
	

void Senteince::set_up_array() {  //creates a double pointer
	action_arr = new int*[options]();
	for (int i = 0; i < options; i++) {
		action_arr[i] = new int[tries]();
	}
}

int main()
{
	srand(time(NULL));
	vector<MAB> List;
	vector<double> List_d;
	MAB human;
	double number;
	int n = 3;
	int input;
	bool first = true;
	for (int i = 0; i < n; i++) {
		List_d.push_back(zero_to_one * 10);
		human.setValues(List_d[List_d.size()-1], zero_to_one * 2);
		List.push_back(human);
	}

	double greater,greater1 = 0.0;
	int stored1 = 0;
	for (int i = 0; i < List_d.size(); i++) {
		if (List_d.at(stored1) < List_d.at(i)) {
			stored1 = i;
		}

	}


	//used to manually pull the multi-armed bandit, running this, proves the effectiveness of the gamble function
	/*
	while (1) {
		cout << ">> ";
		cin >> input;
		if (input < 1 || input > n) {
			break;
		}
		cout << List.at(input - 1).gamble() << endl;
	
	}
	*/

	//display list of means and standard deviations
	cout << "Values for Each Arm for Testing" << endl << endl;

	for (int i = 0; i < n; i++) {
		List.at(i).display();
	}
	cout << "\n" << "Action Learner Mean and Choice" << "\n" << endl;

	//call class Senteince, named Luna
	Senteince Luna;
	Luna.set_up_array();

	
	for (int i = 0; i < 30; i++) {
		Luna.learner(first);
		if (first) {
			first = false;
		}
		
		Luna.action(List);
	}

	//outputs the automated learner results for the best choice in bandit (slot machine)
	/*
	cout << "\n" << "stored learner results\n" << endl;
	for (int i = 0; i < stored.size(); i++)
	{
		
		cout << stored.at(i) << "\t";
	}
	cout << stored.size() << endl;
	*/

	//creates the vector which will store the information for the 30 iterations within the vector avg_reward
	vector<double> avg_reward;
	for (int i = 0; i < Luna.tries; i++) {
		double sum = 0;
		for (int j = 0; j < Luna.options; j++) {
			sum += stored.at(i + j * Luna.tries);
		}
		avg_reward.push_back(sum / Luna.options);
	}

	human.TestA(0.01);
	
	
	ofstream cfile;
	cfile.open("learning_file.csv");
	for (int i = 0; i < avg_reward.size(); i++) {
		cfile << avg_reward.at(i) << endl;
	}
	cfile.close();

	cfile.open("action_curve.txt");
	for (int i = 0; i < Luna.tries; i++) {
		cfile << i;
		for (int j = 0; j < Luna.options; j++) {
			cfile << '\t' << (double)Luna.action_arr[j][i]/30;
		}
		cfile << endl;
	}

	cfile.close();
	Luna.TestB(stored1);
    return 0;
}
