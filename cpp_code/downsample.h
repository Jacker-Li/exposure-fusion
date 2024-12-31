#ifndef downsample_H
#define downsample_H

#include <opencv2/opencv.hpp>
#include"downsample.h"

using namespace cv;

Mat downsample(const Mat& img,const Mat& filter);

#endif // downsample_H
