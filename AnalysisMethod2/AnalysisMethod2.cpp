// AnalysisMethod2.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"


#include "opencv2/core/core.hpp"
// opencv header
#include <opencv2\opencv.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include <opencv2\legacy\legacy.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc_c.h>
#include <opencv2\legacy\compat.hpp>

// opencv library
#ifdef _DEBUG
#pragma comment(lib,"opencv_imgproc249d.lib")
#pragma comment(lib,"opencv_core249d.lib")
#pragma comment(lib,"opencv_highgui249d.lib")
#pragma comment(lib,"opencv_calib3d249d.lib")
#pragma comment(lib,"opencv_nonfree249d.lib")
#pragma comment(lib,"opencv_legacy249d.lib")
#pragma comment(lib,"opencv_features2d249d.lib")
#pragma comment(lib, "opencv_video249d.lib")
#else
#pragma comment(lib,"opencv_imgproc249.lib")
#pragma comment(lib,"opencv_core249.lib")
#pragma comment(lib,"opencv_highgui249.lib")
#pragma comment(lib,"opencv_calib3d249.lib")
#pragma comment(lib,"opencv_nonfree249.lib")
#pragma comment(lib,"opencv_legacy249.lib")
#pragma comment(lib,"opencv_features2d249.lib")
#pragma comment(lib, "opencv_video249.lib")
#endif // ifdef _DEBUG

float analsisResults[6] = { 0.0 };

extern "C" __declspec(dllexport)
float* analysisMethod(const cv::Mat& image, void* param = 0)
{



	try
	{
		analsisResults[0] = 11.44f; // elongation
		analsisResults[1] = -52.33f; // diameter
		analsisResults[2] = 33.f; // left x
		analsisResults[3] = 11.f; // left y
		analsisResults[4] = 3465.f; // right x
		analsisResults[5] = 234523.23f; // right y
		cv::imshow("method2", image);
		cv::waitKey(1);
	}
	catch (std::exception& ex)
	{
		throw;
	}
	catch (...)
	{
		throw;
	}
	return analsisResults;
}

extern "C" __declspec(dllexport)
std::string getAnalysisName()
{
	return "SampleMethod2";
}

extern "C" __declspec(dllexport)
std::string getVersion()
{
	return "2014/07/02 2版";
}

extern "C" __declspec(dllexport)
std::string getCaption()
{
	return "This is a sample method 2.";
}