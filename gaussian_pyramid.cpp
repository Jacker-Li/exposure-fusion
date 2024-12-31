#include<opencv2/opencv.hpp>
#include"pyramid_filter.h"
#include"downsample.h"
#include"gaussian_pyramid.h"

using namespace cv;

// ������˹������������Ȩ�أ�
vector<Mat>gaussian_pyramid(const Mat& img,int levels){
    vector<Mat>im_v;//�������ÿ��ͼ��
    Mat cur=img.clone(),filter=pyramid_filter();
    im_v.push_back(cur);
    for(int i=0;i<levels-1;++i){
        cur=downsample(cur,filter);
        im_v.push_back(cur);
    }return im_v;
}