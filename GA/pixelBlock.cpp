#include "pixelBlock.h"

#define RC 0.6  //RC part of generation will go to mate and produce offspring
#define PC 0.8  //probability to crossover
#define RM 0.2  //RM part of generation will mutate
#define PM 0.3  //probability to mutate

struct by_fitness {
	bool operator()(chromosome* &a, chromosome* &b) {
		return a->getFitness() > b->getFitness();
	}
};

pixelBlock::pixelBlock(cv::Point _position, cv::Vec3b _color, int _population)
{
	position = _position;
	color = _color;
	populationNum = _population;
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

void pixelBlock::breed() {
	int i = 0;
	//while (!isTerminate(goal, img)) {
	while (i<10) {
		FitnessCalculation();
		Crossover();
		Mutation();
		survivorSelection();
//		ShowGeneration();
		i++;
		cout << i << endl;
	}
}

void pixelBlock::FitnessCalculation() {
	int size = currentGeneration.size();
	for (int i = 0; i < size; i++) {
		currentGeneration[i]->calculateFitness();
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
	//	numOfmutation++;
	//}
	cout << "mutation" << endl;
}

void pixelBlock::survivorSelection() {
	int size = currentGeneration.size();
	for (int i = 0; i < size; i++) {
		currentGeneration[i]->color = decode(currentGeneration[i]->gene);
		currentGeneration[i]->calculateFitness();
	}
	sort(currentGeneration.begin(), currentGeneration.end(), by_fitness());
	for (int i = currentGeneration.size() - 1; i >= populationNum; i--) {
		currentGeneration.erase(currentGeneration.begin() + i);
	}
	cout << "survivorSelection" << endl;
}

