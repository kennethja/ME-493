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
	void TestA(vector<MAB>,int);
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

void MAB::TestA(vector<MAB> temp_list, int n) {  //test A pulls the bandit manually, so many iterations to test for u convergence.
	int count = 100;
	int bandit = 1;
	double drag = 0.0;
	double test_avg = 0.0;
	double ftest_avg = 0.0;
	vector<double> verify;
	for (int i = 0; i < count; i++) {
		drag = temp_list.at(bandit).gamble();
		cout << drag << endl;
		verify.push_back(drag);
	}
	for (int i = 0; i < count; i++) {
		test_avg = test_avg + verify.at(i);
	}
	ftest_avg = test_avg / count;
	cout << "\n" << ftest_avg;
	assert((ftest_avg*1.1 >= temp_list.at(bandit)) && (ftest_avg*0.9 <= temp_list.at(bandit)));
}



//learner class

class Senteince{
	public:
	vector<double> Q;
	int good = 0;

	int tries = 120;
	int response = 0;
	int options = 3;
	double alpha = 0.1;
	double epsilon = 0.1;
	double reward = 0;


	void learner();
	int decide();
	int greedy_action();
	int rand_action();
	void learned();
	void action(vector<MAB>);
	void TestB(vector<MAB>,int);
};

void Senteince::learner() { //creates the vector for each option
	for (int i = 0; i < options; i++) {
		Q.push_back((double)0);
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
	int x = rand() % options;
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
		R = list1[index].gamble();
		stored.push_back(R);
		Q[index] = R * alpha + Q[index] * (1 - alpha);

	}
	learned();
}

void Senteince::TestB(vector<MAB> temp_list, int n) {
	action(temp_list);
	for (int i = 0; i < 6; i++) {
		//assert(temp_list(good) >= temp_list(i));
	}
	
}

int main()
{
	srand(time(NULL));
	vector<MAB> List;
	MAB human;
	double number;
	int n = 3;
	int input;
	for (int i = 0; i < n; i++) {
		human.setValues(zero_to_one * 10, zero_to_one * 2);
		List.push_back(human);
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

	//run through 30 iterations of the action learner to create a learning curve out of the data
	for (int i = 0; i < 30; i++) {
		Luna.learner();
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

	human.TestA(List,n);
	Luna.TestB(List,n);

	ofstream cfile;
	cfile.open("card_file.csv", ios_base::app);
	for (int i = 0; i < avg_reward.size(); i++) {
		cfile << avg_reward.at(i) << endl;
}

    return 0;
}

