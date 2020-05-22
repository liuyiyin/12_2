// 12_2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap(0);

	if (!cap.isOpened())
	{
		std::cout << "不能打开视频文件" << std::endl;
		return-1;
	}

	std::vector<cv::Mat> srcMats;

	int nBg = 200;
	float wVar = 1;

	int cnt = 0;
	cv::Mat frame;
	cv::Mat meanMat;
	cv::Mat varMat;
	cv::Mat dstMat;

	while (true) {

		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		if (cnt < nBg) {
			srcMats.push_back(frame);
			if (cnt == 0) {
				std::cout << "reading frame" << std::endl;
			}
		}

		else if (cnt == nBg) {
			meanMat.create(frame.size(), CV_8UC1);
			varMat.create(frame.size(), CV_32FC1);
			std::cout << "calculating background models" << std::endl;
			int rows = srcMats[0].rows;
			int cols = srcMats[0].cols;
			for (int h = 0; h < rows; h++)
			{
				for (int w = 0; w < cols; w++)
				{
					int sum = 0;
					float var = 0;
					for (int i = 0; i < srcMats.size(); i++) {
						sum += srcMats[i].at<uchar>(h, w);
					}
					meanMat.at<uchar>(h, w) = sum / srcMats.size();
					for (int i = 0; i < srcMats.size(); i++) {
						var += pow((srcMats[i].at<uchar>(h, w) - meanMat.at<uchar>(h, w)), 2);
					}
					varMat.at<float>(h, w) = var / srcMats.size();
				}
			}
		}

		else {
			dstMat.create(frame.size(), CV_8UC1);
			int srcI;
			int meanI;
			int dstI;
			int rows = frame.rows;
			int cols = frame.cols;
			for (int h = 0; h < rows; h++)
			{
				for (int w = 0; w < cols; w++)
				{
					srcI = frame.at<uchar>(h, w);
					meanI = meanMat.at<uchar>(h, w);
					int dif = abs(srcI - meanI);
					int th = wVar*varMat.at<float>(h, w);
					if (dif > th) {
						dstMat.at<uchar>(h, w) = 255;
					}
					else {
						dstMat.at<uchar>(h, w) = 0;
					}
				}
			}
			imshow("result", dstMat);
			imshow("frame", frame);
			waitKey(30);
		}

		cnt++;

	}
    return 0;
}

