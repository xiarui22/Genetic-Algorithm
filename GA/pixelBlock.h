#pragma once
#include <vector>
#include "chromosome.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"

class pixelBlock
{
	vector <chromosome *> currentGeneration;
	cv::Point position;  //the position of this small block in the big picture
	cv::Vec3b goalColor;
	cv::Vec3b color;
	cv::Mat *goal;
	cv::Mat *img;
	int populationNum;

public:
	pixelBlock();
	pixelBlock(cv::Mat *, cv::Point position);
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
	void showGeneration();
	bool isTerminate();
};

