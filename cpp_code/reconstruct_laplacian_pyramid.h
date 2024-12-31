#ifndef reconstruct_laplacian_pyramid_H
#define reconstruct_laplacian_pyramid_H

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat reconstruct_laplacian_pyramid(const vector<Mat>& im_v);
#endif // reconstruct_laplacian_pyramid_H
