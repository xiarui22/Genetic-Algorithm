#include <stdio.h>
#include <stdlib.h>
#include <algorithm> 
#include <time.h>
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
	imshow("population", *img);
	waitKey(1000);
}

void FitnessCalculation(Mat * goal) 
{
	int size = currentGeneration.size();
	for (int i = 0; i < size; i++) {
		currentGeneration[i]->calculateFitness();
	}
	sort(currentGeneration.begin() , currentGeneration.end(), by_fitness());
	cout << "fitness" << endl;
	for (int a = 0; a < currentGeneration.size(); a++) {
		cout << currentGeneration[a]->getFitness() <<" "<<currentGeneration[a]->color<< currentGeneration[a]->gene << endl;
	}
}

void Crossover() 
{
	//for (int i = 0; i < currentGeneration.size() * RC; i += 1) {
	for (int i = 1; i < currentGeneration.size()/2; i += 1) {
		int a = rand() % 10 + 1;
		if (float(a / 10) < PC) {
			crossover(currentGeneration[i], currentGeneration[i + 4]);
			cout << "crossover happens" << endl;
		}
	}
	cout << "crossover" << endl;
		for (int a = 0; a < currentGeneration.size(); a++) {
			cout << currentGeneration[a]->getFitness() << " " << currentGeneration[a]->color << currentGeneration[a]->gene << endl;
		}
	
}

void Mutation() {
	//int numOfmutation = 0;
	//while (numOfmutation < currentGeneration.size() * RM) {
		int i = rand() % currentGeneration.size();
		int a = rand() % 10 + 1;
		if (float(a / 10) < PM) {
			mutation(currentGeneration[i]);
			cout << "mutation happens" <<i<< endl;
		}
	//	numOfmutation++;
	//}
	cout << "mutation" << endl;
	for (int a = 0; a < currentGeneration.size(); a++) {
		cout << currentGeneration[a]->getFitness() << " " << currentGeneration[a]->color <<" "<<currentGeneration[a]->gene<< endl;
	}
}

void survivorSelection() {
	int size = currentGeneration.size();
	for (int i = 0; i < size; i++) {
		currentGeneration[i]->color = decode(currentGeneration[i]->gene);
		currentGeneration[i]->calculateFitness();
	}
	sort(currentGeneration.begin(), currentGeneration.end(), by_fitness());
	int i = currentGeneration.size()-1;
	currentGeneration[i] = currentGeneration[0];
	//currentGeneration[i - 1] = currentGeneration[0];
	cout << "survivorSelection" << endl;
	for (int a = 0; a < currentGeneration.size(); a++) {
		cout << currentGeneration[a]->getFitness() << " " << currentGeneration[a]->color << " "<<currentGeneration[a]->gene << endl;
	}
}

void ShowGeneration() {
	for (int y = 0; y < img->rows; y++)
	{
		for (int x = 0; x < img->cols; x++)
		{
			Vec3b color;
			Point a = Point(x, y);
			/*auto it = find_if(generation.begin(), generation.end(), [=](chromosome* f) {
				return (f->position == a);
			});
			
			color = (*it)->decode((*it)->gene);*/
			//color = generation[0]->decode(generation[0]->gene);
			//color = decode(currentGeneration[0]->gene);
			color = currentGeneration[0]->color;
			/*color[0] = 255;
			color[1] = 255;
			color[2] = 255;*/
			img->at<Vec3b>(x, y) = color;
		}
	}
	cout << "show" << endl;
	for (int a = 0; a < currentGeneration.size(); a++) {
		cout << currentGeneration[a]->getFitness() << " " << currentGeneration[a]->color << " " << currentGeneration[a]->gene << endl;
	}
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
		survivorSelection();
		ShowGeneration();
		i++;
		cout << i << endl;
	}
    waitKey();

	return 0;

}

