#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	cout << "Hello" << endl;
	Mat img = imread("lena30.jpg");
	imshow("", img);
	waitKey(0);
	return 0;
}
