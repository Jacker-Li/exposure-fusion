#include<opencv2/opencv.hpp>
#include"pyramid_filter.h"
#include"downsample.h"
#include"upsample.h"
#include"laplacian_pyramid.h"

using namespace std;
using namespace cv;

// ¹¹½¨Laplacian½ð×ÖËþ
vector<Mat>laplacian_pyramid(const Mat& img,int levels){
    vector<Mat>im_v;
    Mat cur=img.clone(),filter=pyramid_filter();
    for(int i=0;i<levels-1;++i){
        Mat down=downsample(cur,filter),up=upsample(down,cur.size(),filter),Lap=cur-up;
        im_v.push_back(Lap);
        cur=down;
    }im_v.push_back(cur);//×îµ×²ã
    return im_v;
}
