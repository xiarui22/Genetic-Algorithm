
#include <iostream>  

#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;
using namespace std;


int main()
{
	Mat img = imread("kingboo.jpg");
	if (img.empty())
	{
		cout << "error";
		return -1;
	}
	imshow("Lena", img);
	waitKey();

	return 0;

}