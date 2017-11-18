#include "pixelBlock.h"
#include <time.h>

using namespace cv;
#define RC 0.6  //RC part of generation will go to mate and produce offspring
#define PC 0.8  //probability to crossover
#define RM 0.2  //RM part of generation will mutate
#define PM 0.3  //probability to mutate

struct by_fitness {
	bool operator()(chromosome* &a, chromosome* &b) {
		return a->getFitness() > b->getFitness();
	}
};

pixelBlock::pixelBlock() {

}

pixelBlock::pixelBlock(Mat *_goal, cv::Point _position)
{
	goal = _goal;
	position = _position;
	populationNum = goal->rows*goal->cols;
	goalColor = goal->at<Vec3b>(Point(0, 0));
	init();
}


pixelBlock::~pixelBlock()
{
}

void pixelBlock::setColor(cv::Vec3b c) {
	color = c;
}
cv::Vec3b pixelBlock::getColor() {
	return color;
}
cv::Point pixelBlock::getPosition() {
	return position;
}

void pixelBlock::init()
{
	img = new Mat(goal->size(), goal->type());
	srand(time(NULL));
	
	for (int y = 0; y < img->rows; y++)
	{
		for (int x = 0; x < img->cols; x++)
		{
			Vec3b color = goal->at<Vec3b>(x, y);

			int b = rand() % 256;
			int g = rand() % 256;
			int r = rand() % 256;
			color[0] = b;
			color[1] = g;
			color[2] = r;
			chromosome * oneGene = new chromosome(color[0], color[1], color[2], x, y);
			currentGeneration.push_back(oneGene);
			img->at<Vec3b>(x, y) = color;
		}
	}
	//imshow("population", *img);
	//waitKey(1000);
}

bool pixelBlock::isTerminate() {
	if (color == goalColor) return true;
	else return false;
}

void pixelBlock::breed() {
	//int i = 0;
	
	//while (!isTerminate(goal, img)) {
	if (!isTerminate()) {
		FitnessCalculation();
		Crossover();
		Mutation();
		survivorSelection();
		showGeneration();
	//	i++;
	//	cout << i << endl;
	}
}

void pixelBlock::FitnessCalculation() {
	int size = currentGeneration.size();
	for (int i = 0; i < size; i++) {
		currentGeneration[i]->calculateFitness(goalColor);
	}
	sort(currentGeneration.begin(), currentGeneration.end(), by_fitness());
	cout << "fitness" << endl;
}

void pixelBlock::Crossover() {
	for (int i = 0; i < (populationNum) / 2; i += 1) {
		int a = rand() % 10 + 1;
		if (float(a / 10) < PC) {
			chromosome * child0 = new chromosome(currentGeneration[i]);
			chromosome * child1 = new chromosome(currentGeneration[i + (populationNum) / 2]);
			crossover(child0, child1);
			currentGeneration.push_back(child0);
			currentGeneration.push_back(child1);
			cout << "crossover happens" << endl;
		}
	}
	cout << "crossover" << endl;
}

void pixelBlock::Mutation() {
	int i = rand() % currentGeneration.size();
	int a = rand() % 10 + 1;
	if (float(a / 10) < PM) {
		mutation(currentGeneration[i]);
		cout << "mutation happens" << i << endl;
	}
	cout << "mutation" << endl;
}

void pixelBlock::survivorSelection() {
	int size = currentGeneration.size();
	for (int i = 0; i < size; i++) {
		currentGeneration[i]->color = decode(currentGeneration[i]->gene);
		currentGeneration[i]->calculateFitness(goalColor);
	}
	sort(currentGeneration.begin(), currentGeneration.end(), by_fitness());
	for (int i = currentGeneration.size() - 1; i >= populationNum; i--) {
		currentGeneration.erase(currentGeneration.begin() + i);
	}
	cout << "survivorSelection" << endl;
	color = currentGeneration[0]->color;
	cout << color;
}

void pixelBlock::showGeneration() {
	for (int y = 0; y < img->rows; y++)
	{
		for (int x = 0; x < img->cols; x++)
		{
			img->at<Vec3b>(x, y) = color;
		}
	}
	imshow("population", *img);
	//waitKey(1000);
}