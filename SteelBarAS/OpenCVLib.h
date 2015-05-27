#ifndef __OPENCV_LIB__
#define __OPENCV_LIB__




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

#endif