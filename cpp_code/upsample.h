#ifndef upsample_H
#define upsample_H

#include<opencv2/opencv.hpp>

using namespace cv;

Mat upsample(const Mat& img,const Size& sz,const Mat& filter);

#endif // upsample_H
