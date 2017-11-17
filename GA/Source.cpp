#include <stdlib.h>
#include <algorithm> 
#include <stdio.h>
#include <time.h>
#include <thread>
#include "chromosome.h"



using namespace cv;
using namespace std;

#define RC 0.6  //RC part of generation will go to mate and produce offspring
#define PC 0.8  //probability to crossover
#define RM 0.2  //RM part of generation will mutate
#define PM 0.3  //probability to mutate

Mat *img = new Mat();
vector <chromosome *> currentGeneration[9];
vector<thread> threads;

int populationNum = 0;
void init(Mat *goal, int p)     
{
    srand(time(NULL));
	populationNum = (goal->rows)*(goal->cols);
	
		for (int y = 0; y < goal->rows; y++)
		{
			for (int x = 0; x < goal->cols; x++)
			{
				Vec3b color = goal->at<Vec3b>(x, y);

				int b = rand() % 256;
				int g = rand() % 256;
				int r = rand() % 256;
				color[0] = b;
				color[1] = g;
				color[2] = r;
				chromosome * oneGene = new chromosome(color[0], color[1], color[2], x, y);
				currentGeneration[p].push_back(oneGene);
				img->at<Vec3b>(x, y+p/3*3) = color;
			}
		}
	
	waitKey(1000);
}


struct by_fitness {
	bool operator()(chromosome* &a, chromosome* &b) {
		return a->getFitness() > b->getFitness();
	}
};

//generate the first generation


void FitnessCalculation(Mat * goal, int p) 
{
	int size = currentGeneration[p].size();
	for (int i = 0; i < size; i++) {
		currentGeneration[p][i]->calculateFitness();
	}
	sort(currentGeneration[p].begin() , currentGeneration[p].end(), by_fitness());
	cout << "fitness" << endl;
	for (int a = 0; a < currentGeneration[p].size(); a++) {
		cout << currentGeneration[p][a]->getFitness() <<" "<<currentGeneration[p][a]->color<< currentGeneration[p][a]->gene << endl;
	}
}

void Crossover(int p) 
{
	for (int i = 0; i < (populationNum)/2; i += 1) {
		int a = rand() % 10 + 1;
		if (float(a / 10) < PC) {
			chromosome * child0 = new chromosome(currentGeneration[p][i]);
	        chromosome * child1 = new chromosome(currentGeneration[p][i+ (populationNum) / 2]);
			crossover(child0, child1);
			currentGeneration[p].push_back(child0);
			currentGeneration[p].push_back(child1);
			cout << "crossover happens" << endl;
		}
	}
	cout << "crossover" << endl;
		for (int a = 0; a < currentGeneration[p].size(); a++) {
			cout << currentGeneration[p][a]->getFitness() << " " << currentGeneration[p][a]->color << currentGeneration[p][a]->gene << endl;
		}
	
}

void Mutation(int p) {
	//int numOfmutation = 0;
	//while (numOfmutation < currentGeneration.size() * RM) {
		int i = rand() % currentGeneration[p].size();
		int a = rand() % 10 + 1;
		if (float(a / 10) < PM) {
			mutation(currentGeneration[p][i]);
			cout << "mutation happens" <<i<< endl;
		}
	//	numOfmutation++;
	//}
	cout << "mutation" << endl;
	for (int a = 0; a < currentGeneration[p].size(); a++) {
		cout << currentGeneration[p][a]->getFitness() << " " << currentGeneration[p][a]->color <<" "<<currentGeneration[p][a]->gene<< endl;
	}
}

void survivorSelection(int p) {
	int size = currentGeneration[p].size();
	for (int i = 0; i < size; i++) {
		currentGeneration[p][i]->color = decode(currentGeneration[p][i]->gene);
		currentGeneration[p][i]->calculateFitness();
	}
	sort(currentGeneration[p].begin(), currentGeneration[p].end(), by_fitness());
	for (int i = currentGeneration[p].size()-1; i >= populationNum; i--) {
		currentGeneration[p].erase(currentGeneration[p].begin() + i);
	}
	cout << "survivorSelection" << endl;
	for (int a = 0; a < currentGeneration[p].size(); a++) {
		cout << currentGeneration[p][a]->getFitness() << " " << currentGeneration[p][a]->color << " "<<currentGeneration[p][a]->gene << endl;
	}
}

void ShowGeneration(int p) {
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
			color = currentGeneration[p][0]->color;
			/*color[0] = 255;
			color[1] = 255;
			color[2] = 255;*/
			img->at<Vec3b>(x, y) = color;
		}
	}
	cout << "show" << endl;
	for (int a = 0; a < currentGeneration[p].size(); a++) {
		cout << currentGeneration[p][a]->getFitness() << " " << currentGeneration[p][a]->color << " " << currentGeneration[p][a]->gene << endl;
	}
	//imshow("population", *img);
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

struct threadParameter {
	Mat *goal;
	int population;

};

void breed(threadParameter &f) {
	init(f.goal, f.population);

	/*int i = 0;
	while (i<10) {
		FitnessCalculation(f.goal, f.population);
		Crossover(f.population);
		Mutation(f.population);
		survivorSelection(f.population);
		ShowGeneration(f.population);
		i++;
		cout << i << endl;
	}*/
	
	waitKey();
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

	img = new Mat(goal->size(), goal->type());
	imshow("population", *img);

	threadParameter tp[9];
	Mat * goals[9];
	Rect roi[9];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
              roi[i*3+j] = Rect(j*3, i * 3, 3, 3);
			  goals[i * 3 + j] = new Mat(*goal, roi[i]);
		}   
	}
	
	
	for (int i = 0; i < 9; ++i) {
		tp[i].goal = goals[i];
		tp[i].population = i;
		threads.push_back(thread(breed, tp[i]));
	}


	// loop again to join the threads
	for (auto& t : threads)
		t.join();

	imshow("population", *img);
	return 0;
}

