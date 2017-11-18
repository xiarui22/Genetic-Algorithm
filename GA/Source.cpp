
#include <stdlib.h>
#include <algorithm> 
#include <stdio.h>
#include <time.h>
#include "pixelBlock.h"


using namespace cv;
using namespace std;

Mat *img;

void ShowGeneration(pixelBlock p) {
	Point a = p.getPosition();

			for (int y = a.y; y < 3+ a.y; y++)
			{
				for (int x = a.x; x < 3+ a.x; x++)
				{
					img->at<Vec3b>(y, x) = p.getColor();
				}
			}

	cout << "show" << endl;
	imshow("population0", *img);
	waitKey(100);
}

bool isTerminate(Mat *goal, Mat *img) {
	for (int y = 0; y < img->rows/3; y++)
	{
		for (int x = 0; x < img->cols/3; x++)
		{
			if (img->at<Vec3b>(x, y) != goal->at<Vec3b>(x, y))  return false;
		}
	}
	return true;
}



int main()
{
	Mat *goal = new Mat();

	*goal = imread("kingboo0.jpg");
	if (goal->empty())
	{
		cout << "error";
		return -1;
	}
	imshow("King boo", *goal);

	img = new Mat(goal->size(), goal->type());

	int groupNum = goal->rows*goal->cols / 9;
	Mat ** partGoal = new Mat*[groupNum];
	pixelBlock* p = new pixelBlock[groupNum];
	for (int y = 0; y < goal->rows; y+=3)
	{
		for (int x = 0; x < goal->cols; x+=3)
		{
			Rect rec = Rect(x, y, 3, 3);
			int index = y / 3 * sqrt(groupNum) + x / 3;
			partGoal[index] = new Mat(*goal, rec);
			p[index] = pixelBlock(partGoal[index], Point(x, y));
			
		}
	}
	int i = 0;
	while (i < 30) {
		for (int a = 0; a < groupNum; a++) {
			p[a].breed();
			ShowGeneration(p[a]);
		}
		i++;
	}
	
	waitKey();
	return 0;

}

