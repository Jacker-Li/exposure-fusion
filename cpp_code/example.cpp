#include<opencv2/opencv.hpp>
#include"pyramid_filter.h"
#include"downsample.h"
#include"upsample.h"
#include"laplacian_pyramid.h"
#include"reconstruct_laplacian_pyramid.h"
#include"gaussian_pyramid.h"
#include<iostream>
#include<vector>
#include<cstring>
#include<cmath>

using namespace cv;
using namespace std;

// 算对比度（单通道）
Mat Contrast(const Mat& img){
	Mat gray,laplacian;
	cvtColor(img,gray,COLOR_BGR2GRAY);
	Laplacian(gray,laplacian,CV_32F);
	return abs(laplacian);
}

// 算饱和度/标准差（单通道）
// 暴力写法,略慢
//Mat computeSaturation(const Mat& img){
//	// img为CV_32FC3,范围[0,1]
//	vector<Mat>ch;
//	split(img,ch);
//	// 计算每个像素的标准差
//	Mat sat(img.size(),CV_32F);
//	for(int i=0;i<img.rows;++i){
//		float* t=sat.ptr<float>(i);
//		const float* r=ch[0].ptr<float>(i);
//		const float* g=ch[1].ptr<float>(i);
//		const float* b=ch[2].ptr<float>(i);
//		for(int j=0;j<img.cols;++j){
//			float mu=(r[j]+g[j]+b[j])/3.0f;
//			float var=((r[j]-mu)*(r[j]-mu)+(g[j]-mu)*(g[j]-mu)+(b[j]-mu)*(b[j]-mu))/3.0f;
//			t[j]=sqrt(var);
//		}
//	}
//	return sat;
//}

// 向量化写法
Mat Saturation(const Mat& img){
	// img 为 CV_32FC3，范围 [0,1]
	vector<Mat>ch(3);
	split(img,ch);
	// 计算均值
	Mat mean=(ch[0]+ch[1]+ch[2])/3.0f;
	// 计算方差
	Mat var=((ch[0]-mean).mul(ch[0]-mean)+(ch[1]-mean).mul(ch[1]-mean)+(ch[2]-mean).mul(ch[2]-mean))/3.0f;
	// 计算标准差（饱和度
	Mat sat;
	sqrt(var,sat);
	return sat;
}

// 算曝光性（单通道）
Mat WellExposedness(const Mat& img){
	// img: CV_32FC3 in [0,1]
	vector<Mat>ch;
	split(img,ch);
	Mat well=Mat::ones(img.size(),CV_32F);
	float sigma=0.2f;
	for(int c=0;c<3;++c){
		Mat diff=ch[c]-0.5f,expImg;
		exp(-0.5f*diff.mul(diff)/(sigma*sigma),expImg);
		well=well.mul(expImg);
	}
	return well;
}


// 根据输入图像设置最高金字塔层数
int ask_levels(const Mat& img){
	// 图像的行（高）和列（宽）
	int r=img.rows,c=img.cols;
	// 计算金字塔的最高层数（实际上要-1）
	int levels=static_cast<int>(floor(log2(min(r,c))));
	return levels;
}

int main(){
	vector<string>filenames={
		// 输入图片路径：如./a.jpg,./b.jpg
	};
	vector<Mat>images;
	for(auto& fn:filenames){
		Mat img=imread(fn);
		if(img.empty()){
			cerr<<"图片空的!"<<fn<<endl;
			return -1;
		}img.convertTo(img,CV_32F,1.0/255.0);
		images.push_back(img);
	}
	// 确保图像大小一致
	Size sz_p=images[0].size();
	int sz=images.size();
	for(int i=1;i<sz;++i){
		if(images[i].size()!=sz_p){
			resize(images[i],images[i],sz_p,0,0,INTER_LINEAR);
		}
	}
	// 计算权重
	// 参数顺序设置成(contrast_parm,sat_parm,wexp_parm)
	float contrast_parm=1.0f,sat_parm=1.0f,wexp_parm=1.0f;
	vector<Mat>weights;
	vector<Mat>Wc,Ws,Ww;
	for(auto& img:images){
		Mat weight=Mat::ones(img.size(),CV_32F);
		if(contrast_parm>0){
			Mat c=Contrast(img);
			pow(c,contrast_parm,c);
			weight=weight.mul(c);
		}if(sat_parm>0){
			Mat s=Saturation(img);
			pow(s,sat_parm,s);
			weight=weight.mul(s);
		}if(wexp_parm>0){
			Mat w=WellExposedness(img);
			pow(w,wexp_parm,w);
			weight=weight.mul(w);
		}
		weight+=1e-12f;// 防止除不尽
		weights.push_back(weight);
	}

	// 权重归一化
	Mat sumW=Mat::zeros(sz_p,CV_32F);
	for(auto& w:weights){sumW+=w;}
	for(auto& w:weights){w=w/sumW;}
	// 将单通道权重扩展为三通道
	for(auto& w:weights){
		Mat w3;
		merge(vector<Mat>{w,w,w},w3);
		w=w3;
	}
	int levels=ask_levels(images[0]);
	// cout<<levels<<"\n";
	// 构建图像的拉普拉斯金字塔和权重的高斯金字塔
	vector<vector<Mat>>lapPyramids(images.size()),weightPyramids(images.size());
	sz=images.size();
	for(int i=0;i<sz;++i){
		lapPyramids[i]=laplacian_pyramid(images[i],levels);
		weightPyramids[i]=gaussian_pyramid(weights[i],levels);
	}
	// 图像融合
	vector<Mat>fusedPyramid(levels);
	for(int l=0;l<levels;++l){
		fusedPyramid[l]=Mat::zeros(lapPyramids[0][l].size(),CV_32FC3);
		Mat sumWeight=Mat::zeros(lapPyramids[0][l].size(),CV_32FC3);
		sz=images.size();
		for(int i=0;i<sz;++i){
			// 权重为高斯金字塔层weightPyramids[i][l]，
			// 其大小和lapPyramids[i][l]一样
			fusedPyramid[l]+=lapPyramids[i][l].mul(weightPyramids[i][l]);
		}
	}
	// 重建最终图像
	Mat result=reconstruct_laplacian_pyramid(fusedPyramid);
	result=max(min(result,1.0f),0.0f);
	result.convertTo(result,CV_8UC3,255.0);
	imshow("融合结果",result);
	waitKey(0);
	return 0;
}
