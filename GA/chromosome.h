#pragma once
#include <iostream>  
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace std;
class chromosome
{
	cv::Vec3b color;
	
	double fitness;
public:
	chromosome(int, int, int, int, int);
	~chromosome();
	string encode(cv::Vec3b);
	cv::Vec3b decode(string);
	double getFitness();
	void calculateFitness(cv::Mat *);
	cv::Point getPosition();
	string gene;
	cv::Point position;
};

void crossover(chromosome * a, chromosome * b);
void mutation(chromosome * a);

