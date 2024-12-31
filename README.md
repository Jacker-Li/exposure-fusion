# exposure-fusion

曝光融合cpp+opencv版本

配置:Win11+VS2022专业版+opencv-4.10.0

参考资料:
原始论文:https://ieeexplore.ieee.org/abstract/document/4392748
论文附matlab版代码:https://github.com/Mericam/exposure-fusion?tab=BSD-2-Clause-1-ov-file

测试图片可以使用https://github.com/Mericam/exposure-fusion/tree/master/house

和matlab相比具有一定的精度问题,按照原论文代码中取层数的方法,在某些情况可能会过曝.必要时可以手动设置层数取值,以及尝试调整精度(如float换成double,某些情况可以尝试使用CV_64F减少精度损失,但要注意兼容)
本代码为方便起见仅使用float+CV_32F.

如下面4帧输入图像
![0](https://github.com/user-attachments/assets/3ebe99e3-8ba6-45b6-9ad1-d5c123bb2b23)
![1](https://github.com/user-attachments/assets/ab84f32e-c941-4327-ba02-a6fc338f031b)
![2](https://github.com/user-attachments/assets/a1f88d73-d32b-40ac-acfc-a7a5a4f0af3e)
![3](https://github.com/user-attachments/assets/3d869dd8-e54a-4031-8127-7bd3617013d1)

特征融合后(特征金字塔设置为11层):
![road11(auto)](https://github.com/user-attachments/assets/8a13e9c0-10d9-4c88-a1cf-7703493ca57b)
