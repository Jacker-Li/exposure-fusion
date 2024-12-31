#include<opencv2/opencv.hpp>
#include"pyramid_filter.h"

using namespace cv;

Mat pyramid_filter(){
    return (Mat_<float>(1,5)<<0.0625f,0.25f,0.375f,0.25f,0.0625f);
}
