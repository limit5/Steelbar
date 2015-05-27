////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////                                                                                //////////
//////////                           鋼棒分析運算變數資料結構                             //////////
//////////                                                                                //////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PARAMETER_HPP__
#define __PARAMETER_HPP__

// C++ 字串
#include <string>

// C++ 向量
#include <vector>

// OpenCV 函式庫
#include "OpenCVLib.h"


// 鋼棒分析程式運算資料結構
class Parameter
{
public:
	
	/**
	*
	* @brief		建構子
	*
	* @param[in]	沒有
	*
	* @param[out]	沒有
	*
	* @return		沒有
	*
	* @note			空的建構子
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	Parameter();
	
	/**
	*
	* @brief		解構子
	*
	* @param[in]	沒有
	*
	* @param[out]	沒有
	*
	* @return		沒有
	*
	* @note			空的解構子
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	~Parameter();
	
	/**
	*
	* @brief		垂直追蹤點樣板的 X 向 大小
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int vTemplateSizeX;
	
	/**
	*
	* @brief		垂直追蹤點樣板的 Y 向 大小
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int vTemplateSizeY;
	
	/**
	*
	* @brief		水平追蹤點樣板的 X 向 大小
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int hTemplateSizeX;
	
	/**
	*
	* @brief		水平追蹤點樣板的 Y 向 大小
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int hTemplateSizeY;
	
	/**
	*
	* @brief		垂直追蹤點數量
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int nVtemplate;
	
	/**
	*
	* @brief		水平追蹤點數量
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int nHtemplate;
	
	/**
	*
	* @brief		校正板棋盤格每一小格的 X 向長度
	*
	* @note			單位:mm
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	float cellSizeDx;
	
	/**
	*
	* @brief		校正板棋盤格每一小格的 Y 向長度
	*
	* @note			單位:mm
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	float cellSizeDy;
	
	/**
	*
	* @brief		校正板棋盤格 X 向 的格子交叉點數量
	*
	* @note	
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int patternSizeNx;
	
	/**
	*
	* @brief		校正板棋盤格 Y 向 的格子交叉點數量
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int patternSizeNy;
	
	/**
	*
	* @brief		鋼棒試體的直徑
	*
	* @note			單位:mm
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	float diameter;

	/**
	*
	* @brief		相機校正: 相機內部參數
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	cv::Mat camMatrix;
	
	/**
	*
	* @brief		相機校正: 形變參數
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	cv::Mat disCoeffs;
	
	/**
	*
	* @brief		相機校正: 選轉矩陣
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	cv::Mat coordR;
	
	/**
	*
	* @brief		相機校正: 位移向量
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	cv::Mat coordT;
	

	/**
	*
	* @brief		正攝影像的高
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int rectfImageHeight;
	
	/**
	*
	* @brief		正攝影像的寬
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int rectfImageWidth;
	
	/**
	*
	* @brief		pixel 和 實際單位(mm) 的轉換倍率
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	float rpx;
	
	/**
	*
	* @brief		物理長度
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	float physicalDistance;
	
	/**
	*
	* @brief		影像長度
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	float imageDistance;
	
	/**
	*
	* @brief		相機校正: 棋盤格點座標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<cv::Point2f> chessImagePoints;
	
	/**
	*
	* @brief		相機校正: 3D物件點
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	cv::Mat gridObjectPoints3D;
	
	/**
	*
	* @brief		相機校正: 格點影像
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	cv::Mat gridImagePoints;




	/**
	*
	* @brief		校正影像實體檔案路徑
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::string calibImagePath;
	
	/**
	*
	* @brief		校正影像的檔案名
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<std::string> calibImageFileName;
	
	/**
	*
	* @brief		實驗影像的實體檔案路徑
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::string expImagePath;
	
	/**
	*
	* @brief		實驗影像的檔案名
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<std::string> expImageFileName;
	
	/**
	*
	* @brief		實驗影像的檔案名(不含副檔名)
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<std::string> expImageFileNameWithoutPath;
	
	/**
	*
	* @brief		正攝影像的檔案名
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<std::string> rectifyImageFileName;
	
	/**
	*
	* @brief		輸出檔案路徑
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::string outputPath;
	
	/**
	*
	* @brief		錯誤訊息
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::string errMsg;


	/**
	*
	* @brief		校正影像
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<cv::Mat> calibImage;
	
	/**
	*
	* @brief		實驗影像
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<cv::Mat> expImage;
	
	/**
	*
	* @brief		正攝影像
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<cv::Mat> rectifyImage;

	std::vector<std::vector<uchar>> compressCalibImage;
	std::vector<std::vector<uchar>> compressExpImage;
	std::vector<std::vector<uchar>> compressRectifyImage;

	/**
	*
	* @brief		正攝用校正基準影像
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int rectifyBaseImageId;
	
	/**
	*
	* @brief		外部匯入校正檔案 旗標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	bool calibDataImprotFromExternal;
	
	/**
	*
	* @brief		外部匯入校正檔案存在 旗標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	bool hasExternalCalibData;
	
	/**
	*
	* @brief		外部校正檔案路徑
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::string externalCalibDataPath;
	
	/**
	*
	* @brief		外部校正檔案名
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::string externalCalibDataName;


	/**
	*
	* @brief		分析結果 水平點座標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<std::vector<cv::Point2f>> hPointsData;
	
	/**
	*
	* @brief		分析結果 垂直點座標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<std::vector<cv::Point2f>> vPointsData;


	/**
	*
	* @brief		分析結果 直徑
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<float> analysisDiameters;
	
	/**
	*
	* @brief		分析結果 拉伸量
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<float> analysisElongations;

	/**
	*
	* @brief		影像壓縮率
	*
	* @note			For PNG, it can be the compression level ( CV_IMWRITE_PNG_COMPRESSION ) from 0 to 9. 
	*				A higher value means a smaller size and longer compression time. Default value is 3.
	*
	*				For JPEG, it can be a quality ( CV_IMWRITE_JPEG_QUALITY ) from 0 to 100 (the higher is the better). Default value is 95.
	*
	*
	* @author		Limit5
	*
	* @date		2014/09/13
	*/
	int imageCompressLevel;
	
	/**
	*
	* @brief		影像壓縮種類
	*
	* @note			PNG or JPEG
	*
	* @author		Limit5
	*
	* @date		2014/09/13
	*/
	int imageCompressType;

	/**
	*
	* @brief		影像壓縮參數
	*
	* @note			
	*
	* @author		Limit5
	*
	* @date		2014/09/13
	*/
	std::vector<int> imageCompressParameter;

	/**
	*
	* @brief		工作執行續 指標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	CWinThread* m_pThread;
	
	/**
	*
	* @brief		訊息執行續 指標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	CWinThread* m_pMsgThread;
	
	/**
	*
	* @brief		檢查工作執行續存在 指標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	CWinThread* m_pCheckWorkThread;
	
	/**
	*
	* @brief		檢查 Ui 參數 指標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/09/13
	*/
	CWinThread* m_pCheckUiParameterThread;

	/**
	*
	* @brief		工作執行續的 HANDLE
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	HANDLE m_hKillEvent;

	/**
	*
	* @brief		分析方法外掛名 清單
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	std::vector<std::string> analysisMethodDllList;
	
	/**
	*
	* @brief		選中的分析方法 ID
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	int analysisMethodIndex;
	
	/**
	*
	* @brief		中斷工作執行續 旗標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	bool terminateThread;
	
	/**
	*
	* @brief		記憶體精簡化 旗標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	bool memoryLite;
	
	/**
	*
	* @brief		靜音模式 旗標
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	bool mute;

private:

}; // end of class Parameter


#endif // end of #ifndef __PARAMETER_HPP__