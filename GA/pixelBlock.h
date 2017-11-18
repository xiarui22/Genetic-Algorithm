#pragma once
#include <vector>
#include "chromosome.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"

class pixelBlock
{
	vector <chromosome *> currentGeneration;
	cv::Point position;
	cv::Vec3b goalColor;
	cv::Vec3b color;
	int populationNum;

public:
	pixelBlock(cv::Point position, cv::Vec3b color, int);
	~pixelBlock();
	void init();
	void setColor(cv::Vec3b);
	cv::Vec3b getColor();
	cv::Point getPosition();
	void breed();

	void FitnessCalculation();
	void Crossover();
	void Mutation();
	void survivorSelection();

};

