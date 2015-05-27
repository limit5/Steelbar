
// SteelBarASDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SteelBarAS.h"
#include "SteelBarASDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSteelBarASDlg ダイアログ



CSteelBarASDlg::CSteelBarASDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSteelBarASDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSteelBarASDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_buttonClear);
	DDX_Control(pDX, IDC_BUTTON_RUN, m_buttonRun);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_buttonStop);
	DDX_Control(pDX, IDC_BUTTON_CALIB_IMAGE_SELECT_PATH, m_buttonCalibImgSelPath);
	DDX_Control(pDX, IDC_BUTTON_EXP_IMAGE_SELECT_PATH, m_buttonExpImgSelPath);
	DDX_Control(pDX, IDC_BUTTON_SELECT_OUTPUT_PATH, m_buttonSelOutputPath);
	DDX_Control(pDX, IDC_LIST_BOX_CALIB_IMAGE_PATH, m_listCalibImagePath);
	DDX_Control(pDX, IDC_PROGRESS_BAR_GLOBAL, m_progressCtrlGlobal);
	DDX_Control(pDX, IDC_PROGRESS_BAR_PARTIAL, m_progressCtrlPartial);
	DDX_Control(pDX, IDC_BUTTON_LOAD_EXTERNAL_CALIB_DATA, m_buttonLoadExtCalibData);
	DDX_Control(pDX, IDC_CHECK_BOX_MEMORY_LITE, m_checkBoxMemoryLite);
}

BEGIN_MESSAGE_MAP(CSteelBarASDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CALIB_IMAGE_SELECT_PATH, &CSteelBarASDlg::OnBnClickedButtonCalibImageSelectPath)
	ON_BN_CLICKED(IDC_BUTTON_EXP_IMAGE_SELECT_PATH, &CSteelBarASDlg::OnBnClickedButtonExpImageSelectPath)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CSteelBarASDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_OUTPUT_PATH, &CSteelBarASDlg::OnBnClickedButtonSelectOutputPath)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSteelBarASDlg::OnBnClickedButtonStop)
	ON_LBN_DBLCLK(IDC_LIST_BOX_CALIB_IMAGE_PATH, &CSteelBarASDlg::OnLbnDblclkListBoxCalibImagePath)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSteelBarASDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_EXTERNAL_CALIB_DATA, &CSteelBarASDlg::OnBnClickedButtonLoadExternalCalibData)
	ON_CBN_SELCHANGE(IDC_COMBO_ANALYSIS_METHOD, &CSteelBarASDlg::OnCbnSelchangeComboAnalysisMethod)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BAR_IMAGE_COMPRESS_LEVEL, &CSteelBarASDlg::OnNMCustomdrawSliderBarImageCompressLevel)
END_MESSAGE_MAP()


// CSteelBarASDlg メッセージ ハンドラー

BOOL CSteelBarASDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	// 主程式 創建初始化
	this->Init();


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CSteelBarASDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CSteelBarASDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CSteelBarASDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////                                                                                //////////
//////////                      上方為 Visual studio 自動產生 code                        //////////
//////////                                                                                //////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
//////////        獨立功能函數          //////////
//////////////////////////////////////////////////

/**
*
* @brief		寬字元字串轉窄字元字串(std::wstring -> std::string)
*
* @param[in]	src		寬字元字串
*
* @param[out]	dest	窄字元字串
*
* @return		沒有
*
* @note
*
* @date			2014/06/20
*/
void wstr2str(const std::wstring &src, std::string &dest)
{
	// 輸出用字串暫存陣列
	char* mbs = new char[src.length() * MB_CUR_MAX + 1];

	// 被轉換的字元數
	size_t charsConverted = 0;

	// 將 src 轉換至 mbs
	wcstombs_s(
		&charsConverted,				// 被轉換的字元數
		mbs,							// 輸出空間
		src.length() * MB_CUR_MAX + 1,	// 輸出字串的Bytes數
		src.c_str(),					// 輸入字串
		src.length() * MB_CUR_MAX + 1	// 輸入字串的Bytes數
		);

	// 將 mbs 指定至 dest
	dest = mbs;

	// 刪除暫存空間
	delete[] mbs;
}

/**
*
* @brief		寬字元字串轉窄字元字串(std::string -> std::wstring)
*
* @param[in]	src		窄字元字串
*
* @param[out]	dest	寬字元字串
*
* @return		沒有
*
* @note
*
* @date			2014/06/20
*/
void str2wstr(const std::string &src, std::wstring &dest)
{
	// 輸出用字串暫存陣列
	wchar_t *wcs = new wchar_t[src.length() + 1];

	// 輸出字元數
	size_t converted = 0;

	// 將 src 轉換至 wcs
	mbstowcs_s(
		&converted,			// 被轉換的字元數
		wcs,				// 輸出空間
		src.length() + 1,	// 輸出字串的Bytes數
		src.c_str(), 		// 輸入字串
		src.length() + 1	// 輸入字串的Bytes數
		);

	// 將 wcs 指定至 dest
	dest = wcs;

	// 刪除暫存空間
	delete[] wcs;
}

/**
*
* @brief		字串取代
*
* @param[in]	subject		要取代之字串
* @param[in]	search		搜尋目標
* @param[in]	replace		取代結果
*
* @param[out]	subject		被取代之字串
*
* @return		沒有
*
* @note			直接改變字串本體
*
* @date			2014/06/20
*/
void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace)
{
	// 位置
	size_t pos = 0;
	
	// 尋找每一個目標到字串結束
	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		// 將 serach 取代為 replace
		subject.replace(pos, search.length(), replace);
		
		// 往下一個位置移動
		pos += replace.length();
	}

	// 結束回傳
	return;
}

/**
*
* @brief		字串取代
*
* @param[in]	subject		要取代之字串
* @param[in]	search		搜尋目標
* @param[in]	replace		取代結果
*
* @return		subject		被取代之字串
*
* @note			不改變字串本體，改由回傳一個新的字串
*
* @date			2014/06/20
*/
std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace)
{
	// 位置
	size_t pos = 0;

	// 尋找每一個目標到字串結束
	while ((pos = subject.find(search, pos)) != std::string::npos) 
	{
		// 將 serach 取代為 replace
		subject.replace(pos, search.length(), replace);

		// 往下一個位置移動
		pos += replace.length();
	}

	// 回傳取代後的字串
	return subject;
}


//////////////////////////////////////////////////
//////////        子執行續函數          //////////
//////////////////////////////////////////////////


/**
*
* @brief		工作執行緒
*
* @param[in]	param		工作執行緒所需參數
*
* @param[out]	沒有
*
* @return		0以外則失敗
*
* @note
*
* @date			2014/06/20
*/
UINT workThread(LPVOID param)
{
	// 取得參數
	// 指定至 CSteelBarASDlg 物件
	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;

	// 將 UI 上的主進度條初始化
	// 範圍 0 到 1000
	dlg->m_progressCtrlGlobal.SetRange32(0, 1000);

	// 新增顏色選項為紅色
	COLORREF clrBar = RGB(255, 0, 0);

	// 設定主進度條的顏色為 clrBar
	// 此動作在 xp 主題下無作用
	dlg->m_progressCtrlGlobal.SetBarColor(clrBar);


	///- 執行 UI 變數存取 -///
	// 將 UI 上的參數設定至變數
	dlg->SetParameterFromUi();

	// 等待中斷訊息
	DWORD dwRet = WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);

	// 如果有中斷訊息傳入則中斷
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		// 輸出中斷訊息
		dlg->PostMessageToLog("Work thread was killed at \"set parameter\" by user.");

		// 將中斷旗標設回 FALSE 狀態
		dlg->parameter.terminateThread = FALSE;

		// 回傳不正常結束
		return 1;
	}

	// 將主進度條狀態設定為 100 / 1000
	dlg->m_progressCtrlGlobal.SetPos(100);


	///- 相機校正 -//
	// 執行相機校正
	dlg->CalibrateImage();

	// 等待中斷訊息
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);

	// 如果有中斷訊息傳入則中斷
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		// 輸出中斷訊息
		dlg->PostMessageToLog("Work thread was killed at \"calibrate image\" by user.");

		// 將中斷旗標設回 FALSE 狀態
		dlg->parameter.terminateThread = FALSE;

		// 回傳不正常結束
		return 1;
	}

	// 將主進度條狀態設定為 300 / 1000
	dlg->m_progressCtrlGlobal.SetPos(300);


	///- 產生物件點 -///
	// 執行產生物件點
	dlg->GenerateObjectPoints();

	// // 等待中斷訊息
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);

	// 如果有中斷訊息傳入則中斷
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		// 輸出中斷訊息
		dlg->PostMessageToLog("Work thread was killed at \"generate object points\" by user.");

		// 將中斷旗標設回 FALSE 狀態
		dlg->parameter.terminateThread = FALSE;

		// 回傳不正常結束
		return 1;
	}

	// 將主進度條狀態設定為 400 / 1000
	dlg->m_progressCtrlGlobal.SetPos(400);


	///- 產生影像座標點 -///
	// 執行產生影像座標點
	dlg->GenerateImagePoints();

	// 等待中斷訊息
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);

	// 如果有中斷訊息傳入則中斷
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		// 輸出中斷訊息
		dlg->PostMessageToLog("Work thread was killed at \"generate image points\" by user.");

		// 將中斷旗標設回 FALSE 狀態
		dlg->parameter.terminateThread = FALSE;

		// 回傳不正常結束
		return 1;
	}

	// 將主進度條狀態設定為 500 / 1000
	dlg->m_progressCtrlGlobal.SetPos(500);


	///- 產生正射影像 -///
	// 執行產生正射影像
	dlg->GenerateRectifyImage();

	// 等待中斷訊息
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);

	// 如果有中斷訊息傳入則中斷
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		// 輸出中斷訊息
		dlg->PostMessageToLog("Work thread was killed at \"generate rectify images\" by user.");

		// 將中斷旗標設回 FALSE 狀態
		dlg->parameter.terminateThread = FALSE;

		// 回傳不正常結束
		return 1;
	}

	// 將主進度條狀態設定為 600 / 1000
	dlg->m_progressCtrlGlobal.SetPos(600);

	///- 計算 -///
	// 執行計算結果
	dlg->Calculate();

	// 等待中斷訊息
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);

	// 如果有中斷訊息傳入則中斷
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		// 輸出中斷訊息
		dlg->PostMessageToLog("Work thread was killed at \"calculate\" by user.");

		// 將中斷旗標設回 FALSE 狀態
		dlg->parameter.terminateThread = FALSE;

		// 回傳不正常結束
		return 1;
	}

	// 將主進度條狀態設定為 875 / 1000
	dlg->m_progressCtrlGlobal.SetPos(875);

	///- 輸出計算結果 -///
	// 執行輸出結果
	dlg->WriteCalcData();

	// 等待中斷訊息
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);

	// 如果有中斷訊息傳入則中斷
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		// 輸出中斷訊息
		dlg->PostMessageToLog("Work thread was killed at \"write calculation data\" by user.");

		// 將中斷旗標設回 FALSE 狀態
		dlg->parameter.terminateThread = FALSE;

		// 回傳不正常結束
		return 1;
	}

	// 將主進度條狀態設定為 1000 / 1000
	dlg->m_progressCtrlGlobal.SetPos(1000);

	// clrBar 指定為 綠色
	clrBar = RGB(0, 255, 0);

	// 將進度條顏色改變為 clrBar
	// 在 xp 主題下沒有作用
	dlg->m_progressCtrlGlobal.SetBarColor(clrBar);

	// 結束回傳正常
	return 0;
}

/**
*
* @brief		訊息執行緒
*
* @param[in]	param		工作執行緒所需參數
*
* @param[out]	沒有
*
* @return		0以外則失敗
*
* @note
*
* @date			2014/06/20
*/
UINT msgThread(LPVOID param)
{
	// 取得參數
	// 指定至 CSteelBarASDlg 物件
	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;

	// 執行輸出訊息
	dlg->message();

	// 結束回傳
	return 0;
}

/**
*
* @brief		檢查工作執行緒
*
* @param[in]	param		工作執行緒所需參數
*
* @param[out]	沒有
*
* @return		0以外則失敗
*
* @note
*
* @date			2014/06/20
*/
UINT checkWorkThread(LPVOID param)
{
	// 取得參數
	// 指定至 CSteelBarASDlg 物件
	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;

	// 無限等待直到執行緒完成工作
	::WaitForSingleObject(dlg->parameter.m_pThread->m_hThread, INFINITE);

	// 中斷執行緒
	//dlg->parameter.m_pThread->ExitInstance();

	// 釋放執行緒指標
	dlg->parameter.m_pThread = NULL;

	// 不開放 UI 上的 Run 按鈕
	dlg->m_buttonRun.EnableWindow(FALSE);

	// 不開放 UI 上的 Terminate 按鈕
	dlg->m_buttonStop.EnableWindow(FALSE);

	// 開放 UI 上的 Clear 按鈕
	dlg->m_buttonClear.EnableWindow(TRUE);

	// 開放 UI 上的 選擇校正影像路徑 按鈕
	dlg->m_buttonCalibImgSelPath.EnableWindow(TRUE);

	// 開放 UI 上的 選擇實驗影像路徑 按鈕
	dlg->m_buttonExpImgSelPath.EnableWindow(TRUE);

	// 開放 UI 上的 選擇輸出路徑 按鈕
	dlg->m_buttonSelOutputPath.EnableWindow(TRUE);

	// 開放 UI 上的 選擇外部校正資料 按鈕
	dlg->m_buttonLoadExtCalibData.EnableWindow(TRUE);

	// 開放 UI 上的 選擇校正基準影像 按鈕
	dlg->m_listCalibImagePath.EnableWindow(TRUE);

	// 開放 UI 上的 記憶體精簡模式 選項
	dlg->m_checkBoxMemoryLite.EnableWindow(TRUE);

	// 輸出完成訊息
	dlg->PostMessageToLog("Done!");

	// 結束回傳
	return 0;
}

/**
*
* @brief		中斷工作執行緒
*
* @param[in]	param		工作執行緒所需參數
*
* @param[out]	沒有
*
* @return		0以外則失敗
*
* @note
*
* @date			2014/06/20
*/
UINT terminateWorkThread(LPVOID param)
{
	// 取得參數
	// 指定至 CSteelBarASDlg 物件
	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;

	// 執行緒狀態訊息變數
	DWORD threadInfo;

	// 無限迴圈
	// 等待到工作執行緒結束或被中斷為止
	while (true)
	{
		// 取得 dlg->parameter.m_pThread 工作執行緒的狀態
		// 回傳狀態結果存入 threadInfo
		::GetExitCodeThread(dlg->parameter.m_pThread->m_hThread, &threadInfo);

		// 如果工作執行緒的狀態不是還活著的話則執行
		if (threadInfo != STILL_ACTIVE)
		{
			// 結束執行緒
			dlg->parameter.m_pThread->ExitInstance();

			// 釋放執行緒
			dlg->parameter.m_pThread = NULL;

			// 輸出中斷訊息
			dlg->PostMessageToLog("work thread end");

			// 結束
			break;
		}

		// 減緩消耗效能
		// 此執行緒休息 0.1 秒
		Sleep(100);
	}

	// 結束回傳
	return 0;
}

/**
*
* @brief		檢查 Ui 上的參數 執行緒
*
* @param[in]	param		工作執行緒所需參數
*
* @param[out]	沒有
*
* @return		0以外則失敗
*
* @note
*
* @date			2014/09/09s
*/
UINT CheckUiParameterThread(LPVOID param)
{
	// 取得參數
	// 指定至 CSteelBarASDlg 物件
	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;

	// 執行 檢查 參數
	dlg->CheckUiParameter();

	// 結束回傳
	return 0;
}


//////////////////////////////////////////////////
//////////      CSteelBar 成員函數      //////////
//////////////////////////////////////////////////


void CSteelBarASDlg::Init()
{
	// 數值初始化
	ValueInit();

	// 變數初始化
	VariableInit();

	// 分析方法初始化
	AnalysisMethodInit();

	// 啟動 檢查參數執行續 2014/09/13
	this->parameter.m_pCheckUiParameterThread = AfxBeginThread(CheckUiParameterThread, (LPVOID)this, NULL);

	// 產生輸出訊息
	this->msgAndLog += "Started successfully\r\r\n";

	// 將輸出訊息輸出至 UI 上的訊息框
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);

	// 將訊息框捲動至最底下
	MessageBoxAutoScrollToBottom();

	// 結束回傳
	return;
}

void CSteelBarASDlg::ValueInit()
{
	// 將 UI 上的 "V Template Size" 的 X 值設定為 40
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_V_TEMPLATE_SIZE_X))->SetWindowTextW(L"40");
	
	// 將 UI 上的 "V Template Size" 的 Y 值設定為 40
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_V_TEMPLATE_SIZE_Y))->SetWindowTextW(L"40");
	
	// 將 UI 上的 "H Template Size" 的 X 值設定為 210
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_H_TEMPLATE_SIZE_X))->SetWindowTextW(L"210");
	
	// 將 UI 上的 "H Template Size" 的 Y 值設定為 40
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_H_TEMPLATE_SIZE_Y))->SetWindowTextW(L"40");
	
	// 將 UI 上的 "Chessboard Size" 的 X 值設定為 5.0
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_CHESSBOARD_SIZE_X))->SetWindowTextW(L"5.0");
	
	// 將 UI 上的 "Chessboard Size" 的 Y 值設定為 5.0
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_CHESSBOARD_SIZE_Y))->SetWindowTextW(L"5.0");
	
	// 將 UI 上的 "Pattern Size" 的 X 值設定為 7
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_PATTERN_SIZE_X))->SetWindowTextW(L"7");
	
	// 將 UI 上的 "Pattern Size" 的 Y 值設定為 25
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_PATTERN_SIZE_Y))->SetWindowTextW(L"25");
	
	// 將 UI 上的 "Number of V Template " 的值設定為 2
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_NUMBER_OF_V_TEMPLATE))->SetWindowTextW(L"2");
	
	// 將 UI 上的 "Number of H Template " 的值設定為 100
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_NUMBER_OF_H_TEMPLATE))->SetWindowTextW(L"100");
	
	// 將 UI 上的 "Diameter" 的值設定為 12.5
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_DIAMETER))->SetWindowTextW(L"12.5");
	
	// 將 UI 上的 壓縮選項 設定為 PNG
	((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_PNG))->SetCheck(TRUE);

	// 設定 壓縮率 滑桿 的 上下限
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BAR_IMAGE_COMPRESS_LEVEL))->SetRange(0, 10);
	
	// 設定 壓縮率 滑桿的 位置
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BAR_IMAGE_COMPRESS_LEVEL))->SetPos(3);

	// 結束回傳
	return;
}


// 定義 template image 輸出資料夾名稱
#define L_TEMPLATE_FOLDER_NAME L"template"

// 定義 control points 輸出資料夾名稱
#define L_CTRLPOINTS_FOLDER_NAME L"ctrlPoints"

// 定義 cheaaboard image 輸出資料夾名稱
#define L_CHESSBOARDIMAGE_FOLDER_NAME L"chessboardImage"

// 定義 rectify image 輸出資料夾名稱
#define L_RECTIFY_FOLDER_NAME L"rectify"

// 定義 trace image 輸出資料夾名稱
#define L_TRACE_FOLDER_NAME L"trace"


void CSteelBarASDlg::DirectoryInit()
{
	// 檢查輸出路徑是否有設定，如果沒有設定就結束
	// 沒有設定則路徑為 ""
	if (this->parameter.outputPath == "")
	{
		// 結束回傳
		return;
	}

	// 將窄字元轉換為寬字元(std::stirng -> std::wstring)
	// stemp 為轉換用的暫存變數
	std::wstring stemp = std::wstring(this->parameter.outputPath.begin(), this->parameter.outputPath.end());
	
	// 將寬字元轉換為 LPCWSTR
	LPCWSTR sw = stemp.c_str();

	// 切換路徑至指定輸出路徑下，即 sw 或 this->parameter.output
	SetCurrentDirectory(sw);

	// 安全屬性結構
	LPSECURITY_ATTRIBUTES lpSecurity;

	// 將安全屬性設為預設，與主執行緒有同樣的安全性。
	// 除自身之外都沒有權限
	lpSecurity = NULL;

	// 使用 CreateDirectory() 創建 template image 資料夾並判斷是否成功
	if (CreateDirectory(L_TEMPLATE_FOLDER_NAME, lpSecurity) == FALSE)
	{
		// 如果失敗的原因不是因為資料夾已經存在，則輸出錯誤訊息
		if (!(GetLastError() == ERROR_ALREADY_EXISTS)) 
		{
			// 輸出彈出式訊息，警告無法產生資料夾
			MessageBoxA(NULL, "can not create template folder.", "Warning!", NULL);
		}
	}


	// 安全屬性結構
	LPSECURITY_ATTRIBUTES lpSecurity2;

	// 將安全屬性設為預設，與主執行緒有同樣的安全性。
	lpSecurity2 = NULL;

	// 使用 CreateDirectory() 創建 control points 資料夾並判斷是否成功
	if (CreateDirectory(L_CTRLPOINTS_FOLDER_NAME, lpSecurity2) == FALSE)
	{
		// 如果失敗的原因不是因為資料夾已經存在，則輸出錯誤訊息
		if (!(GetLastError() == ERROR_ALREADY_EXISTS))
		{
			// 輸出彈出式訊息，警告無法產生資料夾
			MessageBoxA(NULL, "can not create ctrlPoints folder.", "Warning!", NULL); 
		}
	}


	// 安全屬性結構
	LPSECURITY_ATTRIBUTES lpSecurity3;

	// 將安全屬性設為預設，與主執行緒有同樣的安全性。
	lpSecurity3 = NULL;

	// 使用 CreateDirectory() 創建 chessboard image 資料夾並判斷是否成功
	if (CreateDirectory(L_CHESSBOARDIMAGE_FOLDER_NAME, lpSecurity3) == FALSE)
	{
		// 如果失敗的原因不是因為資料夾已經存在，則輸出錯誤訊息
		if (!(GetLastError() == ERROR_ALREADY_EXISTS))
		{
			// 輸出彈出式訊息，警告無法產生資料夾
			MessageBoxA(NULL, "can not create chessboardImage folder.", "Warning!", NULL); 
		}
	}


	// 安全屬性結構
	LPSECURITY_ATTRIBUTES lpSecurity4;

	// 將安全屬性設為預設，與主執行緒有同樣的安全性。
	lpSecurity4 = NULL;

	// 使用 CreateDirectory() 創建 rectify image 資料夾並判斷是否成功
	if (CreateDirectory(L_RECTIFY_FOLDER_NAME, lpSecurity4) == FALSE)
	{
		// 如果失敗的原因不是因為資料夾已經存在，則輸出錯誤訊息
		if (!(GetLastError() == ERROR_ALREADY_EXISTS))
		{
			// 輸出彈出式訊息，警告無法產生資料夾
			MessageBoxA(NULL, "can not create rectf folder.", "Warning!", NULL); 
		}
	}


	// 安全屬性結構
	LPSECURITY_ATTRIBUTES lpSecurity5;

	// 將安全屬性設為預設，與主執行緒有同樣的安全性。
	lpSecurity5 = NULL;

	// 使用 CreateDirectory() 創建 rectify image 資料夾並判斷是否成功
	if (CreateDirectory(L_TRACE_FOLDER_NAME, lpSecurity5) == FALSE)
	{
		// 如果失敗的原因不是因為資料夾已經存在，則輸出錯誤訊息
		if (!(GetLastError() == ERROR_ALREADY_EXISTS))
		{
			// 輸出彈出式訊息，警告無法產生資料夾
			MessageBoxA(NULL, "can not create trace folder.", "Warning!", NULL); 
		}
	}


	// 結束回傳
	return;
}

void CSteelBarASDlg::VariableInit()
{
	// 將訊息變數設定為空 ""
	this->msgAndLog = "";

	// 初始化相機校正物件
	this->camCalib;

	// 設定記憶體精簡選項為 FALSE
	this->parameter.memoryLite = FALSE;

	// 設定輸出訊息選項為 FALSE
	this->parameter.mute = FALSE;

	// 設定外部匯入校正資料為 FALSE
	this->parameter.calibDataImprotFromExternal = FALSE;

	// 結束回傳
	return;
}

int CSteelBarASDlg::SelectPath(int pathItemId, int ListItemId, bool calibOrExp)
{
	// 設定選擇檔案用的過濾器
	// 檔案種類為 JPEG Files 時，接受副檔名為 *.jpg, *.jpeg
	// 檔案種類為 PNG Files 時，接受副檔名為 *.png
	// 檔案種類為 BITMAP Files 時，接受副檔名為 *.bmp
	// 檔案種類為 TIFF Files 時，接受副檔名為 *.tif
	CString filter("JPEG Files (*.jpg, *.jpeg)|*.jpg; *.jpeg|PNG Files (*.png)|*.png|BITMAP Files (*.bmp)|*.bmp|TIFF Files (*.tif)|*.tif||");

	// 檔案路徑
	CString filePath;

	// 檔案路徑暫存變數
	CString strBuf;

	// 檔案位置
	POSITION pos = NULL;

	// 新增一個檔案對話框
	// OFN_HIDEREADONLY -> 不顯示只能讀取的文件
	// OFN->ALLOWMULTISELECT -> 支援選擇多個檔案功能
	// 套用副檔名過濾器 filter
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, filter);

	// 錯誤訊息
	int err = 0;

	// 錯誤訊息
	int lbErr = 0;

	// 如果沒有錯誤，則執行
	if (!err)
	{
		// 嘗試:確保檔名清單用的記憶體
		try
		{
			// 添加暫存空間到 OPENFILENAME 成員裡面
			// 檔案名最大字數
			selDlg.GetOFN().lpstrFile = strBuf.GetBuffer(MAX_PATH * MAX_PATH * 100);
			
			// 最大檔案數量
			selDlg.GetOFN().nMaxFile = MAX_PATH * MAX_PATH * 100;
		}
		// 捕捉例外
		// 如果發生預期外的例外則接住
		catch (...)
		{
			// 無論發生任何例外皆回傳錯誤
			err = 1;
		}
	}

	// 如果沒有錯誤，則執行
	if (!err)
	{
		// 檢查有沒有按下 OK 確認按鈕
		// 如果沒有則執行
		if (selDlg.DoModal() != IDOK)
		{
			// 指定為發生錯誤
			err = 1; 
		}
	}

	// 如果沒有錯誤，則執行
	if (!err)
	{
		// 檢查是否有取得檔案
		// 如果沒有取得則 GetStartPosition() 回傳為 NULL
		if ((pos = selDlg.GetStartPosition()) == NULL) 
		{
			// 指定為發生錯誤
			err = 1; 
		}
	}

	// 如果沒有錯誤，則執行
	if (!err)
	{
		// 如果是執行選擇校正檔案則為 TRUE
		if (calibOrExp)
		{
			// 初始化存放校正照片檔名的變數空間
			this->parameter.calibImageFileName.clear();
		}
		// 如果是執行選擇實驗檔案則為 FALSE
		else
		{
			// 初始化存放實驗照片檔名的變數空間
			this->parameter.expImageFileName.clear();
		}

		// 清空 ListItemId 清單內的數值
		((CListBox*)GetDlgItem(ListItemId))->ResetContent();

		// 輸出選擇的路徑 selDlg.GetPathName() 至 pathItemId 的訊息框
		((CEdit*)GetDlgItem(pathItemId))->SetWindowTextW(selDlg.GetPathName());

		// 從對話框取得路徑型態為 CString
		// 將 CString 轉換為 std::string
		std::string tmpPathName = (CT2CA)selDlg.GetPathName();
		
		// 判斷為選擇校正影像檔案還是實驗影像檔案
		// 如果是選擇校正影像則為 TRUE
		if (calibOrExp) 
		{
			// 將校正影像路徑指定為 tmpPathName
			this->parameter.calibImagePath = tmpPathName; 
		}
		// 如果是選擇實驗影像則為 FALSE
		else
		{
			// 將實驗影像路徑指定為 tmpPathName
			this->parameter.expImagePath = tmpPathName; 
		}

		// 用迴圈將多選的所有檔案分別存入
		// pos 為檔案位置，到達最後時，pos 會變成 NULL 跳出迴圈
		while (pos)
		{
			// 用 GetNextPathName() 取得檔案名
			// pos 會每次 + 1 直到最後則為 NULL
			// 將取得的檔案名存入 filePath
			// filePath 是包含完整路徑的變數 EX: C:\\foo\bar\baz\qux\quux.ext
			filePath = selDlg.GetNextPathName(pos);

			// 如果沒有錯誤則執行
			if (!err)
			{
				// 將 CString 型態的 filePath 轉為 std::string 的 tmpName
				// 用來將檔名存放至檔案名變數 this->parameter.calibImageFileName 內
				std::string tmpName((CT2CA)filePath);

				// 判斷為選擇校正影像檔案還是實驗影像檔案
				// 如果是選擇校正影像則為 TRUE
				if (calibOrExp)
				{
					// 將 tmpName 存入校正影像檔案名變數內
					this->parameter.calibImageFileName.push_back(tmpName); 
				}
				// 如果是選擇實驗影像則為 FALSE
				else
				{
					// 將 tmpName 存入實驗影像檔案名變數內
					this->parameter.expImageFileName.push_back(tmpName); 
				}

				// 從 filePath 中將不含路徑的檔案名單獨取出，並存至 filename 中
				// filePath.Right() 取得右邊的字串
				// 以 GetLength() 取得字串全部長度，再用 ReverseFind() 尋找最後一個 \\ 符號
				// 兩個相減後就是不包含路徑的完整檔案名
				CString filename = filePath.Right(filePath.GetLength() - filePath.ReverseFind(_T('\\')) - 1);

				// 將不包含路徑的完整檔案名增加至在UI上所指定的清單列表
				lbErr = ((CListBox*)GetDlgItem(ListItemId))->InsertString(-1, filename);

				// 將 CString 的檔案名轉換為 std::string 的tmpName2
				// 用來存放不包含路徑的完整檔名
				std::string tmpName2 = (CT2CA)filename;

				// 如果是實驗影像檔名才執行
				if (!calibOrExp)
				{
					// 將檔案名存入變數內
					this->parameter.expImageFileNameWithoutPath.push_back(tmpName2);
				}

				// 判斷插入字串有沒有成功
				// lbErr == LB_ERR 則表示錯誤
				// lbErr == LB_ERRSPACE 則表示沒有足夠的空間可供儲存新的字串
				if (lbErr == LB_ERR || lbErr == LB_ERRSPACE)
				{
					// 指定為發生錯誤 
					err = 1; 
				}
			}

			// 如果發生錯誤則中斷
			if (err) 
			{
				// 跳出
				break; 
			}
		}

		// 更新變數至UI
		UpdateData(FALSE);
	}

	// 釋放檔案路徑空間
	strBuf.ReleaseBuffer();

	// 回傳錯誤訊息
	return err;
}

void CSteelBarASDlg::SelectFolder()
{
	// 瀏覽資訊變數指標
	BROWSEINFO bi;

	// 初始化 bi
	ZeroMemory(&bi, sizeof(bi));

	// 資料夾名稱
	// 因為 pszDisplayName 只接受最大 MAX_PATH的大小
	// 所以 szDisplayName 大小設為 MAX_PATH
	TCHAR   szDisplayName[MAX_PATH];

	// 初始化
	szDisplayName[0] = L'';

	
	///- bi 結構的設定 -///
	// window handle
	// NULL 指定為 CSteelBarASDlg
	bi.hwndOwner = NULL;

	// root資料夾
	// NULL 代表初始預設位置應為 desktop
	bi.pidlRoot = NULL;

	// 資料夾名稱
	// 使用者所選的資料夾名稱
	// 不包含完整路徑
	bi.pszDisplayName = szDisplayName;

	// 選擇路徑的對話盒的標題
	bi.lpszTitle = _T("Please select a folder for output files :");

	// 動作 flags
	// BIF_RETURNONLYFSDIRS 為只回傳檔案系統的路徑
	// 也就是只能選擇資料夾
	bi.ulFlags = BIF_RETURNONLYFSDIRS;

	// 給 lpfn 用的參數
	// NULL 預設為選擇的資料夾
	bi.lParam = NULL;

	// 使用者所選的資料夾的圖示
	// 因為沒有需要所以設為 0
	bi.iImage = 0;

	// PIDL 取得
	// 使用 bi 參數結構並開啟瀏覽資料夾對話盒
	LPITEMIDLIST   pidl = SHBrowseForFolder(&bi);

	// 如果有取得結果則執行
	if (NULL != pidl)
	{
		// 從 pidl 取得路徑名稱
		// bRet判斷取得是否成功
		BOOL bRet = SHGetPathFromIDList(pidl, this->szPathName);

		// 如果無法取得檔案名稱則結束
		if (FALSE == bRet)
		{
			// 結束回傳
			return;
		}
	}

	// 結束回傳
	return;
}

void CSteelBarASDlg::OnBnClickedButtonCalibImageSelectPath()
{
	// 選擇校正影像的資料夾
	int err = SelectPath(IDC_EDIT_TEXT_CALIB_IMAGE_PATH, IDC_LIST_BOX_CALIB_IMAGE_PATH, TRUE);

	///- 輸出訊息 -///
	// 如果選擇完成後，校正影像的數量 > 0 
	// 而且選擇檔案時沒有發生錯誤
	// 則執行
	if ((this->parameter.calibImageFileName.size() > 0) && !err) 
	{
		// 輸出選擇完成訊息
		this->msgAndLog += "selected calibration images\r\r\n"; 
	}
	// 如果檔案數量 < 0
	// 或是選擇檔案時有錯誤發生
	// 則輸出錯誤訊息
	else
	{ 
		// 輸出選擇錯誤訊息
		this->msgAndLog += "No calibration image selected\r\r\n"; 
	}

	// 將訊息顯示到 UI 上的訊息視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
	
	// 將訊息視窗捲動到最底下
	MessageBoxAutoScrollToBottom();

	// 更新 UI 顯示資訊
	UpdateData(FALSE);

	// 計數器:計算檔案數量
	// 從 UI 上的 校正影像清單中計算校正影像的數量
	int count = ((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->GetCount();

	// 如果影像數量 > 0
	// 則忽略外部匯入校正資料
	if (count > 0)
	{
		// 將外部匯入校正資料旗標設為 FALSE
		this->parameter.calibDataImprotFromExternal = false; 
	}

	// 結束回傳
	return;
}

void CSteelBarASDlg::OnBnClickedButtonExpImageSelectPath()
{
	// 選擇實驗影像的資料夾
	int err = SelectPath(IDC_EDIT_TEXT_EXP_IMAGE_PATH, IDC_LIST_BOX_EXP_IMAGE_PATH, FALSE);

	///- 輸出訊息 -///
	// 如果選擇完成後，實驗影像的數量 > 0 
	// 而且選擇檔案時沒有發生錯誤
	// 則執行
	if ((this->parameter.expImageFileName.size() > 0) && !err)
	{
		// 輸出選擇完成訊息
		this->msgAndLog += "selected exp images\r\r\n";
	}
	// 如果檔案數量 < 0
	// 或是選擇檔案時有錯誤發生
	// 則輸出錯誤訊息
	else
	{
		// 輸出選擇錯誤訊息
		this->msgAndLog += "No exp image selected\r\r\n"; 
	}

	// 將訊息顯示到 UI 上的訊息視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);

	// 將訊息視窗捲動到最底下
	MessageBoxAutoScrollToBottom();

	// 更新 UI 顯示資訊
	UpdateData(FALSE);

	// 結束回傳
	return;
}

void CSteelBarASDlg::OnBnClickedButtonRun()
{
	// 實驗影像數量
	// 從 UI 上的實驗影像檔名清單中計算檔案數量
	int expCount = ((CListBox*)GetDlgItem(IDC_LIST_BOX_EXP_IMAGE_PATH))->GetCount();

	// 校正影像數量
	// 從 UI 上的校正影像檔名清單中計算檔案數量
	int calibCount = ((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->GetCount();

	// 外部相機校正參數檔案名
	CString extCalibFile;

	// 從 UI 上取回外部校正參數檔案名
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_LOAD_EXTERNAL_CALIB_DATA))->GetWindowTextW(extCalibFile);

	// 輸出路徑資料夾名
	CString outpath;

	// 從 UI 上取回輸出資料夾路徑
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_OUTPUT_PATH))->GetWindowTextW(outpath);

	///- 基本參數設定檢查 -///
	// 檢查實驗影像數量
	// 如果實驗影像數量 <= 0 則代表沒有任何影像
	if (expCount <= 0)
	{
		// 輸出錯誤訊息至 UI 上的訊息對話框
		this->PostMessageToLog("No exp data.");

		// 結束回傳
		return;
	}

	// 檢查校正檔案
	// 先判斷是否有外部校正參數檔案
	// 再判斷是否有校正影像檔案
	// 外部校正檔案的權限高於校正影像檔案
	// 所以當兩者都存在的時候則以外部校正影像參數檔案為優先
	// 判斷外部校正影像參數檔案名是否存在
	// "" 為不存在，即沒有外部檔案
	if (extCalibFile == L"")
	{
		// 檢查是否有校正影像
		// 如果沒有則輸出錯誤訊息
		if (calibCount <= 0)
		{
			// 輸出錯誤訊息至 UI 上的訊息對話框
			this->PostMessageToLog("No calib data.");

			// 結束回傳
			return;
		}
	}

	// 檢查輸出路徑是否存在
	// "" 為沒有，即不存在
	if (outpath == L"")
	{
		// 輸出錯誤訊息至 UI 上的訊息對話框
		this->PostMessageToLog("No output path");

		// 結束回傳
		return;
	}

	///- 執行主要工作 -///
	// 嘗試:開啟工作執行緒執行工作
	try
	{
		// 將 this->parameter.m_pThread 指向 workThread
		// 傳入整個目前的 CSteelBarASDlg 物件當作參數
		this->parameter.m_pThread = AfxBeginThread(workThread, (LPVOID)this, NULL);

		// 創建一個事件用於追蹤工作執行緒
		// 並將 this->parameter.m_hKillEvent 指向此事件
		this->parameter.m_hKillEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		// 開啟另外一個執行緒
		// 此執行緒會不斷的檢查工作執行緒 workThread 是否已經結束
		// 將 this->parameter.m_pCheckWorkThread 指向 checkWorkThread
		// 傳入整個目前的 CSteelBarASDlg 物件當作參數
		this->parameter.m_pCheckWorkThread = AfxBeginThread(checkWorkThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	}
	// 捕捉例外
	// 如果發生 c++ 標準以外的例外則接住
	catch (std::exception& ex)
	{
		// 例外訊息用變數
		CString exception;

		// 組合訊息字串
		exception = "Exception: ";

		// 取得例外訊息
		// 存入 exception 變數
		exception += ex.what();

		// 將例外訊息輸出至 UI 上的訊息對話框
		this->PostMessageToLog(exception);

		// 回傳結束
		return;
	}
	// 捕捉例外
	// 如果發生其他無法預期的例外則接住
	catch (...)
	{
		// 輸出例外訊息至 UI 上的訊息對話框
		this->PostMessageToLog("An unknown error has occurred.");

		// 回傳結束
		return;
	}

	// 回傳結束
	return;
}

void CSteelBarASDlg::SetParameterFromUi()
{
	// 存取 UI 上的數值用的變數
	CString cstr;

	// 輸出文字狀態至副進度條上
	// 此功能在 xp 主題下沒有作用
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"setting parameter");

	// 設定副進度條的範圍為 0 到 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 10);
	
	// 設定副進度條的狀態為 0 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);

	// 從 UI 上取回 "V Template Size X"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_V_TEMPLATE_SIZE_X))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttoi() 轉為整數並存入 this->parameter.vTemplateSizeX
	this->parameter.vTemplateSizeX = _ttoi(cstr);

	// 從 UI 上取回 "V Template Size Y"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_V_TEMPLATE_SIZE_Y))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttoi() 轉為整數並存入 this->parameter.vTemplateSizeY
	this->parameter.vTemplateSizeY = _ttoi(cstr);

	// 從 UI 上取回 "H Template Size X"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_H_TEMPLATE_SIZE_X))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttoi() 轉為整數並存入 this->parameter.hTemplateSizeX
	this->parameter.hTemplateSizeX = _ttoi(cstr);

	// 從 UI 上取回 "H Template Size Y"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_H_TEMPLATE_SIZE_Y))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttoi() 轉為整數並存入 this->parameter.hTemplateSizeY
	this->parameter.hTemplateSizeY = _ttoi(cstr);

	// 從 UI 上取回 "Chessboard Size X"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_CHESSBOARD_SIZE_X))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttof() 轉為浮點數並存入 this->parameter.cellSizeDx
	this->parameter.cellSizeDx = static_cast<float>(_ttof(cstr));

	// 從 UI 上取回 "Chessboard Size Y"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_CHESSBOARD_SIZE_Y))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttof() 轉為浮點數並存入 this->parameter.cellSizeDy
	this->parameter.cellSizeDy = static_cast<float>(_ttof(cstr));

	// 從 UI 上取回 "Pattern Size X"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_PATTERN_SIZE_X))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttoi() 轉為整數並存入 this->parameter.patternSizeNx
	this->parameter.patternSizeNx = _ttoi(cstr);

	// 從 UI 上取回 "Pattern Size Y"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_PATTERN_SIZE_Y))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttoi() 轉為整數並存入 this->parameter.patternSizeNy
	this->parameter.patternSizeNy = _ttoi(cstr);

	// 從 UI 上取回 "Number of V Template"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_NUMBER_OF_V_TEMPLATE))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttoi() 轉為整數並存入 this->parameter.nVtemplate
	this->parameter.nVtemplate = _ttoi(cstr);

	// 從 UI 上取回 "Number of H Template"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_NUMBER_OF_H_TEMPLATE))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttoi() 轉為整數並存入 this->parameter.nHtemplate
	this->parameter.nHtemplate = _ttoi(cstr);

	// 從 UI 上取回 "Diameter"，並存入 cstr
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_DIAMETER))->GetWindowTextW(cstr);

	// 將取回的值經由 _ttof() 轉為浮點數並存入 this->parameter.diameter
	this->parameter.diameter = static_cast<float>(_ttof(cstr));

	// 設定副進度條的狀態為 4 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(4);

	// 檢查是否勾選記憶體精簡化選項
	// 確認精簡化記憶體選項狀態，並存入 nMemoryLiteCheck
	int nMemoryLiteCheck = ((CButton*)GetDlgItem(IDC_CHECK_BOX_MEMORY_LITE))->GetState();

	// 判斷記憶體精簡化選項是否被勾選
	// 如果 nMemoryLiteCheck & BST_CHECKED 的結果不等於 0
	// 則表示勾選框有被勾選
	if ((nMemoryLiteCheck & BST_CHECKED) != 0)
	{
		// 設定記憶體精簡化旗標為 TRUE
		this->parameter.memoryLite = TRUE;
	}
	// 如果 nMemoryLiteCheck & BST_CHECKED 的結果等於 0
	// 則表示勾選框沒有被勾選
	else
	{
		// 設定記憶體精簡化旗標為 FALSE
		this->parameter.memoryLite = FALSE;
	}

	// 檢查是否勾選靜音模式選項
	// 確認靜音模式選項狀態，並存入 nMuteCheck
	int nMuteCheck = ((CButton*)GetDlgItem(IDC_CHECK_BOX_MUTE))->GetState();

	// 判斷靜音模式選項是否被勾選
	// 如果 nMuteCheck & BST_CHECKED 的結果不等於 0
	// 則表示勾選框有被勾選
	if ((nMuteCheck & BST_CHECKED) != 0)
	{
		// 設定靜音選項旗標為 TRUE
		this->parameter.mute = TRUE;
	}
	// 如果 nMuteCheck & BST_CHECKED 的結果等於 0
	// 則表示勾選框沒有被勾選
	else
	{
		// 設定靜音選項旗標為 FALSE
		this->parameter.mute = FALSE;
	}

	// 存放校正參數檔案的變數
	CString checkFile;

	// 從 UI 上取回匯入的校正檔案名稱，並存入 checkFile
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_LOAD_EXTERNAL_CALIB_DATA))->GetWindowTextW(checkFile);
	
	// 檢查是否有指定匯入外部校正參數檔案
	// 如果不為 L"" 則表示有指定外部檔案
	if (checkFile != L"")
	{
		// 設定有外部校正檔案旗標為 TRUE
		this->parameter.hasExternalCalibData = true;
	}

	// 設定副進度條的狀態為 8 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(8);

	// 如果靜音模式沒有被開啟
	// 則輸出訊息
	if (!this->parameter.mute)
	{
		// 增加訊息
		this->msgAndLog += "Set all parameter\r\r\n";

		// 將訊息顯示到 UI 上的訊息對話框
		((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);

		// 將訊息對話框滾動至最底下
		MessageBoxAutoScrollToBottom();
	}

	///- 設定 UI 功能 -///
	// 停用 Run 按鈕
	((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(FALSE);
	
	// 啟用 Terminate 按鈕
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(TRUE);

	// 停用 Clear 按鈕
	((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(FALSE);
	
	// 停用選擇校正影像資料夾按鈕
	((CButton*)GetDlgItem(IDC_BUTTON_CALIB_IMAGE_SELECT_PATH))->EnableWindow(FALSE);
	
	// 停用選擇實驗影像資料夾按鈕
	((CButton*)GetDlgItem(IDC_BUTTON_EXP_IMAGE_SELECT_PATH))->EnableWindow(FALSE);
	
	// 停用選擇輸出路徑資料夾按鈕
	((CButton*)GetDlgItem(IDC_BUTTON_SELECT_OUTPUT_PATH))->EnableWindow(FALSE);
	
	// 停用外部匯入校正參數按鈕
	((CButton*)GetDlgItem(IDC_BUTTON_LOAD_EXTERNAL_CALIB_DATA))->EnableWindow(FALSE);

	// 停用校正影像清單功能，使其無法變更正射基準影像
	((CListCtrl*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->EnableWindow(FALSE);

	// 停用 記憶體精簡模式 選項
	((CButton*)GetDlgItem(IDC_CHECK_BOX_MEMORY_LITE))->EnableWindow(FALSE);

	////- 暫時不關閉 壓縮選項 -////
	//// 關閉 PNG 壓縮選項
	//((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_PNG))->EnableWindow(FALSE);

	//// 關閉 JPG 壓縮選項
	//((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_JPG))->EnableWindow(FALSE);

	//// 關閉 NONE 壓縮選項
	//((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_NONE))->EnableWindow(FALSE);

	//// 關閉 壓縮等級 滑桿
	//((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BAR_IMAGE_COMPRESS_LEVEL))->EnableWindow(FALSE);

	//// 關閉 壓縮等級 文字方塊
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_IMAGE_COMPRESS_LEVEL))->EnableWindow(FALSE);

	///- 取得 壓縮方式的選取狀態 -///
	// 取得 PNG 壓縮選項 的 狀態
	int compressTypePNG = ((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_PNG))->GetState();
	
	// 取得 JPG 壓縮選項 的 狀態
	int compressTypeJPG = ((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_JPG))->GetState();
	
	// 取得 NONE 壓縮選項 的 狀態
	int compressTypeNONE = ((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_NONE))->GetState();

	// 如果是 記憶體精簡模式的話
	// 執行 壓縮動作 或 其他
	if (this->parameter.memoryLite)
	{
		// 如果 選擇的是 PNG 壓縮形式
		if ((compressTypePNG & BST_CHECKED) != 0)
		{
			// 指定 壓縮形式 為 PNG
			this->parameter.imageCompressType = cv::IMWRITE_PNG_COMPRESSION;
		}
		// 如果 選擇的是 JPG 壓縮形式
		else if ((compressTypeJPG & BST_CHECKED) != 0)
		{
			// 指定 壓縮形式 為 JPG
			this->parameter.imageCompressType = cv::IMWRITE_JPEG_QUALITY;
		}
		// 如果 選擇的是 NONE
		else if (compressTypeNONE)
		{
			// 指定 壓縮形式 為 -1
			// Limit5: 目前還沒有對 -1 的動作進行定義 2014/09/13
			this->parameter.imageCompressType = -1;
		}
	} // end of if (this->parameter.memoryLite)

	// 初始化 影像壓縮 參數
	this->parameter.imageCompressParameter = std::vector<int>(2);
	
	// 存取 影像壓縮選項 形式
	this->parameter.imageCompressParameter[0] = this->parameter.imageCompressType;
	
	// 存取 影像壓縮選項 等級
	this->parameter.imageCompressParameter[1] = this->parameter.imageCompressLevel;

	// 設定副進度條的狀態為 10 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(10);

	// 結束回傳
	return;
}

void CSteelBarASDlg::CalibrateImage()
{
	// 輸出文字狀態至副進度條上
	// 此功能在 xp 主題下沒有作用
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"calibrating");

	// 設定副進度條的範圍為 0 到 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 100);

	// 設定副進度條的狀態為 0 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);

	// 判斷是否有外部匯入校正參數
	// 如果有，則匯入校正參數並不進行校正
	if (this->parameter.hasExternalCalibData)
	{
		// 匯入校正參數
		this->LoadExternalCalibData();

		// 結束回傳
		return;
	}

	///- 區域變數 -///
	// CamCalib物件內成員函數回傳值
	int returnVal;

	// 校正板棋盤格的 x 向大小
	double cellSizeDx;

	// 校正板棋盤格的 y 向大小
	double cellSizeDy;

	// 校正板棋盤格的 x 向數量
	int patternSizeNx;
	
	// 校正板棋盤格的 y 向數量
	int patternSizeNy;

	// 校正影像的大小
	cv::Size photoSize;

	// 設定從 UI 取得的校正板棋盤格 x 向大小到 cellSizeDx
	cellSizeDx = this->parameter.cellSizeDx;

	// 設定從 UI 取得的校正板棋盤格 y 向大小到 cellSizeDy
	cellSizeDy = this->parameter.cellSizeDy;

	// 設定從 UI 取得的校正板棋盤格 x 向數量到 patternSizeNx
	patternSizeNx = this->parameter.patternSizeNx;

	// 設定從 UI 取得的校正板棋盤格 x 向數量到 patternSizeNy
	patternSizeNy = this->parameter.patternSizeNy;

	// 設定副進度條的狀態為 10 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(10);

	// 設定校正板棋盤格大小到 this->camCalib 物件
	this->camCalib.setCellSize(cellSizeDx, cellSizeDy);

	// 設定校正板棋盤格數量到 this->camCalib 物件
	this->camCalib.setPatternSize(patternSizeNx, patternSizeNy);

	// 設定副進度條的狀態為 20 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(20);

	// 如果靜音模式沒有開啟則輸出訊息
	if (!this->parameter.mute)
	{
		// 輸出訊息至 UI 上的訊息對話框
		this->PostMessageToLog("Calibration images are loading..."); 
	}

	// 初始化存放校正影像的空間
	this->parameter.calibImage.clear();

	// 初始化存放校正影像的空間 壓縮形式 2014/09/13
	this->parameter.compressCalibImage.clear();

	// 將存放 校正影像 的空間大小變更至校正影像的數量
	this->parameter.calibImage.resize(this->parameter.calibImageFileName.size());
	
	// 將存放 校正影像 壓縮版 的空間大小變更至校正影像的數量 2014/09/13
	this->parameter.compressCalibImage.resize(this->parameter.calibImageFileName.size());

	// 讀入校正影像
	// 迴圈每次依序讀取一張影像
	// 直到所有校正影像數量則結束
	for (size_t i = 0; i < this->parameter.calibImageFileName.size(); i++)
	{
		// 將校正影像以檔案名的方式 this->parameter.calibImageFileName[i]
		// 增加至 this->camCalib 物件
		// 並取回狀態回傳值存入 returnVal
		returnVal = this->camCalib.addCalibImg(this->parameter.calibImageFileName[i]);
		
		// 如果不是靜音模式則輸出訊息
		if (!this->parameter.mute)
		{
			// 輸出校正影像檔案名至訊息視窗
			this->PostMessageToLog(this->parameter.calibImageFileName[i]);
		}

		// 以檔案名的方式使用 cv::imread() 讀入影像
		cv::Mat temp = cv::imread(this->parameter.calibImageFileName[i]);

		// 如果不是記憶體精簡模式
		// 則將校正影像讀入記憶體中
		if (!this->parameter.memoryLite)
		{
			// 存取影像
			this->parameter.calibImage[i] = temp;
		}
		// 如果是記憶體精簡模式
		else
		{
			// 如果 壓縮選項 不是 NONE
			if (this->parameter.imageCompressType != -1)
			{
				// 將讀入的影像壓縮到記憶體暫存中
				this->CompressMat(temp, this->parameter.compressCalibImage[i]);
			}
		}
	} // end of for (size_t i = 0; i < this->parameter.calibImageFileName.size(); i++)

	// 設定副進度條的狀態為 30 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(30);

	// 從 this->camCalib 物件中取回校正影像大小存到 photoSize
	// 並取得狀態回傳值存入 returnVal
	returnVal = this->camCalib.getPhotoSize(photoSize);

	// 如果不是靜音模式則輸出訊息
	if (!this->parameter.mute)
	{
		// 輸出狀態訊息至訊息視窗
		this->PostMessageToLog("Calibrating...");
	}

	// 執行相機校正
	// 此函數會花較久的時間
	this->camCalib.calibrate();

	// 設定副進度條的狀態為 60 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(60);

	// 正射影像所需使用的校正影像 ID
	// ID 是依據校正影像檔案讀入的順序所排列
	int imageIdOfGlobalCoord;

	// 使用者在 UI 上的校正影像清單中對檔名進行左鍵雙擊則會取回 校正影像檔案ID
	// 如果使用者沒有執行雙擊動作，則預設情況 ID 會為 0，即第一張影像
	imageIdOfGlobalCoord = this->parameter.rectifyBaseImageId;

	// 從this->camCalib 物件中取得相機校正參數
	// 輸入參數 0 為相機 ID 
	this->parameter.camMatrix = this->camCalib.getCamMatrix(0);

	// 從this->camCalib 物件中取得相機校正參數
	// 輸入參數 0 為相機 ID 
	this->parameter.disCoeffs = this->camCalib.getDisCoeffs(0);

	// 設定副進度條的狀態為 70 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(70);

	// 取得校正板參數訊息
	// 根據 imageIdOfGlobalCoord 所指定的校正板取回校正板的 R 矩陣
	// 並存入 this->parameter.coordR
	returnVal = this->camCalib.getChessboardRVecs(imageIdOfGlobalCoord, this->parameter.coordR, 0);

	// 取得校正板參數訊息
	// 根據 imageIdOfGlobalCoord 所指定的校正板取回校正板的 T 矩陣
	// 並存入 this->parameter.coordT
	returnVal = this->camCalib.getChessboardTVecs(imageIdOfGlobalCoord, this->parameter.coordT, 0);

	// 設定副進度條的狀態為 85 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(85);

	// 區域變數
	// 存放輸出訊息用的字串
	CString cstr;

	///- 產生輸出訊息字串 -///
	// 相機的焦距
	this->msgAndLog += "Fx/Fy: ";
	
	// 從 this->parameter.camMatrix 中取得相機的 Fx
	cstr.Format(_T("%.3f"), this->parameter.camMatrix.at<double>(0, 0));

	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr + L" / ";

	// 從 this->parameter.camMatrix 中取得相機的 Fy
	cstr.Format(_T("%.3f"), this->parameter.camMatrix.at<double>(1, 1));
	
	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr;
	
	// 加入換行符號
	// 為了確保換行成功所以使用兩次 \r 符號
	this->msgAndLog += "\r\r\n";

	// 相機的中心
	this->msgAndLog += "Cx/Cy: ";
	
	// 從 this->parameter.camMatrix 中取得相機的 Cx
	cstr.Format(_T("%.3f"), this->parameter.camMatrix.at<double>(0, 2));
	
	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr + L" / ";
	
	// 從 this->parameter.camMatrix 中取得相機的 Cy
	cstr.Format(_T("%.3f"), this->parameter.camMatrix.at<double>(1, 2));
	
	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr;

	// 加入換行符號
	// 為了確保換行成功所以使用兩次 \r 符號
	this->msgAndLog += "\r\r\n";

	// 相機校正的誤差
	this->msgAndLog += "Calib. error: ";
	
	// 從 this->camCalib 物件中取回相機校正誤差
	// 輸入參數 0 為相機 ID
	cstr.Format(_T("%.3f"), this->camCalib.getCamCalibErr(0));
	
	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr;
	
	// 加入換行符號
	// 為了確保換行成功所以使用兩次 \r 符號
	this->msgAndLog += "\r\r\n";

	// 從 this->camCalib 物件中取得錯誤訊息並輸出
	this->msgAndLog += this->camCalib.getErrorMessage();

	// 加入換行符號
	this->msgAndLog += "\r\r\n";

	// 設定副進度條的狀態為 100 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(100);

	// 將訊息輸出到 UI 上的訊息視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
	
	// 將訊息視窗捲動至最底下
	MessageBoxAutoScrollToBottom();

	// 如果不是靜音模式則輸出訊息
	if (!this->parameter.mute)
	{
		// 輸出校正完成訊息
		this->PostMessageToLog("Calibrate done\n");
	}

	// 結束回傳
	return;
}

void CSteelBarASDlg::LoadExternalCalibData()
{
	// 使用 opencv 讀取檔案
	// this->parameter.externalCalibDataPath 為外部檔案名
	cv::FileStorage cvfs(this->parameter.externalCalibDataPath.c_str(), CV_STORAGE_READ);
	
	// 增加標籤節點
	cv::FileNode node(cvfs.fs, NULL);

	// 讀取標籤節點為 "cellSzizeDx" 的值到 this->parameter.cellSizeDx
	this->parameter.cellSizeDx = node["cellSizeDx"];

	// 讀取標籤節點為 "cellSzizeDy" 的值到 this->parameter.cellSizeDy
	this->parameter.cellSizeDy = node["cellSizeDy"];

	// 讀取標籤節點為 "patternSizeNx" 的值到 this->parameter.patternSizeNx
	this->parameter.patternSizeNx = node["patternSizeNx"];

	// 讀取標籤節點為 "patternSizeNy" 的值到 this->parameter.patternSizeNy
	this->parameter.patternSizeNy = node["patternSizeNy"];

	// 讀取標籤節點為 "chessImagePointsSize" 的值到暫存變數chessImgPtsSize
	// 用來計算 chessImage 有多少點
	int chessImgPtsSize = node["chessImagePointsSize"];

	// 增加標籤節點: 增加 "chessImagePoints"內的節點
	cv::FileNode fn = node[std::string("chessImagePoints")];

	// 增加節點迭代子
	cv::FileNodeIterator it = fn.begin(), it_end = fn.end();
	
	// 將棋盤影像點座標變數空間大小設定為 chessImgPtsSize
	this->parameter.chessImagePoints.resize(chessImgPtsSize);

	// 讀取標籤節點為 "coordR" 的值到 this->parameter.coordR
	cv::read(node["coordR"], this->parameter.coordR);

	// 讀取標籤節點為 "coordT" 的值到 this->parameter.coordT
	cv::read(node["coordT"], this->parameter.coordT);

	// 讀取標籤節點為 "camMatrix" 的值到 this->parameter.camMatrix
	cv::read(node["camMatrix"], this->parameter.camMatrix);

	// 讀取標籤節點為 "discoeffs" 的值到 this->parameter.discoeffs
	cv::read(node["discoeffs"], this->parameter.disCoeffs);

	// 讀入棋盤影像點
	// 因為 opencv 所提供的 node 讀取方式不支援型態 cv::Point2f
	// 所以改用迴圈將所有點依序讀入
	for (int i = 0; i < chessImgPtsSize; i++)
	{
		// 暫存變數
		// 用來將迴圈計數器數值轉換成字串
		std::stringstream ss;

		// 將整數 i 轉換為 std:;stringstream
		ss << i;

		// 產生節點名稱
		std::string ciptsX = "chessImagePoints_X_" + ss.str();
		
		// 讀入座標點 x
		this->parameter.chessImagePoints[i].x = node[ciptsX];

		// 產生節點名稱
		std::string ciptsY = "chessImagePoints_Y_" + ss.str();
		
		// 讀入座標點 y
		this->parameter.chessImagePoints[i].y = node[ciptsY];
	}

	// 輸出訊息至訊息視窗
	this->PostMessageToLog("Calibration data loaded");
	
	// 結束回傳
	return;
}

void CSteelBarASDlg::GenerateObjectPoints()
{
	// 輸出文字狀態至副進度條上
	// 此功能在 xp 主題下沒有作用
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"generating obj. pts");

	// 設定副進度條的範圍為 0 到 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 10);

	// 設定副進度條的狀態為 0 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);

	///- 區域變數 -///
	// CamCalib物件內成員函數回傳值
	int returnVal;

	// 輸出用訊息暫存變數
	CString cstr;

	// 從 this->camCalib 物件中取回 Corners 影像座標
	returnVal = this->camCalib.getCornersImagePoints(this->parameter.rectifyBaseImageId, this->parameter.chessImagePoints);

	// 物理距離，即實際在真實世界中的距離
	//      ________________________________________________________________________________________
	//     /   
	//    /  (棋盤格 X 向的大小 * 棋盤格 X 向的數量)^2 + (棋盤格 Y 向的大小 * 棋盤格 Y 向的數量)^2
	//   /
	// \/
	this->parameter.physicalDistance = (float)sqrt(										// 開根號
		pow(this->parameter.cellSizeDx * (this->parameter.patternSizeNx - 1), 2)		// (棋盤格X大小 * 棋盤格X數量)^2
		+ pow(this->parameter.cellSizeDy * (this->parameter.patternSizeNy - 1), 2)		// (棋盤格Y大小 * 棋盤格Y數量)^2
		);


	// 影像距離，影像上的距離
	// 看不懂OTL，無法加註解
	// 請詢問原著 Yuan-Sen Yang
	//      ________________________________________________________________________________________
	//     /   
	//    /  (影像X距離)^2 + (影像Y距離)^2
	//   /
	// \/
	this->parameter.imageDistance = (float)sqrt
		(pow(this->parameter.chessImagePoints[0].x - this->parameter.chessImagePoints[this->parameter.patternSizeNx * this->parameter.patternSizeNy - 1].x, 2)
		+ pow(this->parameter.chessImagePoints[0].y	- this->parameter.chessImagePoints[this->parameter.patternSizeNx * this->parameter.patternSizeNy - 1].y, 2)
		);

	// rpx 為影像對於真實世界的距離變換關係
	// rpx = 0.1 代表 1 pixel = 0.1 mm
	// rpx = 物理距離 / 影像距離 * 0.5
	this->parameter.rpx = this->parameter.physicalDistance / this->parameter.imageDistance * 0.5f;

	// 設定副進度條的狀態為 20 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(20);


	///- 產生輸出訊息字串 -///
	// rpx
	this->msgAndLog += "rpx(";

	// 從 this->parameter.rpx 中取得 rpx
	// 並從浮點數轉換為 CString 型態
	cstr.Format(_T("%.3f"), this->parameter.rpx);

	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr;

	// 物理距離
	this->msgAndLog += ") = phyDist(";

	// 從 this->parameter.physicalDistance 中取得真實世界座標距離
	// 並從浮點數轉換為 CString 型態
	cstr.Format(_T("%.3f"), this->parameter.physicalDistance);
	
	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr;
	
	// 影像距離
	this->msgAndLog += ") / imgDist(";
	
	// 從 this->parameter.imageDistance 中取得影像距離
	// 並從浮點數轉換為 CString 型態
	cstr.Format(_T("%.3f"), this->parameter.imageDistance);
	
	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr;
	
	// 輸出最後的運算文字
	// 加入換行符號
	// 為了確保換行成功所以使用兩次 \r 符號
	this->msgAndLog += ") * 0.5\r\r\n";


	// 設定副進度條的狀態為 50 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(50);
	
	// 正射影像 X 軸迭代用變數
	int ix;

	// 正射影像 Y 軸迭代用變數
	int iy;

	// 正射後的影像 X 座標
	float x;

	// 正射後的影像 Y 座標
	float y;

	// 正射影像的高
	// 高度 ＝　(棋盤格 Y 軸大小 * (棋盤格 Y 軸點數 - 1)) / rpx + 1
	this->parameter.rectfImageHeight = (int)(this->parameter.cellSizeDy * (this->parameter.patternSizeNy - 1) / this->parameter.rpx + 1.0);

	// 正射影像的寬
	// 寬度 ＝　(棋盤格 X 軸大小 * (棋盤格 X 軸點數 - 1)) / rpx + 1
	this->parameter.rectfImageWidth = (int)(this->parameter.cellSizeDx * (this->parameter.patternSizeNx - 1) / this->parameter.rpx + 1.0);
	
	// 3D 物件點
	// 看不懂...
	this->parameter.gridObjectPoints3D = cv::Mat(this->parameter.rectfImageHeight, this->parameter.rectfImageWidth, CV_32FC3 /* float, float, float */);

	// 整段看不懂...OTL
	// (´・ω・｀)
	for (iy = 0; iy < this->parameter.rectfImageHeight; iy++)
	{
		y = iy * this->parameter.rpx; // convert from pixel to physical size (e.g., mm) 
		for (ix = 0; ix < this->parameter.rectfImageWidth; ix++)
		{
			x = ix * this->parameter.rpx;
			this->parameter.gridObjectPoints3D.at<cv::Point3f>(this->parameter.rectfImageHeight - iy - 1, ix).x = x;
			this->parameter.gridObjectPoints3D.at<cv::Point3f>(this->parameter.rectfImageHeight - iy - 1, ix).y = y;
			this->parameter.gridObjectPoints3D.at<cv::Point3f>(this->parameter.rectfImageHeight - iy - 1, ix).z = 0.f;
		}
	}

	// 設定副進度條的狀態為 90 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(90);

	// 正射影像大小
	this->msgAndLog += "Rectify image with size of ";

	// 從 this->parameter.rectfImageWidth 中取得正射影像寬度
	// 並從浮點數轉換為 CString 型態
	cstr.Format(_T("%i"), this->parameter.rectfImageWidth);

	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr;

	// 排版用文字訊息
	this->msgAndLog += " × ";

	// 從 this->parameter.rectfImageHeight 中取得正射影像高度
	// 並從浮點數轉換為 CString 型態
	cstr.Format(_T("%i"), this->parameter.rectfImageHeight);

	// 將訊息加入全域訊息變數中
	this->msgAndLog += cstr;

	// 加入換行符號
	// 為了確保換行成功所以使用兩次 \r 符號
	this->msgAndLog += "\r\r\n";

	// 將訊息輸出到 UI 上的訊息視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);

	// 將訊息視窗文字捲動至最底下
	MessageBoxAutoScrollToBottom();

	// 輸出校正資料至檔案
	OutputCalibrationData();

	// 設定副進度條的狀態為 100 / 100
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(100);

	// 結束回傳
	return;
}

void CSteelBarASDlg::GenerateImagePoints()
{
	// 輸出文字狀態至副進度條上
	// 此功能在 xp 主題下沒有作用
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"generating images pts.");
	
	// 設定副進度條的範圍為 0 到 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 10);
	
	// 設定副進度條的狀態為 0 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);

	// 初始化 格點影像點
	this->parameter.gridImagePoints = cv::Mat(this->parameter.rectfImageHeight, this->parameter.rectfImageWidth, CV_32FC2 /* float */);

	// 設定副進度條的狀態為 5 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(5);

	// 產生格點影像
	cv::projectPoints(this->parameter.gridObjectPoints3D.reshape(0 /* channel not changed */, 1),
		this->parameter.coordR, this->parameter.coordT, this->parameter.camMatrix, this->parameter.disCoeffs,
		this->parameter.gridImagePoints.reshape(0 /* channel not changed */, 1));

	// 設定副進度條的狀態為 6 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(8);

	// 如果靜音模式沒有被開啟
	// 則輸出訊息
	if (!this->parameter.mute)
	{
		// 增加訊息
		this->msgAndLog += "\nGenerate grid image points\r\r\n";
	}

	// 將訊息顯示到 UI 上的訊息對話框
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);

	// 將訊息視窗捲動至最底下
	MessageBoxAutoScrollToBottom();

	// 設定副進度條的狀態為 10 / 10
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(10);

	// 結束回傳
	return;
}

void CSteelBarASDlg::GenerateRectifyImage()
{
	// 輸出文字狀態至副進度條上
	// 此功能在 xp 主題下沒有作用	
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"rectifying");
	
	// 設定副進度條的範圍為 0 到 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 1000);
	
	// 設定副進度條的狀態為 0 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);


	// 產生工作路徑
	DirectoryInit();
	
	// 設定副進度條的狀態為 100 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(100);

	// 如果靜音模式沒有被開啟
	// 則輸出訊息
	if (!this->parameter.mute)
	{
		// 增加訊息
		this->PostMessageToLog("Rectify images are generating...");
	} 
	
	// 清空 正射影像的檔名 空間
	this->parameter.rectifyImageFileName.clear();
	
	// 配置 正射影像的檔名 空間
	this->parameter.rectifyImageFileName.resize(this->parameter.expImageFileName.size());
	
	// 清空 實驗影像的檔案 空間
	this->parameter.expImage.clear();
	this->parameter.compressExpImage.clear();
	
	// 配置 實驗影像的檔案 空間
	this->parameter.expImage.resize(this->parameter.expImageFileName.size());
	this->parameter.compressExpImage.resize(this->parameter.expImageFileName.size());
	
	// 清空 正射影像的檔案 空間
	this->parameter.rectifyImage.clear();
	this->parameter.compressRectifyImage.clear();
	
	// 配置 正射影像的檔案 空間
	this->parameter.rectifyImage.resize(this->parameter.expImageFileName.size());
	this->parameter.compressRectifyImage.resize(this->parameter.expImageFileName.size());
	
	// 設定副進度條的狀態為 199 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(199);

	// 如果靜音模式沒有被開啟
	// 則創建顯示視窗
	if (!this->parameter.mute)
	{
		// 產生 顯示正射影像的視窗
		cv::namedWindow("rectify");
	}

	// 將所有的 實驗影像 轉換成 正射影像
	for (size_t i = 0; i < this->parameter.rectifyImageFileName.size(); i++)
	{
		// 產生 正射影像的 檔案名
		this->parameter.rectifyImageFileName[i] = this->parameter.outputPath + "\\rectify\\rectify_" + this->parameter.expImageFileNameWithoutPath[i];

		// 如果靜音模式沒有被開啟
		// 則輸出訊息
		if (!this->parameter.mute)
		{
			// 增加訊息
			this->PostMessageToLog(this->parameter.rectifyImageFileName[i]);
		}
		
		// 讀取 實驗影像
		cv::Mat expImage = cv::imread(this->parameter.expImageFileName[i]);
		
		// 如果讀取失敗
		if (expImage.empty())
		{
			// 如果靜音模式沒有被開啟
			// 則輸出訊息
			if (!this->parameter.mute)
			{
				// 增加訊息
				this->PostMessageToLog("can not read image: " + this->parameter.expImageFileName[i]);
			}
		}

		// 如果不是記憶體精簡模式
		// 則保留 實驗影像 在記憶體裡面
		if (!this->parameter.memoryLite)
		{
			// 存取 實驗影像
			this->parameter.expImage[i] = expImage;
		}
		// 如果是 記憶體精簡模式
		else
		{
			// 如果 壓縮形式 不是 NONE
			if (this->parameter.imageCompressType != -1)
			{
				// 存入 實驗影像 到記憶體中
				this->CompressMat(expImage, this->parameter.compressExpImage[i]);
			}
		}
		
		// 正射影像 
		cv::Mat rectifyImage;
		
		// 空影像
		cv::Mat emptyMat;
		
		// 產生正射影像
		cv::remap(expImage, rectifyImage, this->parameter.gridImagePoints, emptyMat, cv::INTER_LINEAR);
		
		// 如果不是記憶體精簡模式
		// 則保留 正射影像 在記憶體裡面
		if (!this->parameter.memoryLite)
		{
			// 存取 正射影像
			this->parameter.rectifyImage[i] = rectifyImage;
		}
		// 如果是 記憶體精簡模式
		else
		{
			// 如果 壓縮形式 不是 NONE
			if (this->parameter.imageCompressType != -1)
			{
				// 存入 正射影像 到記憶體中
				this->CompressMat(rectifyImage, this->parameter.compressRectifyImage[i]);
			}
		}
		
		// 取得 輸出用的正射影像檔案名
		std::string oFileName = this->parameter.rectifyImageFileName[i];
		
		// 嘗試:將正射影像寫入硬碟
		try
		{
			// 輸出 正射影像 到硬碟
			bool bSuccess = cv::imwrite(oFileName, rectifyImage);
			
			// 如果無法正常產生影像檔案
			if (!bSuccess)
			{
				// 如果靜音模式沒有開啟
				if (!this->parameter.mute)
				{
					// 輸出寫入失敗訊息到 UI 上的訊息對話框
					this->PostMessageToLog("Fail writing image to storage: " + oFileName);
				} // end of if (!this->parameter.mute)
			} // end of if (!bSuccess)
		} // end of try
		// 捕捉例外
		// 如果發生 runtime 錯誤則接住
		catch (std::runtime_error& ex)
		{
			// 取得 例外訊息
			std::string ss = std::string(ex.what());
			
			// 如果不是靜音模式
			// 則輸出訊息
			if (!this->parameter.mute)
			{
				// 輸出錯誤訊息
				this->PostMessageToLog("Exception converting image to JPG format: " + ss);
			} // end of if (!this->parameter.mute)
		} // end of catch (std::runtime_error)
		// 捕捉例外
		// 如果發生 c++ 標準以外的例外則接住
		catch (std::exception& ex)
		{
			// 取得 例外訊息
			std::string ss = std::string(ex.what());
			
			// 如果不是靜音模式
			// 則輸出訊息
			if (!this->parameter.mute)
			{
				// 輸出錯誤訊息
				this->PostMessageToLog("Exception: " + ss);
			} // end of if (!this->parameter.mute)
		} // end of catch (std''exception)

		// 如果不是靜音模式
		// 則顯示正射影像結果
		if (!this->parameter.mute)
		{
			// 顯示 正射影像
			cv::imshow("rectify", rectifyImage);
			
			// 等待 5ms
			cv::waitKey(5);
		}

		// 計算 進度條 的 進度數值
		int procCount = 199 + (int)((double)800 * (double)((double)i / (double)this->parameter.rectifyImageFileName.size()));
		
		// 設定 副進度條的狀態為 procCount
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(procCount);
	} // end of for (size_t i = 0; i < this->parameter.rectifyImageFileName.size(); i++)

	// 如果不是靜音模式
	// 則銷毀正射影像視窗
	if (!this->parameter.mute)
	{
		// 銷毀 正射影像視窗
		cv::destroyWindow("rectify");
	}

	// 設定 副進度條的狀態為 1000 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(1000);
	
	// 如果不是靜音模式
	// 則輸出訊息
	if (!this->parameter.mute)
	{
		// 增加訊息
		this->msgAndLog += "Rectify images are generated\r\r\n";
		
		// 顯示正射訊息
		((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
		
		// 將訊息視窗捲動到最底下
		MessageBoxAutoScrollToBottom();
	}

	// 結束回傳
	return;
}

void CSteelBarASDlg::Calculate()
{
	// 輸出文字狀態至副進度條上
	// 此功能在 xp 主題下沒有作用
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"computing");
	
	// 設定副進度條的範圍為 0 到 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 1000);
	
	// 設定副進度條的狀態為 0 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);

	// 清空 垂直點 資料空間
	this->parameter.vPointsData.clear();
	
	// 配置 垂直點 資料空間
	this->parameter.vPointsData.resize(this->parameter.expImageFileName.size());

	// 清空 水平點 資料空間
	this->parameter.hPointsData.clear();
	
	// 配置 水平點 資料空間
	this->parameter.hPointsData.resize(this->parameter.expImageFileName.size());

	// 清空 直徑 資料空間
	this->parameter.analysisDiameters.clear();
	
	// 配置 直徑 資料空間
	this->parameter.analysisDiameters.resize(this->parameter.expImageFileName.size());

	// 清空 拉伸量 資料空間
	this->parameter.analysisElongations.clear();
	
	// 配置 拉伸量 資料空間
	this->parameter.analysisElongations.resize(this->parameter.expImageFileName.size());

	// 取得 分析方法 的連續性
	// false: 表示 分析方法不連續，表示每張照片是獨立不會受到其他影像影響
	// true: 表示 分析方法必須是連續的，每張影像間有關連性
	bool continueType = this->analysisContinueTypes[this->parameter.analysisMethodIndex]();

	// 宣告 存放分析結果空間指標
	float** results = { 0 };

	// 從分析方法中取回計算結果
	// 分析方法為外掛模組
	// 依照不同的分析方法會得到不同的結果
	// this->analysisMethods[number] 表示選擇使用的是哪一個方法
	// (this->parameter.rectifyImage, 0) 表示傳入正射影像進行運算， 第二個參數暫時保留
	results = this->analysisMethods[this->parameter.analysisMethodIndex](this->parameter.rectifyImage, 0);
	
	// 設定 副進度條 的狀態為 800 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(800);

	// 將取回的計算結果分別存入
	for (size_t i = 0; i < this->parameter.rectifyImage.size(); i++)
	{
		// 取得 拉伸量 的分析結果
		this->parameter.analysisElongations[i] = results[0][i];
		
		// 取得 直徑 的分析結果
		this->parameter.analysisDiameters[i] = results[1][i];

		// 計算 進度條 的 進度數值
		int procCount = 800 + (int)((double)200 * (double)((double)i / (double)this->parameter.expImageFileName.size()));
		
		// 設定 副進度條的狀態為 procCount
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(procCount);
	}

	// 設定 副進度條的狀態為 1000 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(1000);

	// 結束回傳
	return;
}

void CSteelBarASDlg::WriteCalcData()
{
	// 輸出文字狀態至副進度條上
	// 此功能在 xp 主題下沒有作用	
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"writing data");
	
	// 設定副進度條的範圍為 0 到 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 1000);
	
	// 設定副進度條的狀態為 0 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);


	// 防呆: 如果沒有任何的計算資料
	// 則輸出錯誤訊息 並 結束回傳
	if ((this->parameter.vPointsData.size() <= 0) || (this->parameter.hPointsData.size() <= 0))
	{
		// 輸出錯誤訊息
		if (!this->parameter.mute) this->PostMessageToLog("Output: no data");

		// 結束回傳
		return;
	}

	// 設定 副進度條 的狀態為 50 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(50);

	// 創建 輸出用的結果檔案
	std::ofstream oFile("data.csv", std::ios::out | std::ios::trunc);

	// 產生 title
	oFile << "No.,,";

	// 產生 title
	oFile << "Diameter,Elongation" << std::endl;

	// 取得 每張影像的計算結果
	for (size_t i = 0; i < this->parameter.expImageFileName.size(); i++)
	{
		// 輸出 直徑 及 伸長量 到 結果檔案
		oFile << i << ",," << this->parameter.analysisDiameters[i] << "," << this->parameter.analysisElongations[i] << std::endl;

		// 計算 進度條 的 進度數值
		int procCount = 50 + (int)((double)950 * (double)((double)i / (double)this->parameter.expImageFileName.size()));
		
		// 設定 副進度條 的 狀態為 procCount
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(procCount);
	}

	// 關閉 輸出用的結果檔案
	oFile.close();

	// 設定 副進度條 的狀態為 1000 / 1000
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(1000);

	// 結束回傳
	return;
}

void CSteelBarASDlg::OnBnClickedButtonSelectOutputPath()
{
	// 開啟選擇輸出路徑的 對話盒
	SelectFolder();

	// 將寬字元轉換為窄字元
	wstr2str(this->szPathName, this->parameter.outputPath);

	// 將 std::string 轉換成 CString
	CString cstr(this->parameter.outputPath.c_str());

	// 顯示 指定的輸出路徑位置
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_OUTPUT_PATH))->SetWindowTextW(cstr);

	// 如果不是靜音模式
	if (!this->parameter.mute)
	{
		// 增加訊息
		this->msgAndLog += "output path: ";

		// 增加訊息
		this->msgAndLog += this->parameter.outputPath.c_str();
		
		// 增加訊息
		this->msgAndLog += "\r\r\n";
		
		// 輸出訊息到 UI 上的訊息視窗
		((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
		
		// 將訊息視窗捲動到最下方
		MessageBoxAutoScrollToBottom();
	}

	// 結束回傳
	return;
}

void CSteelBarASDlg::OnBnClickedButtonStop()
{
	// 設定 中斷工作執行續 旗標為 TRUE
	this->parameter.terminateThread = TRUE;
	
	// 中斷 工作執行續
	SetEvent(this->parameter.m_hKillEvent);
	
	// 將 UI 上的 暫停按鈕設為 不可使用
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(FALSE);
	
	// 輸出訊息
	this->PostMessageToLog("Wait for work thread terminate...");
	
	// 放出 中斷執行續命令
	this->parameter.m_pThread->PostThreadMessageW(WM_QUIT, 0, 0);

	// 清除 執行續 HANDLE
	this->parameter.m_pThread->m_hThread = 0;
	
	// 清除 執行續
	this->parameter.m_pThread = 0;
	
	// 如果不是靜音模式
	if (!this->parameter.mute)
	{
		// 增加訊息
		this->msgAndLog += "Terminated by user\r\r\n";

		// 輸出中斷訊息
		((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);

		// 將訊息視窗捲動到最底下
		MessageBoxAutoScrollToBottom();
	}

	// 結束回傳
	return;
}

void CSteelBarASDlg::message()
{
	// 增加訊息
	this->msgAndLog += this->tMsgLog;

	// 輸出訊息到 UI 上的訊息對話框
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);

	// 將訊息視窗捲動到最底下
	MessageBoxAutoScrollToBottom();

	// 結束回傳
	return;
}

void CSteelBarASDlg::PostMessageToLog(CString msg)
{
	// 增加訊息
	this->tMsgLog = msg;
	
	// 增加換行符號
	this->tMsgLog += L"\r\r\n";
	
	// 開啟 訊息 執行續
	this->parameter.m_pMsgThread = AfxBeginThread(msgThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	
	// 等待輸出訊息執行續完成
	::WaitForSingleObject(this->parameter.m_pMsgThread, INFINITE);
	
	// 休息 5ms
	// 確保訊息執行續回收
	Sleep(5);

	// 結束回傳
	return;
}

void CSteelBarASDlg::PostMessageToLog(std::string msg)
{
	// 將 std::string 轉換成 CString
	CString tmp = (CA2CT)msg.c_str();
	
	// 增加訊息
	this->tMsgLog = tmp;
	
	// 增加換行符號
	this->tMsgLog += L"\r\r\n";
	
	// 開啟 訊息 執行續
	this->parameter.m_pMsgThread = AfxBeginThread(msgThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	
	// 等待 輸出訊息 執行續完成
	::WaitForSingleObject(this->parameter.m_pMsgThread, INFINITE);
	
	// 休息 5ms
	// 確保訊息執行續回收
	Sleep(5);

	// 結束回傳
	return;
}

void CSteelBarASDlg::OnLbnDblclkListBoxCalibImagePath()
{
	// 校正基準影像檔案名
	CString str;

	// 從 UI 上取回最新的結果
	UpdateData();

	// 取得 UI 上的 校正影像清單中被選中的 ID
	this->parameter.rectifyBaseImageId = ((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->GetCurSel();
	
	// 如果取得失敗
	if (this->parameter.rectifyBaseImageId == LB_ERR)
	{
		// 結束回傳
		return;
	}

	// 取得 被選中的 ID 的文字
	((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->GetText(this->parameter.rectifyBaseImageId, str);

	// 顯示選種的正射基準影像檔名
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_RECTIFY_BASE_IMAGE))->SetWindowTextW(str);

	// 正射基準影像 預覽用的空間指標
	IplImage* image = NULL;

	// 防呆
	// 如果已經有東西則清空
	if (image)
	{
		// 清除 影像
		cvReleaseImage(&image);
	}
	
	// 取得 校正影像完整路徑及檔案名
	std::string path = this->parameter.calibImageFileName[this->parameter.rectifyBaseImageId];
	
	// 讀取正射用校正基準影像
	image = cvLoadImage(path.c_str(), 1);
	
	// 將基準影像顯示到指定的圖形盒中
	DrawImageToHDC(image, IDC_PICTURE_CONTROL_BASE_IMAGE);

	// 結束回傳
	return;
}

void CSteelBarASDlg::DrawImageToHDC(IplImage* img, UINT Id)
{
	// 取得 圖形盒的指標
	CDC* pDC = GetDlgItem(Id)->GetDC();
	
	// 取得 圖形盒的 handle
	HDC hDC = pDC->GetSafeHdc();
	
	// 圖形盒的位置
	CRect rect;
	
	// 取得 圖形盒的座標
	GetDlgItem(Id)->GetClientRect(&rect);
	
	// 顯示用的 影像 空間
	CvvImage cimg;
	
	// 複製影像到輸出用的空間
	cimg.CopyOf(img);
	
	// 將影像畫到指定的 DC 上
	cimg.DrawToHDC(hDC, &rect);
	
	// 釋放 DC指標
	ReleaseDC(pDC);

	// 結束回傳
	return;
}

void CSteelBarASDlg::OnBnClickedButtonClear()
{
	// 清除變數
	VariableClear();
	
	// 清除 UI 上的資料
	UiDataClear();

	// 如果不是靜音模式
	if (!this->parameter.mute)
	{
		// 輸出訊息
		this->PostMessageToLog("Clear");
	}

	// 結束回傳
	return;
}

void CSteelBarASDlg::OutputCalibrationData()
{
	// 宣告 校正資料的檔案名
	char filename[MAX_PATH];
	
	// 產生 校正資料的檔案名
	sprintf_s(filename, "%s\\calibData.xml", this->parameter.outputPath.c_str());

	// 開啟 opencv 資料結構的 檔案
	cv::FileStorage cvfsCalibData(filename, CV_STORAGE_WRITE);

	// 輸出 cellSizeDx 到 校正資料檔案中
	cv::write(cvfsCalibData, "cellSizeDx", this->parameter.cellSizeDx);
	
	// 輸出 cellSizeDy 到 校正資料檔案中
	cv::write(cvfsCalibData, "cellSizeDy", this->parameter.cellSizeDy);
	
	// 輸出 patternSizeNx 到 校正資料檔案中
	cv::write(cvfsCalibData, "patternSizeNx", this->parameter.patternSizeNx);
	
	// 輸出 patternSizeNy 到 校正資料檔案中
	cv::write(cvfsCalibData, "patternSizeNy", this->parameter.patternSizeNy);
	
	// 輸出 棋盤格點數 到 校正資料檔案中
	int chessImgPtsSize = static_cast<int>(this->parameter.chessImagePoints.size());
	
	// 輸出 chessImagePointsSize 到 校正資料檔案中
	cv::write(cvfsCalibData, "chessImagePointsSize", chessImgPtsSize);
	
	// 輸出 coordR 到 校正資料檔案中
	cv::write(cvfsCalibData, "coordR", this->parameter.coordR);
	
	// 輸出 coordT 到 校正資料檔案中
	cv::write(cvfsCalibData, "coordT", this->parameter.coordT);
	
	// 輸出 camMatrix 到 校正資料檔案中
	cv::write(cvfsCalibData, "camMatrix", this->parameter.camMatrix);
	
	// 輸出 discoeffs 到 校正資料檔案中
	cv::write(cvfsCalibData, "discoeffs", this->parameter.disCoeffs);

	// 依序輸出棋盤格點座標
	for (int i = 0; i < chessImgPtsSize; i++)
	{
		// 數值轉文字用變數
		std::stringstream ss;
		
		// 將數值轉成文字
		ss << i;
		
		// 產生 棋盤格 X 位置的 文字
		std::string ciptsX = "chessImagePoints_X_" + ss.str();
		
		// 輸出 棋盤格 X 座標
		cv::write(cvfsCalibData, ciptsX, this->parameter.chessImagePoints[i].x);
		
		// 產生 棋盤格 Y 位置的 文字
		std::string ciptsY = "chessImagePoints_Y_" + ss.str();
		
		// 輸出 棋盤格 Y 座標
		cv::write(cvfsCalibData, ciptsY, this->parameter.chessImagePoints[i].y);
	}

	// 釋放 校正資料檔案
	cvfsCalibData.release();

	// 結束回傳
	return;

}

void CSteelBarASDlg::OnBnClickedButtonLoadExternalCalibData()
{
	// 副檔名 過濾器
	CString filter("XML File (*.xml)|*.xml||");
	
	// 檔案路徑
	CString filePath;
	
	// 檔案路徑暫存
	CString strBuf;

	// 檔案位置
	POSITION pos = NULL;
	
	// 開啟檔案對話盒
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	
	// 錯誤訊息變數
	int err = 0;
	
	// 錯誤訊息變數
	int lbErr = 0;
	
	// 如果沒有錯誤
	if (!err)
	{
		// 嘗試: 取得檔案狀態
		try
		{
			// 取得檔案名
			selDlg.GetOFN().lpstrFile = strBuf.GetBuffer(MAX_PATH * 100);
			
			// 取得檔案數量
			selDlg.GetOFN().nMaxFile = MAX_PATH * 100;
		}
		// 捕捉例外
		catch (...)
		{
			// 錯誤訊息
			err = 1;
		} // end of catch (...)
	} // end of if (!err)

	// 如果沒有錯誤
	if (!err)
	{
		// 如果對話盒被沒有按下 OK
		if (selDlg.DoModal() != IDOK)
		{
			// 錯誤訊息
			err = 1;
		}
	}
	
	// 如果沒有錯誤
	if (!err)
	{
		// 如果沒有選擇檔案
		if ((pos = selDlg.GetStartPosition()) == NULL)
		{
			// 錯誤訊息
			err = 1;
		}
	}
	
	// 如果沒有錯誤
	if (!err)
	{
		// 一個一個取回檔案名
		while (pos)
		{
			// 取得檔案名
			filePath = selDlg.GetNextPathName(pos);
			
			// 如果沒有錯誤
			if (!err)
			{
				// 檔案名的暫存
				std::string tmpName((CT2CA)filePath);
				
				// 取得 外部校正參數檔案名 含 路徑
				this->parameter.externalCalibDataPath = tmpName;
				
				// 檔案名 不含 路徑
				CString filename = filePath.Right(filePath.GetLength() - filePath.ReverseFind(_T('\\')) - 1);
				
				// 顯示 檔案名到 UI 上
				((CEdit*)GetDlgItem(IDC_EDIT_TEXT_LOAD_EXTERNAL_CALIB_DATA))->SetWindowTextW(filename);
				
				// 檔案名暫存
				std::string tmpName2 = (CT2CA)filename;
				
				// 取得外部檔案名 不瞞路徑
				this->parameter.externalCalibDataName = tmpName2;
				
				// 如果有錯誤
				if (lbErr == LB_ERR || lbErr == LB_ERRSPACE)
				{
					// 錯誤訊息
					err = 1;
				}
				
				// 如果不是靜音模式
				if (!this->parameter.mute)
				{
					// 輸出 讀取的外部校正檔案名
					this->PostMessageToLog("Improt: " + this->parameter.externalCalibDataName);
				}
				
				// 外部校正資料的旗標
				this->parameter.calibDataImprotFromExternal = true;
			}

			// 如果 發生錯誤
			if (err)
			{
				// 離開
				break;
			}
		}// end of while (pos)

		// 更新 UI 上的資料
		UpdateData(FALSE);
	}

	// 釋放 檔案名暫存
	strBuf.ReleaseBuffer();

	// 結束回傳
	return;
}

void CSteelBarASDlg::VariableClear()
{
	// 清除 校正影像 記憶體空間
	this->parameter.calibImage.clear();
	
	// 清除 實驗影像 記憶體空間
	this->parameter.expImage.clear();
	
	// 清除 正射影像 記憶體空間
	this->parameter.rectifyImage.clear();
	
	// 清除 棋盤格點影像 記憶體空間
	this->parameter.chessImagePoints.clear();
	
	// 清除 實驗影像 檔案名 記憶體空間
	this->parameter.expImageFileName.clear();
	
	// 清除 實驗影像 不含路徑 檔案名 記憶體空間
	this->parameter.expImageFileNameWithoutPath.clear();
	
	// 清除 水平點 記憶體空間
	this->parameter.hPointsData.clear();
	
	// 清除 垂直點 記憶體空間
	this->parameter.vPointsData.clear();
	
	// 清除 正射影像檔案名 記憶體空間
	this->parameter.rectifyImageFileName.clear();


	// 放下 外部校正資料 旗標 
	this->parameter.calibDataImprotFromExternal = FALSE;
	
	// 放下 記憶體精簡 旗標
	this->parameter.memoryLite = FALSE;
	
	// 放下 靜音模式 旗標
	this->parameter.mute = FALSE;
	
	// 重設相機校正資料
	this->camCalib.reset();
	
	// 初始化變數
	VariableInit();

	// 結束回傳
	return;
}

void CSteelBarASDlg::UiDataClear()
{
	// 清除 校正影像路徑 文字視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_CALIB_IMAGE_PATH))->SetWindowTextW(L"");
	
	// 清除 實驗影像路徑 文字視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_EXP_IMAGE_PATH))->SetWindowTextW(L"");
	
	// 清除 校正影像路徑 清單
	((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->ResetContent();
	
	// 清除 實驗影像路徑 清單
	((CListBox*)GetDlgItem(IDC_LIST_BOX_EXP_IMAGE_PATH))->ResetContent();
	
	// 清除 外部校正資料 文字視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_LOAD_EXTERNAL_CALIB_DATA))->SetWindowTextW(L"");
	
	// 清除 輸出路徑 文字視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_OUTPUT_PATH))->SetWindowTextW(L"");
	
	// 清除 校正基準影像 文字視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_RECTIFY_BASE_IMAGE))->SetWindowTextW(L"");
	
	// 清除 校正基準影像 圖形盒
	((CStatic*)GetDlgItem(IDC_PICTURE_CONTROL_BASE_IMAGE))->SetBitmap(0);
	
	// 重繪 校正基準影像 圖形盒
	((CStatic*)GetDlgItem(IDC_PICTURE_CONTROL_BASE_IMAGE))->Invalidate();
	
	// 更新 校正基準影像 圖形盒
	((CStatic*)GetDlgItem(IDC_PICTURE_CONTROL_BASE_IMAGE))->UpdateWindow();
	
	// 清除 勾選 記憶體精簡 選項
	((CButton*)GetDlgItem(IDC_CHECK_BOX_MEMORY_LITE))->SetCheck(FALSE);
	
	// 清除 勾選 靜音模式 選項
	((CButton*)GetDlgItem(IDC_CHECK_BOX_MUTE))->SetCheck(FALSE);
	
	// 重繪 主對話盒
	Invalidate();
	
	// 更新 UI 數據資料
	UpdateWindow();

	// 設定副進度條狀態為 0 / 1
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);
	
	// 設定主進度條狀態為 0 / 1
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_GLOBAL))->SetPos(0);
	
	// 設定 不勾選 記憶體精簡 選項
	((CButton*)GetDlgItem(IDC_CHECK_BOX_MEMORY_LITE))->SetState(FALSE);
	
	// 設定 執行 按鈕 為 可使用
	((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(TRUE);
	
	// 設定 清除 按鈕 為 不可使用
	((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(FALSE);

	// 結束回傳
	return;
}


void CSteelBarASDlg::MessageBoxAutoScrollToBottom()
{
	// 取得 訊息視窗 目前總行數
	int line = ((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->GetLineCount() - 1;
	
	// 取得 訊息視窗 目前位置
	int index = ((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->LineIndex(line);
	
	// 設定 訊息視窗 位置為最後一行
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(index, index);

	// 結束回傳
	return;
}

void CSteelBarASDlg::LoadExternalAnalysisMethod()
{
	// 嘗試: 讀取外部分析方法
	try
	{
		// 當前路徑 變數
		WCHAR nowDir[MAX_PATH];
		
		// 當前路徑 變數
		char cdir[MAX_PATH];
		
		// 預設符號
		char defChar = ' ';
		
		// 取得 當前工作路徑
		::GetCurrentDirectory(MAX_PATH, nowDir);
		
		// 型態轉換 將 WCHAR 轉換成 char
		WideCharToMultiByte(CP_ACP, 0, nowDir, -1, cdir, MAX_PATH, &defChar, NULL);
		
		// 將 char 轉換成 std::string
		std::string dir(cdir);
		
		// 外掛路徑 = 當前路徑 + "plugin"
		dir += "\\plugin";
		
		// 輸出 當前路徑 訊息
		this->PostMessageToLog("now: " + dir);
		
		// 取得分析方法
		GetAnalysisMethodDirectory(dir);
		
		// 如果沒有取得任何方法
		if (this->parameter.analysisMethodDllList.size() < 0)
		{
			// 結束回傳
			return;
		}

		// 宣告 dll 實體位置
		std::vector<HINSTANCE> hDllInstances;

		// 清空 dll 實體位置 空間
		hDllInstances.clear();

		// 配置分析方法實體位置 空間
		hDllInstances.resize(this->parameter.analysisMethodDllList.size());
		
		// 清空 分析方法 本體函數 記憶體空間
		this->analysisMethods.clear();
		
		// 配置 分析方法 本體函數 記憶體空間
		this->analysisMethods.resize(this->parameter.analysisMethodDllList.size());
		
		// 清空 分析方法 名稱函數 記憶體空間
		this->analysisNames.clear();
		
		// 配置 分析方法 名稱函數 記憶體空間
		this->analysisNames.resize(this->parameter.analysisMethodDllList.size());
		
		// 清空 分析方法 簡述函數 記憶體空間
		this->analysisCaptions.clear();
		
		// 配置 分析方法 簡述函數 記憶體空間
		this->analysisCaptions.resize(this->parameter.analysisMethodDllList.size());
		
		// 清空 分析方法 版本函數 記憶體空間
		this->analysisVersions.clear();
		
		// 配置 分析方法 版本函數 記憶體空間
		this->analysisVersions.resize(this->parameter.analysisMethodDllList.size());
		
		// 清空 分析方法 連續型態函數 記憶體空間
		this->analysisContinueTypes.clear();
		
		// 配置 分析方法 連續型態函數 記憶體空間
		this->analysisContinueTypes.resize(this->parameter.analysisMethodDllList.size());
		
		// 清空 分析方法 釋放記憶體函數 記憶體空間
		this->analysisFreeMemory.clear();
		
		// 配置 分析方法 釋放記憶體函數 記憶體空間
		this->analysisFreeMemory.resize(this->parameter.analysisMethodDllList.size());

		
		// 依序讀取分析方法
		// 先讀取然後釋放
		// 要用時再重新讀取
		for (size_t i = 0; i < this->parameter.analysisMethodDllList.size(); i++)
		{
			// 從分析方法名稱清單中 取出 分析方法名
			std::string str = this->parameter.analysisMethodDllList[i];

			// 分析方法路徑 + 名稱
			std::string path = "plugin\\" + str;

			// 載入 分析方法
			hDllInstances[i] = ::LoadLibraryA(path.c_str());

			// 錯誤碼
			DWORD errCode;

			// 取得當前錯誤訊息
			errCode = ::GetLastError();

			// 如果載入分析方法失敗
			// 則輸出錯誤訊息
			if (!hDllInstances[i])
			{
				// 輸出無法載入分析方法訊息
				this->PostMessageToLog("Can not load plug-in: " + this->parameter.analysisMethodDllList[i]);
			}
			// 如果載入分析方法成功
			else
			{
				// 取得 分析方法 連續種類
				// Limit5: 保留項目 2014/06/20
				this->analysisContinueTypes[i] = (GETANALSYISCONTINUETYPE)::GetProcAddress(hDllInstances[i], "getContinueType");

				// 如果無法取得分析方法的連續種類
				// 則輸出錯誤訊息
				// Limit5: 保留項目 不理會錯誤 2014/06/20
				if (!this->analysisContinueTypes[i])
				{
					// 輸出錯誤訊息
					this->PostMessageToLog("Can not load plug-in continue type: " + this->parameter.analysisMethodDllList[i]);
				}

				// 取得分析方法本體
				this->analysisMethods[i] = (ANALYSISMETHOD)::GetProcAddress(hDllInstances[i], "analysisMethod");

				// 如果無法取得分析方法本體
				// 則輸出錯誤訊息
				if (!this->analysisMethods[i])
				{
					// 輸出錯誤訊息
					this->PostMessageToLog("Can not load plug-in function: " + this->parameter.analysisMethodDllList[i]);
				}
				// 如果取得分析方法本體成功
				else
				{
					// 取得 分析方法名稱
					this->analysisNames[i] = (GETANALYSISNAME)::GetProcAddress(hDllInstances[i], "getAnalysisName");

					// 如果無法取得分析方法名稱
					// 則輸出錯誤訊息
					if (!this->analysisNames[i])
					{
						// 輸出錯誤訊息
						this->PostMessageToLog("Can not load plug-in name: " + this->parameter.analysisMethodDllList[i]);
					}
					// 如果取得分析方法名稱成功
					else
					{
						// 取得分析方法簡述內容
						this->analysisCaptions[i] = (GETANALYSISCAPTION)::GetProcAddress(hDllInstances[i], "getCaption");

						// 如果無法取得分析方法簡述內容
						// 則輸出錯誤訊息
						if (!this->analysisCaptions[i])
						{
							// 輸出錯誤訊息
							this->PostMessageToLog("Can not load plug-in caption: " + this->parameter.analysisMethodDllList[i]);
						}
						// 如果取得分析方法簡述內容成功
						else
						{
							// 取得分析方法版本
							this->analysisVersions[i] = (GETANALYSISVERSION)::GetProcAddress(hDllInstances[i], "getVersion");

							// 如果無法取得分析方法版本
							// 則輸出錯誤訊息
							if (!this->analysisVersions[i])
							{
								// 輸出錯誤訊息
								this->PostMessageToLog("Can not load plug-in version: " + this->parameter.analysisMethodDllList[i]);
							}
							// 如果取得分析方法版本成功
							else
							{
								// 取得分析方法釋放記憶體功能
								this->analysisFreeMemory[i] = (FREEANALYSISDATAMEMORY)::GetProcAddress(hDllInstances[i], "freeDataMemory");

								// 如果無法取得分析方法釋放記憶體功能
								// 則輸出錯誤訊息
								if (!this->analysisFreeMemory[i])
								{
									// 輸出錯誤訊息
									this->PostMessageToLog("Can not load plug-in free memory: " + this->parameter.analysisMethodDllList[i]);
								}
								// 如果取得分析方法釋放記憶體功能成功
								else
								{
									// 輸出讀取 外掛 分析方法 成功
									this->PostMessageToLog(this->parameter.analysisMethodDllList[i] + " loaded.");
								}

							} // end of 取得分析方法版本成功
						} // end of 取得分析方法簡述內容成功
					} // end of 取得分方法名稱成功
				} // end of 取得分析方法本體
			} // end of 載入分析方法成功
		} // end of 依序讀取分析方法
	} // end of 嘗試: 讀取外部分析方法
	// 捕捉例外
	// 如果發生 c++ 標準以外的例外則接住
	catch (std::exception& ex)
	{
		// 例外訊息用變數
		CString exception;

		// 組合訊息字串
		exception = "Exception: ";

		// 取得例外訊息
		// 存入 exception 變數
		exception += ex.what();

		// 將例外訊息輸出至 UI 上的訊息對話框
		this->PostMessageToLog(exception);

		// 回傳結束
		return;
	}
	// 捕捉例外
	// 如果發生其他無法預期的例外則接住
	catch (...)
	{
		// 輸出例外訊息至 UI 上的訊息對話框
		this->PostMessageToLog("An unknown error has occurred.");

		// 回傳結束
		return;
	}

	// 彈出式對話框確認 dll 載入
	//::MessageBox(NULL, L"load ext dll", L"MSG", NULL);

	// 回傳結束
	return;
}

void CSteelBarASDlg::AnalysisMethodInit()
{
	// 載入分析方法
	this->LoadExternalAnalysisMethod();

	// 防呆
	// 檢查分析方法是否存在
	// 如果分析方法清單中沒有存在任何一種方法
	// 則離開
	if (this->parameter.analysisMethodDllList.size() <= 0)
	{
		// 結束回傳
		return;
	}

	// 從分析方法清單中一個一個取出每一個方法名
	for (size_t i = 0; i < this->parameter.analysisMethodDllList.size(); i++)
	{
		// 分析方法名
		std::string name;

		// 嘗試: 取出分析方法名
		try
		{
			// 從分析方法外掛中取得分析方法的名稱
			name = this->analysisNames[i]();

			// 將分析方法的名稱顯示到 UI 上的訊息視窗
			this->PostMessageToLog(name);
		}
		// 捕捉例外
		// 如果發生 c++ 標準以外的例外則接住
		catch (std::exception& ex)
		{
			// 取得 例外訊息
			std::string errMsg = ex.what();

			// 輸出例外訊息至 UI 上的訊息對話框
			this->PostMessageToLog("Load dll failed: " + errMsg);
		}
		// 捕捉例外
		// 如果發生其他無法預期的例外則接住
		catch (...)
		{
			// 輸出例外訊息至 UI 上的訊息對話框
			this->PostMessageToLog("Load dll failed: An unknown error has occurred.");
		}
		
		// 將分析方法名稱 從 std::string 轉換成 CString
		CString cstr(name.c_str());

		// 輸出分析方法名到 UI 上的 分析方法下拉式選單中
		((CComboBox*)GetDlgItem(IDC_COMBO_ANALYSIS_METHOD))->InsertString(-1, cstr);
	}

	// 更新 UI 數據
	UpdateData(FALSE);

	//::MessageBox(NULL, L"ext dll init", L"MSG", NULL);

	// 結束回傳
	return;
}

void CSteelBarASDlg::GetAnalysisMethodDirectory(std::string folder)
{
	// 尋找檔案的 HANDLE
	HANDLE hFind;

	// 尋找檔案的結果
	WIN32_FIND_DATAA fd;

	// 路徑轉型用變數
	std::stringstream ss;
	
	// std::string 轉換成 stringstream
	ss << folder;

	// 字串搜尋用迭代子
	std::string::iterator itr = folder.end();
	
	// 迭代子往前方移動
	itr--;

	// 如果位置上的文字 不是 \\ 
	// 則在字串最後面補上 
	if (*itr != '\\')
	{
		// 補上路徑符號
		ss << '\\';
	}

	// 字串後方加入 副檔名為 dll 的字串
	ss << "*.dll";

	// 尋找第一個檔案
	hFind = FindFirstFileA(ss.str().c_str(), &fd);
	
	// 如果沒有找到任何檔案
	// 或是發生錯誤
	// 則輸出訊息
	if (hFind == INVALID_HANDLE_VALUE)
	{
		// 輸出錯誤訊息
		this->PostMessageToLog("Can not read any plug-in.");
	}

	// 一個一個將檔案讀入
	do
	{
		// 如果檔案不是 目錄 也不是 隱藏檔案
		// 則載入
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			// 取得 檔案名稱
			char* file = fd.cFileName;
			
			// 將檔案名稱轉型
			std::string str = file;
			
			// 將檔案名稱存入 外掛名稱清單中
			this->parameter.analysisMethodDllList.push_back(str);
		}

	} while (FindNextFileA(hFind, &fd)); // 尋找下一個檔案

	// 關閉尋找檔案的 HANDLE
	FindClose(hFind);

	// 結束回傳
	return;
}

void CSteelBarASDlg::OnCbnSelchangeComboAnalysisMethod()
{
	// 從 UI 上的 分析方法下拉式選單中取得當前的選取方法 ID
	this->parameter.analysisMethodIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_ANALYSIS_METHOD))->GetCurSel();
	
	// 以 ID 取得 分析方法的名稱
	CString cstr = (CA2CT)this->analysisNames[this->parameter.analysisMethodIndex]().c_str();
	
	// 增加分隔符號
	cstr += " : ";
	
	// 以 ID 取得 分析方法的版本
	cstr += (CA2CT)this->analysisVersions[this->parameter.analysisMethodIndex]().c_str();
	
	// 增加換行符號
	cstr += "\r\r\n";
	
	// 取得分析方法的簡述內容
	cstr += (CA2CT)this->analysisCaptions[this->parameter.analysisMethodIndex]().c_str();

	// 輸出 選擇的 分析方法相關訊息到 分析方法的訊息視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_ANALYSIS_METHOD_MESSAGE))->SetWindowTextW(cstr);

	// 結束回傳
	return;
}

void CSteelBarASDlg::CompressMat(cv::Mat image, std::vector<uchar> buffer)
{
	// 嘗試: 進行記憶體資料壓縮
	try
	{
		// 防呆
		// 如果 輸出位置 的 空間不是空的
		if (!buffer.empty())
		{
			// 清除 輸出空間 內部的資料
			buffer.clear();
		}
		
		// 如果 壓縮形式 是 PNG
		if (this->parameter.imageCompressType == cv::IMWRITE_PNG_COMPRESSION)
		{
			// 以 PNG 的方式 將 影像資料壓縮
			cv::imencode(".png", image, buffer, this->parameter.imageCompressParameter);
		}
		// 如果 壓縮形式 是 JPG
		else if (this->parameter.imageCompressType == cv::IMWRITE_JPEG_QUALITY)
		{
			// 以 JPG 的方式 將 影像資料壓縮
			cv::imencode(".jpg", image, buffer, this->parameter.imageCompressParameter);
		}
		// 如果 不是上述兩種 則丟出 例外
		// Limit5: 目前還沒定義 其他處理形式 2014/09/13
		else
		{
			// 丟出例外
			throw;
		}
	} // end of try
	// 捕捉例外
	// 如果發生 c++ 標準以外的例外則接住
	catch (std::exception& ex)
	{
		// 取得 例外訊息
		std::string errMsg = ex.what();

		// 輸出例外訊息至 UI 上的訊息對話框
		this->PostMessageToLog("Compress image failed: " + errMsg);
	}
	// 捕捉例外
	// 如果發生其他無法預期的例外則接住
	catch (...)
	{
		// 輸出例外訊息至 UI 上的訊息對話框
		this->PostMessageToLog("Compress image failed: An unknown error has occurred.");
	}

	// 結束回傳
	return;
}

void CSteelBarASDlg::CheckUiParameter()
{
	// 從 啟動開始 到 程式 結束 不間斷的 檢查 參數 狀態
	while (true)
	{
		// 取得 記憶體精簡模式 狀態
		int nMemoryLiteCheck = ((CButton*)GetDlgItem(IDC_CHECK_BOX_MEMORY_LITE))->GetState();
		
		// 如果 記憶體精簡模式 被選取
		if ((nMemoryLiteCheck & BST_CHECKED) != 0)
		{
			// 開啟 PNG 壓縮選項
			((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_PNG))->EnableWindow(TRUE);
			
			// 開啟 JPG 壓縮選項
			((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_JPG))->EnableWindow(TRUE);
			
			// 開啟 NONE 壓縮選項
			((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_NONE))->EnableWindow(TRUE);
			
			// 開啟 壓縮等級 滑桿
			((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BAR_IMAGE_COMPRESS_LEVEL))->EnableWindow(TRUE);
			
			// 開啟 壓縮等級 文字方塊
			((CEdit*)GetDlgItem(IDC_EDIT_TEXT_IMAGE_COMPRESS_LEVEL))->EnableWindow(TRUE);

			// 取得 壓縮等級 滑桿 的 位置(即數值)
			this->GetSliderBarValue();
		}
		// 如果 記憶體精簡模式 沒有被選取
		else
		{
			// 關閉 PNG 壓縮選項
			((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_PNG))->EnableWindow(FALSE);

			// 關閉 JPG 壓縮選項
			((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_JPG))->EnableWindow(FALSE);
			
			// 關閉 NONE 壓縮選項
			((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_NONE))->EnableWindow(FALSE);
			
			// 關閉 壓縮等級 滑桿
			((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BAR_IMAGE_COMPRESS_LEVEL))->EnableWindow(FALSE);
			
			// 關閉 壓縮等級 文字方塊
			((CEdit*)GetDlgItem(IDC_EDIT_TEXT_IMAGE_COMPRESS_LEVEL))->EnableWindow(FALSE);
		}

		// 減少效能消耗 此工作 睡眠 10 微秒
		Sleep(10);
	} // end of while(true)

	// 結束回傳
	return;
}


void CSteelBarASDlg::OnNMCustomdrawSliderBarImageCompressLevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 取得 滑桿控制 狀態
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	// 執行 滑桿 取值
	this->GetSliderBarValue();
	
	// 動作結果
	*pResult = 0;
}

void CSteelBarASDlg::GetSliderBarValue()
{
	// 取得 壓縮等級 滑桿 游標 的 位置
	int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_BAR_IMAGE_COMPRESS_LEVEL))->GetPos();
	
	// 位置 文字暫存
	CString tPos;

	// 取得 壓縮形式 PNG 選項 的 狀態
	int compressTypePNG = ((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_PNG))->GetState();
	
	// 取得 壓縮形式 JPG 選項 的 狀態
	int compressTypeJPG = ((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_JPG))->GetState();
	
	// 取得 壓縮形式 NONE 選項 的 狀態
	int compressTypeNONE = ((CButton*)GetDlgItem(IDC_RADIO_IMAGE_COMPRESS_OPTIONS_NONE))->GetState();
	
	// 如果 壓縮形式 是 PNG
	if ((compressTypePNG & BST_CHECKED) != 0)
	{
		// do nothing
		// 維持原狀
	}
	// 如果 壓縮形式 是 JPG
	else if ((compressTypeJPG & BST_CHECKED) != 0)
	{
		// 位置 的 值 放大 10 倍
		pos *= 10;
	}
	// 如果都不是的話
	else
	{
		// 壓縮等級 的 值 等於 0
		pos = 0;
	}

	// 將 整數 轉換成 cstring 存入 tPos
	tPos.Format(_T("%d"), pos);

	// 將 壓縮等級 的 數值 顯示到 壓縮等級 文字視窗
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_IMAGE_COMPRESS_LEVEL))->SetWindowTextW(tPos);
	
	// 存取 壓縮等級
	this->parameter.imageCompressLevel = pos;
}