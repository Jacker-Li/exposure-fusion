#include <opencv2/opencv.hpp>
#include"pyramid_filter.h"
#include"upsample.h"
#include"reconstruct_laplacian_pyramid.h"

using namespace cv;

// ÖØ½¨Laplacian½ð×ÖËþ

Mat reconstruct_laplacian_pyramid(const vector<Mat>& im_v){
    Mat cur=im_v.back(),filter=pyramid_filter();
    int sz=im_v.size();
    for(int i=sz-2;~i;--i){
        Mat up=upsample(cur,im_v[i].size(),filter);
        cur=up+im_v[i];
    }return cur;
}