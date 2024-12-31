#include<opencv2/opencv.hpp>
#include"downsample.h"

using namespace cv;

//sepFilter2D£º°×Êép151
Mat downsample(const Mat& img,const Mat& filter){
    Mat tmp,res;
    sepFilter2D(img,tmp,CV_32F,filter,filter.t(),Point(-1,-1),0,BORDER_REFLECT);
    resize(tmp,res,Size(tmp.cols>>1,tmp.rows>>1),0,0,INTER_NEAREST);
    return res;
}