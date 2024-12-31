#include<opencv2/opencv.hpp>
#include"upsample.h"

using namespace cv;

// ÉÏ²ÉÑù²¢ÂË²¨
Mat upsample(const Mat& img,const Size& sz,const Mat& filter){
    Mat tmp,up;
    resize(img,up,sz,0,0,INTER_LINEAR);
    sepFilter2D(up,tmp,CV_32F,filter,filter.t(),Point(-1,-1),0,BORDER_REFLECT);
    return tmp;
}
