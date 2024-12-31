#ifndef gaussian_pyramid_H
#define gaussian_pyramid_H

#include<opencv2/opencv.hpp>
#include<vector>

using namespace cv;
using namespace std;

vector<Mat>gaussian_pyramid(const Mat& img,int levels);

#endif // gaussian_pyramid_H
