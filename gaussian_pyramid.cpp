#include<opencv2/opencv.hpp>
#include"pyramid_filter.h"
#include"downsample.h"
#include"gaussian_pyramid.h"

using namespace cv;

// 构建高斯金字塔（用于权重）
vector<Mat>gaussian_pyramid(const Mat& img,int levels){
    vector<Mat>im_v;//存金字塔每层图像
    Mat cur=img.clone(),filter=pyramid_filter();
    im_v.push_back(cur);
    for(int i=0;i<levels-1;++i){
        cur=downsample(cur,filter);
        im_v.push_back(cur);
    }return im_v;
}