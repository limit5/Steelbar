#ifndef _DLL_ANALYSIS_METHOD_H_
#define _DLL_ANALYSIS_METHOD_H_

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

// 如果是 C++ 形式
#ifdef __cplusplus
// 轉成 C 形式
extern "C"{
#endif // end of __cplusplus

	/**
	*
	* @brief		分析方法 本體
	*
	* @param[in]	image			被分析的系列影像
	*
	* @param[out]	沒有
	*
	* @return		analsisResults	分析結果
	*
	* @note			DLL 匯出函數
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	__declspec(dllexport) float**  analysisMethod(const std::vector<cv::Mat>& image, void* param = 0);

	/**
	*
	* @brief		分析方法 名稱
	*
	* @param[in]	沒有
	*
	* @param[out]	沒有
	*
	* @return		name	分析方法 名稱
	*
	* @note			DLL 匯出函數
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	__declspec(dllexport) std::string  getAnalysisName();
	
	/**
	*
	* @brief		分析方法 版本
	*
	* @param[in]	沒有
	*
	* @param[out]	沒有
	*
	* @return		version		版本
	*
	* @note			DLL 匯出函數
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	__declspec(dllexport) std::string  getVersion();
	
	/**
	*
	* @brief		分析方法 內容簡述
	*
	* @param[in]	沒有
	*
	* @param[out]	沒有
	*
	* @return		caption		內容
	*
	* @note			DLL 匯出函數
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	__declspec(dllexport) std::string  getCaption();
	
	/**
	*
	* @brief		分析方法 連續型態
	*
	* @param[in]	沒有
	*
	* @param[out]	沒有
	*
	* @return		type	是否連續
	*
	* @note			DLL 匯出函數
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	__declspec(dllexport) bool  getContinueType();
	
	/**
	*
	* @brief		分析方法 釋放記憶體
	*
	* @param[in]	沒有
	*
	* @param[out]	沒有
	*
	* @return		沒有
	*
	* @note			DLL 匯出函數
	*				Limit5: 看起來這個應該可以省略
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	__declspec(dllexport) void  freeDataMemory();


// 如果是 C++ 形式
#ifdef __cplusplus
} // end of extern "C"
#endif // end of __cplusplus

#endif // end of ifndef _DLL_ANALYSIS_METHOD_H_