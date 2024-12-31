#ifndef EXPOSURE_FUSION_H
#define EXPOSURE_FUSION_H

#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

vector<Mat>laplacian_pyramid(const Mat& img,int levels);
#endif // EXPOSURE_FUSION_H
