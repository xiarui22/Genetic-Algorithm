#include "chromosome.h"
//#include <algorithm>
#include <stdio.h>
#include <time.h>

using namespace cv;

string convertDecimaltoBinary(int n) {
	string r;
	while (n > 0) {
		int digit = n % 2;
		n /= 2;
		r = (char)(digit + '0') + r;
	}
	while (r.length() < 8) {
		r = '0' + r;
	}
	//reverse(r.begin(), r.end());
	return r;
}

chromosome::chromosome(int b, int g, int r, int x, int y)
{
	color[0] = b;
	color[1] = g;
	color[2] = r;
	position.x = x;
	position.y = y;
	gene = encode(color);
}

chromosome::chromosome(chromosome* a) {
	color = a->color;
	position = a->position;
	gene = a->gene;
	fitness = a->fitness;
}

chromosome::~chromosome()
{
}

string encode(Vec3b c) {
	int colorb = c[0], colorg = c[1], colorr = c[2];
	string geneb = convertDecimaltoBinary(colorb);
	string geneg = convertDecimaltoBinary(colorg);
	string gener = convertDecimaltoBinary(colorr);
	string g = geneb + geneg + gener;
	return g;
}

cv::Vec3b decode(string g) {
	int colorb = 0, colorg = 0, colorr = 0;
	for (int i = 0; i < 8; i++) {
		colorb += (g[i] -'0') * pow(2, 7-i);
	}
	for (int i = 8; i < 16; i++) {
		colorg += (g[i] - '0') * pow(2, 7-(i-8));
	}
	for (int i = 16; i < 24; i++) {
		colorr += (g[i] - '0') * pow(2, 7-(i-16));
	}
	Vec3b c;
	c[0] = colorb;
	c[1] = colorg;
	c[2] = colorr;
	return c;
}

double chromosome::getFitness() {
	return fitness;
}

//void chromosome::calculateFitness() {
void chromosome::calculateFitness(cv::Vec3b c) {
	double fit = 0;
	//Vec3b goalColor = goal->at<Vec3b>(position);
	//Vec3b goalColor = Vec3b(0, 0, 255);
	Vec3b goalColor = c;
	/*string goalGene = encode(goalColor);
	for (int i = 0; i < goalGene.length(); i++) {
		if (gene[i] == goalGene[i]) fit++;
	}*/
	fit = abs((double)color[0] - (double)goalColor[0]) + abs((double)color[1] - (double)goalColor[1]) + abs((double)color[2] - (double)goalColor[2]);
	fitness = -fit;
}

cv::Point chromosome::getPosition() {
	return position;
}

void crossover(chromosome * a, chromosome * b) {
	int i = 0;
	srand(time(NULL));
	while (i < 24) {
		int begin = rand() % 8+i;
		int end = rand() % 8+i;
		if (begin > end) {
			int temp = begin;
			begin = end;
			end = temp;
		}
		if (begin == end && begin != 0) begin -= 1;
		if (begin == end && begin == 0) end += 1;

		string aSegment = a->gene.substr(begin, end - begin + 1);
		string bSegment = b->gene.substr(begin, end - begin + 1);

		a->gene.replace(begin, end - begin + 1, bSegment);
		b->gene.replace(begin, end - begin + 1, aSegment);
		i += 8;
	}

}

void mutation(chromosome * a) {
	srand(time(NULL));
	int mutationP = rand() % 24;
	if (a->gene[mutationP] == 0) a->gene[mutationP] = 1+'0';
	else a->gene[mutationP] = 0 + '0';
}

