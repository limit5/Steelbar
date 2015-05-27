// AnalysisMethod.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AnalysisMethod.h"


/**
*
* @brief		分析結果
*
* @note			全域變數
*
* @author		Limit5
*
* @date		2014/06/20
*/
float* analsisResults[10] = {0};

extern "C" __declspec(dllexport)
float** analysisMethod(const std::vector<cv::Mat>& image, void* param)
{
	// 嘗試: 進行分析運算
	try
	{
		// 配置分析結果 記憶體空間
		for (size_t i = 0; i < 10; i++)
		{
			// 依序配置分析結果
			analsisResults[i] = new float[image.size()];
		}
		for (size_t i = 0; i < image.size(); i++)
		{
			analsisResults[0][i] = 10.f; // elongation
			analsisResults[1][i] = -12.f; // diameter
			analsisResults[2][i] = 33.f; // left x
			analsisResults[3][i] = 11.f; // left y
			analsisResults[4][i] = 3465.f; // right x
			analsisResults[5][i] = 234523.23f; // right y
			analsisResults[6][i] = 3465.f; // up x
			analsisResults[7][i] = 234523.23f; // up y
			analsisResults[8][i] = 3465.f; // bottom x
			analsisResults[9][i] = 234523.23f; // bottom y

		}
	}
	// 捕捉例外
	// 如果發生 C++ 標準以外的例外則接住
	catch (std::exception& ex)
	{
		// 往上層再丟
		throw;
	}
	// 捕捉例外
	// 如果發生無法預期的例外
	catch (...)
	{
		// 往上層再丟
		throw;
	}

	// 回傳 分析結果
	return analsisResults;
}

extern "C" __declspec(dllexport)
std::string getAnalysisName()
{
	// 回傳 分析方法 名稱
	return "SampleMethod3";
}

extern "C" __declspec(dllexport)
std::string getVersion()
{
	// 回傳 分析版本
	return "2014/02/03版";
}

extern "C" __declspec(dllexport)
std::string getCaption()
{
	// 回傳 內容簡述
	return "This is a sample method.\r\r\nContinue Test.";
}

extern "C" __declspec(dllexport)
bool getContinueType()
{
	// 回傳 連續型態
	return true;
}

extern "C" __declspec(dllexport)
void freeDataMemory()
{
	// 依序釋放記憶體
	for (size_t i = 0; i < 10; i++)
	{
		// 釋放 分析結果空間
		delete[] analsisResults[i];
	}

	// 結束回傳
	return;
}