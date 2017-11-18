#pragma once
#include <iostream>  
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <string>
using namespace std;
class chromosome
{
	double fitness;
public:
	chromosome(chromosome*);
	chromosome(int, int, int, int, int);
	~chromosome();
	
	double getFitness();
	void calculateFitness();
	//void calculateFitness(cv::Vec3b);
	cv::Point getPosition();
	string gene;
	cv::Point position;
	cv::Vec3b color;
};

void crossover(chromosome * a, chromosome * b);
void mutation(chromosome * a);

string encode(cv::Vec3b);
cv::Vec3b decode(string);