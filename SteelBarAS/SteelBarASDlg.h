
// SteelBarASDlg.h : ヘッダー ファイル
//

#pragma once

// 鋼棒程式參數結構
#include "Parameter.hpp"

// OpenCV 函式庫
#include "OpenCVLib.h"

// 相機校正功能
#include "ImProCore_CamCalib.h"

// 選點功能
#include "ImProHui_pickAPoint.h"

// 平滑縮放功能
#include "ImProHui_smoothZoomAndShow.h"

// IplImage繪製到 MFC 的 DC 用
#include "CvvImage.h"

// 執行續
#include <Windows.h>

// 檔案串流
#include <fstream>

#include "afxwin.h"

#include "afxcmn.h"

// CSteelBarASDlg ダイアログ
class CSteelBarASDlg : public CDialogEx
{
// コンストラクション
public:
	CSteelBarASDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_STEELBARAS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

public:

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();


	//////////////////////////////////////////////////
	//////////      CSteelBar 成員函數      //////////
	//////////////////////////////////////////////////

	/**
	 *
	 * @brief		初始化
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void Init();
	
	/**
	 *
	 * @brief		UI數值初始化
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void ValueInit();
	
	/**
	 *
	 * @brief		輸出路徑初始化
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void DirectoryInit();
	
	/**
	 *
	 * @brief		變數初始化
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void VariableInit();
	
	/**
	 *
	 * @brief		選擇檔案
	 *
	 * @param[in]	pathItemId		UI上顯示資料夾路徑的控制ID
	 * @param[in]	ListItemId		UI上顯示資料夾內檔案名的控制ID
	 * @param[in]	calibOrExp		選擇校正資料或實驗資料
	 *
	 * @param[out]	沒有
	 *
	 * @return		err				回傳選擇是否成功 0以外皆失敗
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg int SelectPath(int pathId, int filenameId, bool calibOrExp);
	
	/**
	 *
	 * @brief		選擇路徑
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void SelectFolder();
	
	DECLARE_MESSAGE_MAP()
public:
	
	/**
	 *
	 * @brief		選擇校正檔案路徑
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnBnClickedButtonCalibImageSelectPath();
	
	/**
	 *
	 * @brief		選擇實驗照片路徑
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnBnClickedButtonExpImageSelectPath();
	
	/**
	 *
	 * @brief		執行
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnBnClickedButtonRun();
	
	/**
	 *
	 * @brief		輸出訊息
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void message();
	
	/**
	 *
	 * @brief		輸出訊息至特定CEditBox (CString)
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void PostMessageToLog(CString msg);
	
	/**
	 *
	 * @brief		輸出訊息至特定CEditBox (std::string)
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void PostMessageToLog(std::string msg);
	
	/**
	 *
	 * @brief		將UI參數存入變數
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void SetParameterFromUi();
	
	/**
	 *
	 * @brief		相機校正
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void CalibrateImage();
	
	/**
	 *
	 * @brief		讀取外部校正檔案
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void LoadExternalCalibData();
	
	/**
	 *
	 * @brief		產生物件點
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void GenerateObjectPoints();
	
	/**
	 *
	 * @brief		產生影像點
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void GenerateImagePoints();
	
	/**
	 *
	 * @brief		產生正射影像
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void GenerateRectifyImage();
	
	/**
	 *
	 * @brief		計算
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void Calculate();
	
	/**
	 *
	 * @brief		輸出計算結果
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void WriteCalcData();
	
	/**
	 *
	 * @brief		選擇輸出路徑
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnBnClickedButtonSelectOutputPath();
	
	/**
	 *
	 * @brief		中斷工作
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnBnClickedButtonStop();
	
	/**
	 *
	 * @brief		選擇校正基準影像
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnLbnDblclkListBoxCalibImagePath();
	
	/**
	 *
	 * @brief		畫IplImage到指定DC上
	 *
	 * @param[in]	img		輸入影像
	 * @param[in]	Id		輸出DC之ID
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void DrawImageToHDC(IplImage* img, UINT ID);
	
	/**
	 *
	 * @brief		清除
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnBnClickedButtonClear();
	
	/**
	 *
	 * @brief		輸出校正資料
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OutputCalibrationData();
	
	/**
	 *
	 * @brief		清除變數
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void VariableClear();
	
	/**
	 *
	 * @brief		清除UI數值
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void UiDataClear();
	
	/**
	 *
	 * @brief		訊息DC自動捲動至最底下
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void MessageBoxAutoScrollToBottom();
	
	/**
	 *
	 * @brief		載入外部校正資料
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnBnClickedButtonLoadExternalCalibData();
	


	/**
	 *
	 * @brief		讀取外部分析方法
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void LoadExternalAnalysisMethod();

	/**
	 *
	 * @brief		初始化分析方法
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void AnalysisMethodInit();

	/**
	 *
	 * @brief		從指定路徑取得分析方法外掛
	 *
	 * @param[in]	folder	外掛路徑
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void GetAnalysisMethodDirectory(std::string folder);

	/**
	 *
	 * @brief		當分析方法下拉式選單被選中的項目改變時顯示選中的項目訊息
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	afx_msg void OnCbnSelchangeComboAnalysisMethod();

	/**
	*
	* @brief		將 image 資料壓縮後存入  buffer
	*
	* @param[in]	image	cv::Mat 影像資料
	*
	* @param[in]	buffer	std::vector<uchar> 壓縮後的影像資料空間
	*
	* @param[out]	沒有
	*
	* @return		沒有
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/09/13
	*/
	afx_msg void CompressMat(cv::Mat image, std::vector<uchar> buffer);

	/**
	*
	* @brief		檢查 Ui 上的 特定參數
	*
	* @param[out]	沒有
	*
	* @return		沒有
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/09/13
	*/
	afx_msg void CheckUiParameter();

	/**
	*
	* @brief		取得 UI 上 滑桿 游標 位置
	*
	* @param[out]	沒有
	*
	* @return		沒有
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/09/13
	*/
	afx_msg void GetSliderBarValue();

	/**
	*
	* @brief		當 影像壓縮等級 滑桿 游標改變時進行動作
	*
	* @param[in]	pNMHDR 滑桿 控制 指標
	*
	* @param[in]	pResult 滑桿 控制 結果
	*
	* @param[out]	沒有
	*
	* @return		沒有
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/09/13
	*/
	afx_msg void OnNMCustomdrawSliderBarImageCompressLevel(NMHDR *pNMHDR, LRESULT *pResult);

	//////////////////////////////////////////////////
	//////////      CSteelBar 成員變數      //////////
	//////////////////////////////////////////////////

	/**
	 *
	 * @brief		控制 UI 上的 校正影像清單
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CListBox m_listCalibImagePath;

	/**
	 *
	 * @brief		控制 UI 上的 主進度條
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CProgressCtrl m_progressCtrlGlobal;

	/**
	 *
	 * @brief		控制 UI 上的 副進度條
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CProgressCtrl m_progressCtrlPartial;

	/**
	 *
	 * @brief		控制 UI 上的 選擇外部校正資料按鈕
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CButton m_buttonLoadExtCalibData;

	/**
	*
	* @brief		控制 UI 上的 清除按鈕
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	CButton m_buttonClear;

	/**
	 *
	 * @brief		控制 UI 上的 執行按鈕
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CButton m_buttonRun;

	/**
	 *
	 * @brief		控制 UI 上的 暫停按鈕
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CButton m_buttonStop;

	/**
	 *
	 * @brief		控制 UI 上的 選擇校正影像路徑按鈕
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CButton m_buttonCalibImgSelPath;

	/**
	 *
	 * @brief		控制 UI 上的 選擇實驗影像路徑按鈕
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CButton m_buttonExpImgSelPath;

	/**
	 *
	 * @brief		控制 UI 上的 選擇輸出路徑按鈕
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CButton m_buttonSelOutputPath;

	/**
	*
	* @brief		控制 UI 上的 記憶體精簡模式 選項
	*
	* @note
	*
	* @author		Limit5
	*
	* @date		2014/06/20
	*/
	CButton m_checkBoxMemoryLite;

	/**
	 *
	 * @brief		訊息執行續的訊息暫存
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CString tMsgLog;


	/**
	 *
	 * @brief		鋼棒程式訊息全域變數
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CString msgAndLog;

	/**
	 *
	 * @brief		輸出路徑資料夾名
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	TCHAR   szPathName[MAX_PATH];

	/**
	 *
	 * @brief		相機校正物件
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	CamCalib camCalib;

	/**
	 *
	 * @brief		鋼棒參數結構
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	Parameter parameter;


	//////////////////////////////////////////////////
	//////////   CSteelBar 外掛函數 定義    //////////
	//////////////////////////////////////////////////


	/**
	 *
	 * @brief		定義 影像不連續 分析方法 功能
	 *
	 * @param[in]	const cv::Mat&	被分析的影像
	 *
	 * @param[in]	void*			保留
	 *
	 * @param[out]	沒有
	 *
	 * @return		float*			運算結果
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	//typedef float* (__stdcall *ANALYSISMETHOD)(const cv::Mat&, void*);

	/**
	 *
	 * @brief		定義 影像不連續 分析方法 功能
	 *
	 * @param[in]	const std::vector<cv::Mat>&	被分析的系列影像
	 *
	 * @param[in]	void*			保留
	 *
	 * @param[out]	沒有
	 *
	 * @return		float**			運算結果
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	typedef float** (__stdcall *ANALYSISMETHOD)(const std::vector<cv::Mat>&, void*);

	/**
	 *
	 * @brief		定義 分析方法 名稱
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		std::string		分析方法 名稱
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	typedef std::string (__stdcall *GETANALYSISNAME)(void);

	/**
	 *
	 * @brief		定義 分析方法 版本
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		std::string		分析方法 版本
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	typedef std::string (__stdcall *GETANALYSISVERSION)(void);

	/**
	 *
	 * @brief		定義 分析方法 簡述內容
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		std::string		分析方法 檢述內容
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	typedef std::string (__stdcall *GETANALYSISCAPTION)(void);

	/**
	 *
	 * @brief		定義 分析方法 連續型態
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		bool		分析方法 連續型態 
	 *
	 * @note		目前不使用
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	typedef bool (__stdcall *GETANALSYISCONTINUETYPE)(void);
	
	/**
	 *
	 * @brief		定義 分析方法 釋放記憶體
	 *
	 * @param[in]	沒有
	 *
	 * @param[out]	沒有
	 *
	 * @return		沒有
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	typedef void(__stdcall *FREEANALYSISDATAMEMORY)(void);


	//////////////////////////////////////////////////
	//////////    CSteelBar 外掛函數變數    //////////
	//////////////////////////////////////////////////

	
	/**
	 *
	 * @brief		分析方法 運算函數 實體
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	std::vector<ANALYSISMETHOD> analysisMethods;

	/**
	 *
	 * @brief		分析方法 運算函數 實體 (連續系列影像)
	 *
	 * @note		暫時不使用
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	//std::vector<ANALYSISMETHODC> analysisMethodCs;
	
	/**
	 *
	 * @brief		分析方法 名稱函數 實體
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	std::vector<GETANALYSISNAME> analysisNames;
	
	/**
	 *
	 * @brief		分析方法 內容簡述函數 實體
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	std::vector<GETANALYSISCAPTION> analysisCaptions;
	
	/**
	 *
	 * @brief		分析方法 版本函數 實體
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	std::vector<GETANALYSISVERSION> analysisVersions;
	
	/**
	 *
	 * @brief		分析方法 連續型態函數 實體
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	std::vector<GETANALSYISCONTINUETYPE> analysisContinueTypes;
	
	/**
	 *
	 * @brief		分析方法 釋放記憶體函數 實體
	 *
	 * @note
	 *
	 * @author		Limit5
	 *
	 * @date		2014/06/20
	 */
	std::vector<FREEANALYSISDATAMEMORY> analysisFreeMemory;
	
}; // end of class CSteelBarASDlg : public CDialogEx
