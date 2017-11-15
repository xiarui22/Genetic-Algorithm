
#include <stdlib.h>
#include <algorithm> 

#include "chromosome.h"


using namespace cv;
using namespace std;

#define RC 0.6  //RC part of generation will go to mate and produce offspring
#define PC 0.8  //probability to crossover
#define RM 0.2  //RM part of generation will mutate
#define PM 0.3  //probability to mutate


Mat *img = new Mat();
vector <chromosome *> currentGeneration;

struct by_fitness {
	bool operator()(chromosome* &a, chromosome* &b) {
		return a->getFitness() > b->getFitness();
	}
};

//generate the first generation
void init(Mat *goal)     
{
	img = new Mat(goal->size(), goal->type());

	for (int y = 0; y < img->rows; y++)
	{
		for (int x = 0; x < img->cols; x++)
		{
			Vec3b color = goal->at<Vec3b>(x, y);
			color[0] = rand() % 256;
			color[1] = rand() % 256;
			color[2] = rand() % 256;
			chromosome * oneGene = new chromosome(color[0], color[1], color[2], x, y);
			currentGeneration.push_back(oneGene);
			img->at<Vec3b>(x, y) = color;
		}	
	}
	imshow("population", *img);
	waitKey(1000);
}

void FitnessCalculation(Mat * goal) 
{
	int size = currentGeneration.size();
	for (int i = 0; i < size; i++) {
		currentGeneration[i]->calculateFitness(goal);
	}
	sort(currentGeneration.begin() , currentGeneration.end(), by_fitness());
	cout << "fitness" << endl;
}

void Crossover() 
{
	for (int i = 0; i < currentGeneration.size() * RC; i += 2) {
		int a = rand() % 10 + 1;
		if(float(a/10) > PC) crossover(currentGeneration[i], currentGeneration[i + 1]);
	}
	cout << "crossover" << endl;
}

void Mutation() {
	int numOfmutation = 0;
	while (numOfmutation < currentGeneration.size() * RM) {
		int i = rand() % currentGeneration.size();
		int a = rand() % 10 + 1;
		if (float(a / 10) > PM) mutation(currentGeneration[i]);
		numOfmutation++;
	}
	cout << "mutation" << endl;
}


void ShowGeneration(vector <chromosome *> generation) {
	for (int y = 0; y < img->rows; y++)
	{
		for (int x = 0; x < img->cols; x++)
		{
			Vec3b color;
			Point a = Point(x, y);
			auto it = find_if(generation.begin(), generation.end(), [=](chromosome* f) {
				return (f->position == a);
			});
			
			color = (*it)->decode((*it)->gene);
			/*color[0] = 255;
			color[1] = 255;
			color[2] = 255;*/
			img->at<Vec3b>(x, y) = color;
		}
	}
	cout << "show" << endl;
	imshow("population", *img);
	waitKey(1000);
}

bool isTerminate(Mat *goal, Mat *img) {
	for (int y = 0; y < img->rows; y++)
	{
		for (int x = 0; x < img->cols; x++)
		{
			if (img->at<Vec3b>(x, y) != goal->at<Vec3b>(x, y))  return false;
		}
	}
	return true;
}

int main()
{
	Mat *goal = new Mat();
	*goal = imread("kingboos.jpg");
	if (goal->empty())
	{
		cout << "error";
		return -1;
	}
	imshow("King boo", *goal);
	init(goal);
	int i = 0;
	while (!isTerminate(goal, img)) {
		FitnessCalculation(goal);
		Crossover();
		Mutation();
		ShowGeneration(currentGeneration);
		i++;
		cout << i << endl;
	}
    waitKey();

	return 0;

}

