
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
	Init();


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

/**
 *
 * @brief
 *
 * @param[in]
 *
 * @param[out]
 *
 * @return
 *
 * @note
 *
 * @date
 */

void wstr2str(const std::wstring &src, std::string &dest)
{
	char* mbs = new char[src.length() * MB_CUR_MAX + 1];

	size_t charsConverted = 0;
	wcstombs_s(&charsConverted, mbs, src.length() * MB_CUR_MAX + 1, src.c_str(), src.length() * MB_CUR_MAX + 1);

	dest = mbs;
	delete[] mbs;
}

void str2wstr(const std::string &src, std::wstring &dest)
{
	wchar_t *wcs = new wchar_t[src.length() + 1];
	size_t converted = 0;
	mbstowcs_s(&converted, wcs, src.length() + 1, src.c_str(), src.length() + 1);
	//mbstowcs(wcs, src.c_str(), src.length() + 1);

	dest = wcs;
	delete[] wcs;
}

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace) 
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void CSteelBarASDlg::Init()
{
	ValueInit();
	VariableInit();
	this->msgAndLog += "Started successfully\r\r\n";
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
	MessageBoxAutoScrollToBottom();
}

void CSteelBarASDlg::ValueInit()
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_V_TEMPLATE_SIZE_X))->SetWindowTextW(L"40");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_V_TEMPLATE_SIZE_Y))->SetWindowTextW(L"40");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_H_TEMPLATE_SIZE_X))->SetWindowTextW(L"210");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_H_TEMPLATE_SIZE_Y))->SetWindowTextW(L"40");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_CHESSBOARD_SIZE_X))->SetWindowTextW(L"5.0");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_CHESSBOARD_SIZE_Y))->SetWindowTextW(L"5.0");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_PATTERN_SIZE_X))->SetWindowTextW(L"7");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_PATTERN_SIZE_Y))->SetWindowTextW(L"25");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_NUMBER_OF_V_TEMPLATE))->SetWindowTextW(L"2");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_NUMBER_OF_H_TEMPLATE))->SetWindowTextW(L"100");
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_DIAMETER))->SetWindowTextW(L"12.5");
	return;
}


#define L_TEMPLATE_FOLDER_NAME L"template"
#define L_CTRLPOINTS_FOLDER_NAME L"ctrlPoints"
#define L_CHESSBOARDIMAGE_FOLDER_NAME L"chessboardImage"
#define L_RECTIFY_FOLDER_NAME L"rectify"
#define L_TRACE_FOLDER_NAME L"trace"
void CSteelBarASDlg::DirectoryInit()
{
	if (this->parameter.outputPath == "")
	{
		return;
	}
	std::wstring stemp = std::wstring(this->parameter.outputPath.begin(), this->parameter.outputPath.end());
	LPCWSTR sw = stemp.c_str();
	SetCurrentDirectory(sw);
	LPSECURITY_ATTRIBUTES lpSecurity;
	lpSecurity = NULL;
	std::string templateFolderName = "template";
	if (CreateDirectory(L_TEMPLATE_FOLDER_NAME, lpSecurity) == FALSE)
	{
		if (!(GetLastError() == ERROR_ALREADY_EXISTS)) MessageBoxA(NULL, "can not create template folder.", "Warning!", NULL);
	}

	LPSECURITY_ATTRIBUTES lpSecurity2;
	lpSecurity2 = NULL;
	std::string ctrlPointFolderName = "ctrlPoints";
	if (CreateDirectory(L_CTRLPOINTS_FOLDER_NAME, lpSecurity2) == FALSE)
	{
		if (!(GetLastError() == ERROR_ALREADY_EXISTS)) MessageBoxA(NULL, "can not create ctrlPoints folder.", "Warning!", NULL);
	}

	LPSECURITY_ATTRIBUTES lpSecurity3;
	lpSecurity3 = NULL;
	std::string chessboardImageFolderName = "chessboardImage";
	if (CreateDirectory(L_CHESSBOARDIMAGE_FOLDER_NAME, lpSecurity3) == FALSE)
	{
		if (!(GetLastError() == ERROR_ALREADY_EXISTS)) MessageBoxA(NULL, "can not create chessboardImage folder.", "Warning!", NULL);
	}

	LPSECURITY_ATTRIBUTES lpSecurity4;
	lpSecurity4 = NULL;
	std::string rectifyFolderName = "rectify";
	if (CreateDirectory(L_RECTIFY_FOLDER_NAME, lpSecurity4) == FALSE)
	{
		if (!(GetLastError() == ERROR_ALREADY_EXISTS)) MessageBoxA(NULL, "can not create rectf folder.", "Warning!", NULL);
	}

	LPSECURITY_ATTRIBUTES lpSecurity5;
	lpSecurity5 = NULL;
	std::string traceFolderName = "trace";
	if (CreateDirectory(L_TRACE_FOLDER_NAME, lpSecurity5) == FALSE)
	{
		if (!(GetLastError() == ERROR_ALREADY_EXISTS)) MessageBoxA(NULL, "can not create trace folder.", "Warning!", NULL);
	}

	return;
}

void CSteelBarASDlg::VariableInit()
{
	this->msgAndLog = "";
	this->camCalib;
	this->parameter.memoryLite = FALSE;
	this->parameter.mute = FALSE;
	this->parameter.calibDataImprotFromExternal = FALSE;

	return;
}

int CSteelBarASDlg::SelectPath(int pathItemId, int ListItemId, bool calibOrExp)
{
	CString filter("JPEG Files (*.jpg, *.jpeg)|*.jpg; *.jpeg|PNG Files (*.png)|*.png|BITMAP Files (*.bmp)|*.bmp|TIFF Files (*.tif)|*.tif||");
	CString filePath, strBuf;
	POSITION pos = NULL;
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, filter);
	int err = 0, lbErr = 0;

	if (!err)
	{
		try
		{
			selDlg.GetOFN().lpstrFile = strBuf.GetBuffer(MAX_PATH * 100);
			selDlg.GetOFN().nMaxFile = MAX_PATH * 100;
		}
		catch (...)
		{
			err = 1;
		}
	}

	if (!err) if (selDlg.DoModal() != IDOK) err = 1;
	if (!err) if ((pos = selDlg.GetStartPosition()) == NULL) err = 1;
	if (!err)
	{
		if (calibOrExp) this->parameter.calibImageFileName.clear();
		else this->parameter.expImageFileName.clear();
		((CListBox*)GetDlgItem(ListItemId))->ResetContent();
		((CEdit*)GetDlgItem(pathItemId))->SetWindowTextW(selDlg.GetPathName());
		std::string tmpPathName = (CT2CA)selDlg.GetPathName();
		if (calibOrExp) this->parameter.calibImagePath = tmpPathName;
		else this->parameter.expImagePath = tmpPathName;

		while (pos)
		{
			filePath = selDlg.GetNextPathName(pos);
			if (!err)
			{
				std::string tmpName((CT2CA)filePath);
				if (calibOrExp) this->parameter.calibImageFileName.push_back(tmpName);
				else this->parameter.expImageFileName.push_back(tmpName);
				CString filename = filePath.Right(filePath.GetLength() - filePath.ReverseFind(_T('\\')) - 1);
				lbErr = ((CListBox*)GetDlgItem(ListItemId))->InsertString(-1, filename);
				std::string tmpName2 = (CT2CA)filename;
				if (!calibOrExp) this->parameter.expImageFileNameWithoutPath.push_back(tmpName2);
				if (lbErr == LB_ERR || lbErr == LB_ERRSPACE) err = 1;
			}
			if (err) break;
		}
		UpdateData(FALSE);
	}
	strBuf.ReleaseBuffer();
	return err;
}

void CSteelBarASDlg::SelectFolder()
{
	BROWSEINFO   bi;
	ZeroMemory(&bi, sizeof(bi));
	TCHAR   szDisplayName[MAX_PATH];
	szDisplayName[0] = L'';

	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDisplayName;
	bi.lpszTitle = _T("Please select a folder for storing received files :");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lParam = NULL;
	bi.iImage = 0;

	LPITEMIDLIST   pidl = SHBrowseForFolder(&bi);
	
	if (NULL != pidl)
	{
		BOOL bRet = SHGetPathFromIDList(pidl, this->szPathName);
		if (FALSE == bRet)
			return;
		//AfxMessageBox(szPathName);
	}
	
}


void CSteelBarASDlg::OnBnClickedButtonCalibImageSelectPath()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int err = SelectPath(IDC_EDIT_TEXT_CALIB_IMAGE_PATH, IDC_LIST_BOX_CALIB_IMAGE_PATH, TRUE);

	if ((this->parameter.calibImageFileName.size() > 0) && !err) this->msgAndLog += "selected calibration images\r\r\n";
	else this->msgAndLog += "No calibration image selected\r\r\n";
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
	MessageBoxAutoScrollToBottom();
	UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);
	int count = ((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->GetCount();
	if (count > 0) this->parameter.calibDataImprotFromExternal = false;
}


void CSteelBarASDlg::OnBnClickedButtonExpImageSelectPath()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int err = SelectPath(IDC_EDIT_TEXT_EXP_IMAGE_PATH, IDC_LIST_BOX_EXP_IMAGE_PATH, FALSE);

	if ((this->parameter.calibImageFileName.size() > 0) && !err) this->msgAndLog += "selected exp images\r\r\n";
	else this->msgAndLog += "No exp image selected\r\r\n";
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
	MessageBoxAutoScrollToBottom();
	UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);
}


UINT workThread(LPVOID param)
{
	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;
	dlg->m_progressCtrlGlobal.SetRange32(0, 1000);
	//COLORREF clrBar = RGB(255, 0, 0);
	//dlg->m_progressCtrlGlobal.SetBarColor(clrBar);
	
	dlg->SetParameterFromUi();
	DWORD dwRet = WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		dlg->PostMessageToLog("Work thread was killed at \"set parameter\" by user.");
		dlg->parameter.terminateThread = FALSE;
		return 1;
	}
	dlg->m_progressCtrlGlobal.SetPos(100);
	dlg->CalibrateImage();
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		dlg->PostMessageToLog("Work thread was killed at \"calibrate image\" by user.");
		dlg->parameter.terminateThread = FALSE;
		return 1;
	}
	dlg->m_progressCtrlGlobal.SetPos(300);
	dlg->GenerateObjectPoints();
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		dlg->PostMessageToLog("Work thread was killed at \"generate object points\" by user.");
		dlg->parameter.terminateThread = FALSE;
		return 1;
	}
	dlg->m_progressCtrlGlobal.SetPos(400);
	dlg->GenerateImagePoints();
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		dlg->PostMessageToLog("Work thread was killed at \"generate image points\" by user.");
		dlg->parameter.terminateThread = FALSE;
		return 1;
	}
	dlg->m_progressCtrlGlobal.SetPos(500);
	dlg->GenerateRectifyImage();
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		dlg->PostMessageToLog("Work thread was killed at \"generate rectify images\" by user.");
		dlg->parameter.terminateThread = FALSE;
		return 1;
	}
	dlg->m_progressCtrlGlobal.SetPos(800);
	dlg->Calculate();
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		dlg->PostMessageToLog("Work thread was killed at \"calculate\" by user.");
		dlg->parameter.terminateThread = FALSE;
		return 1;
	}
	dlg->m_progressCtrlGlobal.SetPos(875);
	dlg->WriteCalcData();
	WaitForSingleObject(dlg->parameter.m_hKillEvent, 0);
	if (dlg->parameter.terminateThread || (dwRet == WAIT_OBJECT_0))
	{
		dlg->PostMessageToLog("Work thread was killed at \"write calculation data\" by user.");
		dlg->parameter.terminateThread = FALSE;
		return 1;
	}
	dlg->m_progressCtrlGlobal.SetPos(999);
	//clrBar = RGB(0, 255, 0);
	//dlg->m_progressCtrlGlobal.SetBarColor(clrBar);
	return 0;
}

UINT msgThread(LPVOID param)
{
	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;
	dlg->message();
	return 0;
}

UINT checkWorkThread(LPVOID param)
{

	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;
	DWORD threadInfo;

	::WaitForSingleObject(dlg->parameter.m_pThread->m_hThread, INFINITE);
	//dlg->parameter.m_pThread->ExitInstance();
	dlg->parameter.m_pThread = NULL;
	dlg->m_buttonRun.EnableWindow(FALSE);
	dlg->m_buttonStop.EnableWindow(FALSE);
	dlg->m_buttonClear.EnableWindow(TRUE);
	dlg->m_buttonCalibImgSelPath.EnableWindow(TRUE);
	dlg->m_buttonExpImgSelPath.EnableWindow(TRUE);
	dlg->m_buttonSelOutputPath.EnableWindow(TRUE);
	dlg->m_buttonLoadExtCalibData.EnableWindow(TRUE);
	dlg->m_listCalibImagePath.EnableWindow(TRUE);

	dlg->PostMessageToLog("Done!");
	


	return 0;
}

UINT terminateWorkThread(LPVOID param)
{
	CSteelBarASDlg* dlg = (CSteelBarASDlg*)param;
	DWORD threadInfo;

	while (true)
	{
		::GetExitCodeThread(dlg->parameter.m_pThread->m_hThread, &threadInfo);
		if (threadInfo != STILL_ACTIVE)
		{
			dlg->parameter.m_pThread->ExitInstance();
			dlg->parameter.m_pThread = NULL;
			//dlg->m_buttonRun.EnableWindow(TRUE);
			//dlg->m_buttonStop.EnableWindow(TRUE);
			//dlg->m_buttonClear.EnableWindow(TRUE);
			dlg->PostMessageToLog("work thread end");
			break;
		}
		Sleep(100);
	}

	return 0;
}




void CSteelBarASDlg::OnBnClickedButtonRun()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int expCount = ((CListBox*)GetDlgItem(IDC_LIST_BOX_EXP_IMAGE_PATH))->GetCount();
	int calibCount = ((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->GetCount();
	CString extCalibFile;
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_LOAD_EXTERNAL_CALIB_DATA))->GetWindowTextW(extCalibFile);
	CString outpath;
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_OUTPUT_PATH))->GetWindowTextW(outpath);

	if (expCount <= 0)
	{
		this->PostMessageToLog("No exp data.");
		return;
	}

	if (extCalibFile == L"")
	{
		if (calibCount <= 0)
		{
			this->PostMessageToLog("No calib data.");
			return;
		}
	}
	if (outpath == L"")
	{
		this->PostMessageToLog("No output path");
		return;
	}
	
	try
	{
		this->parameter.m_pThread = AfxBeginThread(workThread, (LPVOID)this, NULL);
		this->parameter.m_hKillEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		this->parameter.m_pCheckWorkThread = AfxBeginThread(checkWorkThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	}
	catch (std::exception& ex)
	{
		CString exception;
		exception = "Exception: ";
		exception += ex.what();

		this->PostMessageToLog(exception);
		return;
	}
	catch (...)
	{
		this->PostMessageToLog("An unknown error has occurred.");
		return;
	}

}

void CSteelBarASDlg::SetParameterFromUi()
{
	CString cstr;
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"setting parameter");
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 10);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);

	((CEdit*)GetDlgItem(IDC_EDIT_BOX_V_TEMPLATE_SIZE_X))->GetWindowTextW(cstr);
	this->parameter.vTemplateSizeX = _ttoi(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_V_TEMPLATE_SIZE_Y))->GetWindowTextW(cstr);
	this->parameter.vTemplateSizeY = _ttoi(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_H_TEMPLATE_SIZE_X))->GetWindowTextW(cstr);
	this->parameter.hTemplateSizeX = _ttoi(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_H_TEMPLATE_SIZE_Y))->GetWindowTextW(cstr);
	this->parameter.hTemplateSizeY = _ttoi(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_CHESSBOARD_SIZE_X))->GetWindowTextW(cstr);
	this->parameter.cellSizeDx = _ttof(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_CHESSBOARD_SIZE_Y))->GetWindowTextW(cstr);
	this->parameter.cellSizeDy = _ttof(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_PATTERN_SIZE_X))->GetWindowTextW(cstr);
	this->parameter.patternSizeNx = _ttoi(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_PATTERN_SIZE_Y))->GetWindowTextW(cstr);
	this->parameter.patternSizeNy = _ttoi(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_NUMBER_OF_V_TEMPLATE))->GetWindowTextW(cstr);
	this->parameter.nVtemplate = _ttoi(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_NUMBER_OF_H_TEMPLATE))->GetWindowTextW(cstr);
	this->parameter.nHtemplate = _ttoi(cstr);
	((CEdit*)GetDlgItem(IDC_EDIT_BOX_DIAMETER))->GetWindowTextW(cstr);
	this->parameter.diameter = _ttof(cstr);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(4);
	int nMemoryLiteCheck = ((CButton*)GetDlgItem(IDC_CHECK_BOX_MEMORY_LITE))->GetState();
	if ((nMemoryLiteCheck & BST_CHECKED) != 0)
	{
		this->parameter.memoryLite = TRUE;
	}
	else
	{
		this->parameter.memoryLite = FALSE;
	}

	int nMuteCheck = ((CButton*)GetDlgItem(IDC_CHECK_BOX_MUTE))->GetState();
	if ((nMuteCheck & BST_CHECKED) != 0)
	{
		this->parameter.mute = TRUE;
	}
	else
	{
		this->parameter.mute = FALSE;
	}

	CString checkFile;
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_LOAD_EXTERNAL_CALIB_DATA))->GetWindowTextW(checkFile);
	if (checkFile != L"")
	{
		this->parameter.hasExternalCalibData = true;
	}

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(8);
	if (!this->parameter.mute)
	{
		this->msgAndLog += "Set all parameter\r\r\n";
		((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
		MessageBoxAutoScrollToBottom();
	}


	((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_CALIB_IMAGE_SELECT_PATH))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_EXP_IMAGE_SELECT_PATH))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_SELECT_OUTPUT_PATH))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_LOAD_EXTERNAL_CALIB_DATA))->EnableWindow(FALSE);

	((CListCtrl*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->EnableWindow(FALSE);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(10);
	
	//UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);

	return;
}

void CSteelBarASDlg::CalibrateImage()
{
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"calibrating");
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 100);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);
	
	
	if (this->parameter.hasExternalCalibData)
	{
		this->LoadExternalCalibData();
		return;
	}
	
	int returnVal;

	double cellSizeDx, cellSizeDy;
	int patternSizeNx, patternSizeNy;
	cv::Size photoSize;
	cellSizeDx = this->parameter.cellSizeDx;
	cellSizeDy = this->parameter.cellSizeDy;
	patternSizeNx = this->parameter.patternSizeNx;
	patternSizeNy = this->parameter.patternSizeNy;

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(10);


	this->camCalib.setCellSize(cellSizeDx, cellSizeDy);
	this->camCalib.setPatternSize(patternSizeNx, patternSizeNy);
	
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(20);

	if (!this->parameter.mute) this->PostMessageToLog("Calibration images are loading...");
	this->parameter.calibImage.clear();
	this->parameter.calibImage.resize(this->parameter.calibImageFileName.size());

	for (size_t i = 0; i < this->parameter.calibImageFileName.size(); i++)
	{
		returnVal = this->camCalib.addCalibImg(this->parameter.calibImageFileName[i]);
		//CString tmp = (CA2CT)this->parameter.calibImageFileName[i].c_str();
		if (!this->parameter.mute) this->PostMessageToLog(this->parameter.calibImageFileName[i]);
		if (!this->parameter.memoryLite) this->parameter.calibImage[i] = cv::imread(this->parameter.calibImageFileName[i]);
	}

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(30);


	returnVal = this->camCalib.getPhotoSize(photoSize);
	
	if (!this->parameter.mute) this->PostMessageToLog("Calibrating...");
	this->camCalib.calibrate();
	
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(60);

	int imageIdOfGlobalCoord;
	imageIdOfGlobalCoord = this->parameter.rectifyBaseImageId;
	this->parameter.camMatrix = this->camCalib.getCamMatrix(0);
	this->parameter.disCoeffs = this->camCalib.getDisCoeffs(0);
	
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(70);

	returnVal = this->camCalib.getChessboardRVecs(imageIdOfGlobalCoord, this->parameter.coordR, 0);
	returnVal = this->camCalib.getChessboardTVecs(imageIdOfGlobalCoord, this->parameter.coordT, 0);

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(85);

	CString cstr;

	this->msgAndLog += "Fx/Fy: ";
	cstr.Format(_T("%.3f"), this->parameter.camMatrix.at<double>(0, 0));
	this->msgAndLog += cstr + L" / ";
	cstr.Format(_T("%.3f"), this->parameter.camMatrix.at<double>(1, 1));
	this->msgAndLog += cstr;
	this->msgAndLog += "\r\r\n";

	this->msgAndLog += "Cx/Cy: ";
	cstr.Format(_T("%.3f"), this->parameter.camMatrix.at<double>(0, 2));
	this->msgAndLog += cstr + L" / ";
	cstr.Format(_T("%.3f"), this->parameter.camMatrix.at<double>(1, 2));
	this->msgAndLog += cstr;
	this->msgAndLog += "\r\r\n";

	this->msgAndLog += "Calib. error: ";
	cstr.Format(_T("%.3f"), this->camCalib.getCamCalibErr(0));
	this->msgAndLog += cstr;
	this->msgAndLog += "\r\r\n";

	this->msgAndLog += this->camCalib.getErrorMessage();
	this->msgAndLog += "\r\r\n";

	//((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(90);
	
	
	
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(100);

	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
	MessageBoxAutoScrollToBottom();
	if (!this->parameter.mute) this->PostMessageToLog("Calibrate done\n");
	//UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);

	return;
}

void CSteelBarASDlg::LoadExternalCalibData()
{
	cv::FileStorage cvfs(this->parameter.externalCalibDataPath.c_str(), CV_STORAGE_READ);
	cv::FileNode node(cvfs.fs, NULL);

	this->parameter.cellSizeDx = node["cellSizeDx"];
	this->parameter.cellSizeDy = node["cellSizeDy"];
	this->parameter.patternSizeNx = node["patternSizeNx"];
	this->parameter.patternSizeNy = node["patternSizeNy"];
	int chessImgPtsSize = node["chessImagePointsSize"];

	cv::FileNode fn = node[std::string("chessImagePoints")];
	cv::FileNodeIterator it = fn.begin(), it_end = fn.end();
	int idx = 0;
	this->parameter.chessImagePoints.resize(chessImgPtsSize);


	cv::read(node["coordR"], this->parameter.coordR);
	cv::read(node["coordT"], this->parameter.coordT);
	cv::read(node["camMatrix"], this->parameter.camMatrix);
	cv::read(node["discoeffs"], this->parameter.disCoeffs);

	for (int i = 0; i < chessImgPtsSize; i++)
	{
		std::stringstream ss;
		ss << i;
		std::string ciptsX = "chessImagePoints_X_" + ss.str();
		this->parameter.chessImagePoints[i].x = node[ciptsX];
		std::string ciptsY = "chessImagePoints_Y_" + ss.str();
		this->parameter.chessImagePoints[i].y = node[ciptsY];
	}


	this->PostMessageToLog("Calibration data loaded");
	return;
}

void CSteelBarASDlg::GenerateObjectPoints()
{
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"generating obj. pts");
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 10);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);

	int returnVal;
	
	
	CString cstr;
	
	returnVal = this->camCalib.getCornersImagePoints(this->parameter.rectifyBaseImageId, this->parameter.chessImagePoints);
	
	this->parameter.physicalDistance = (float)sqrt(pow(this->parameter.cellSizeDx * (this->parameter.patternSizeNx - 1), 2) + pow(this->parameter.cellSizeDy * (this->parameter.patternSizeNy - 1), 2));
	this->parameter.imageDistance = (float)sqrt(pow(this->parameter.chessImagePoints[0].x
		- this->parameter.chessImagePoints[this->parameter.patternSizeNx * this->parameter.patternSizeNy - 1].x, 2)
		+ pow(this->parameter.chessImagePoints[0].y
		- this->parameter.chessImagePoints[this->parameter.patternSizeNx * this->parameter.patternSizeNy - 1].y, 2));
	this->parameter.rpx = this->parameter.physicalDistance / this->parameter.imageDistance * 0.5f;

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(20);

	this->msgAndLog += "rpx(";
	cstr.Format(_T("%.3f"), this->parameter.rpx);
	this->msgAndLog += cstr;
	this->msgAndLog += ") = phyDist(";
	cstr.Format(_T("%.3f"), this->parameter.physicalDistance);
	this->msgAndLog += cstr;
	this->msgAndLog += ") / imgDist(";
	cstr.Format(_T("%.3f"), this->parameter.imageDistance);
	this->msgAndLog += cstr;
	this->msgAndLog += ") * 0.5\r\r\n";


	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(50);
	//int rectfImageHeight, rectfImageWidth; // size of rectf. image that we will generate
	int ix, iy;
	float x, y;
	this->parameter.rectfImageHeight = (int)(this->parameter.cellSizeDy * (this->parameter.patternSizeNy - 1) / this->parameter.rpx + 1.0);
	this->parameter.rectfImageWidth = (int)(this->parameter.cellSizeDx * (this->parameter.patternSizeNx - 1) / this->parameter.rpx + 1.0);
	this->parameter.gridObjectPoints3D = cv::Mat(this->parameter.rectfImageHeight, this->parameter.rectfImageWidth, CV_32FC3 /* float, float, float */);

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
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(90);
	this->msgAndLog += "Rectify image with size of ";
	cstr.Format(_T("%i"), this->parameter.rectfImageWidth);
	this->msgAndLog += cstr;
	this->msgAndLog += " × ";
	cstr.Format(_T("%i"), this->parameter.rectfImageHeight);
	this->msgAndLog += cstr;
	this->msgAndLog += "\r\r\n";
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
	MessageBoxAutoScrollToBottom();
	//UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);

	OutputCalibrationData();

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(100);
	return;
}


void CSteelBarASDlg::GenerateImagePoints()
{
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"generating images pts.");
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 10);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);


	this->parameter.gridImagePoints = cv::Mat(this->parameter.rectfImageHeight, this->parameter.rectfImageWidth, CV_32FC2 /* float */);
	
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(5);

	cv::projectPoints(this->parameter.gridObjectPoints3D.reshape(0 /* channel not changed */, 1),
		this->parameter.coordR, this->parameter.coordT, this->parameter.camMatrix, this->parameter.disCoeffs,
		this->parameter.gridImagePoints.reshape(0 /* channel not changed */, 1));

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(8);

	if (!this->parameter.mute) this->msgAndLog += "\nGenerate grid image points\r\r\n";
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
	MessageBoxAutoScrollToBottom();
	//UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(10);
	return;
}

void CSteelBarASDlg::GenerateRectifyImage()
{
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"rectifying");
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 1000);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);


	DirectoryInit();
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(100);
	if (!this->parameter.mute) this->PostMessageToLog("Rectify images are generating...");
	this->parameter.rectifyImageFileName.clear();
	this->parameter.rectifyImageFileName.resize(this->parameter.expImageFileName.size());
	this->parameter.expImage.clear();
	this->parameter.expImage.resize(this->parameter.expImageFileName.size());
	this->parameter.rectifyImage.clear();
	this->parameter.rectifyImage.resize(this->parameter.expImageFileName.size());
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(199);

	if (!this->parameter.mute) cv::namedWindow("rectify");
	for (size_t i = 0; i < this->parameter.rectifyImageFileName.size(); i++)
	{
		//std::string outputPath = ReplaceString(this->parameter.outputPath, "\\", "\\\\");
		// 
		this->parameter.rectifyImageFileName[i] = this->parameter.outputPath + "\\rectify\\rectify_" + this->parameter.expImageFileNameWithoutPath[i];
		if (!this->parameter.mute) this->PostMessageToLog(this->parameter.rectifyImageFileName[i]);
		cv::Mat expImage = cv::imread(this->parameter.expImageFileName[i]); 
		if (expImage.empty())
		{
			if (!this->parameter.mute) this->PostMessageToLog("can not read image: " + this->parameter.expImageFileName[i]);
		}
		if (!this->parameter.memoryLite) this->parameter.expImage[i] = expImage;
		cv::Mat rectifyImage;
		cv::Mat emptyMat;
		cv::remap(expImage, rectifyImage, this->parameter.gridImagePoints, emptyMat, cv::INTER_LINEAR);
		if (!this->parameter.memoryLite) this->parameter.rectifyImage[i] = rectifyImage;
		std::string oFileName = this->parameter.rectifyImageFileName[i];
		try
		{
			bool bSuccess = cv::imwrite(oFileName, rectifyImage);
			if (!bSuccess) if (!this->parameter.mute) this->PostMessageToLog("Fail writing image to storage: " + oFileName);
		}
		catch (std::runtime_error& ex)
		{
			std::string ss = std::string(ex.what());
			if (!this->parameter.mute) this->PostMessageToLog("Exception converting image to JPG format: " + ss);
		}
		catch(std::exception& ex)
		{
			std::string ss = std::string(ex.what());
			if (!this->parameter.mute) this->PostMessageToLog("Exception: " + ss);
		}
		if (!this->parameter.mute)
		{
			cv::imshow("rectify", rectifyImage);
			cv::waitKey(5);
		}

		int procCount = 199 + (int)((double)800 * (double)((double)i / (double)this->parameter.rectifyImageFileName.size()));
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(procCount);
	}
	if (!this->parameter.mute) cv::destroyWindow("rectify");
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(1000);
	if (!this->parameter.mute)
	{
		this->msgAndLog += "Rectify images are generated\r\r\n";
		((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
		MessageBoxAutoScrollToBottom();
	}
	//UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);

	return;
}

void CSteelBarASDlg::Calculate()
{
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"computing");
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 1000);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);

	this->parameter.vPointsData.clear();
	this->parameter.vPointsData.resize(this->parameter.expImageFileName.size());

	this->parameter.hPointsData.clear();
	this->parameter.hPointsData.resize(this->parameter.expImageFileName.size());




	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(1000);
	return;

}

void CSteelBarASDlg::WriteCalcData()
{
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetWindowTextW(L"writing data");
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetRange32(0, 1000);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);


	if ((this->parameter.vPointsData.size() <= 0) || (this->parameter.hPointsData.size() <= 0))
	{
		if (!this->parameter.mute) this->PostMessageToLog("Output: no data");
		return;
	}

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(50);

	std::ofstream oFile("data.csv", std::ios::out|std::ios::trunc);

	oFile << "No.,,";
	for (size_t i = 0; i < this->parameter.vPointsData.size(); i++)
	{
		oFile << "vPoint" << i << " x," << "vPoint" << i << " y,,";
	}

	for (size_t i = 0; i < this->parameter.hPointsData.size(); i++)
	{
		oFile << "hPoint" << i << " x," << "hPoint" << i << " y,,";
	}
	oFile << std::endl;



	
	for (size_t i = 0; i < this->parameter.expImageFileName.size(); i++)
	{
		oFile << i << ",,";
		for (size_t j = 0; j < this->parameter.vPointsData[i].size(); j++)
		{
			oFile << this->parameter.vPointsData[i][j].x << "," << this->parameter.vPointsData[i][j].y;
			if (i < this->parameter.vPointsData[i].size() - 1) oFile << ",,";
			else oFile << ",,,";
		}

		for (size_t j = 0; j < this->parameter.hPointsData[i].size(); j++)
		{
			oFile << this->parameter.hPointsData[i][j].x << "," << this->parameter.hPointsData[i][j].y;
			if (i < this->parameter.hPointsData[i].size() - 1) oFile << ",,";
			else oFile << ",,,";
		}
		oFile << std::endl;
		int procCount = 50 + (int)((double)950 * (double)((double)i / (double)this->parameter.expImageFileName.size()));
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(procCount);
	}
	
	oFile.close();
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(1000);
	return;
}


void CSteelBarASDlg::OnBnClickedButtonSelectOutputPath()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	SelectFolder();

	wstr2str(this->szPathName, this->parameter.outputPath);

	CString cstr(this->parameter.outputPath.c_str());
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_OUTPUT_PATH))->SetWindowTextW(cstr);

	if (!this->parameter.mute)
	{
		this->msgAndLog += "output path: ";
		this->msgAndLog += this->parameter.outputPath.c_str();
		this->msgAndLog += "\r\r\n";
		((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
		MessageBoxAutoScrollToBottom();
	}
	//UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);
	
	return;
}


void CSteelBarASDlg::OnBnClickedButtonStop()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	this->parameter.terminateThread = TRUE;
	SetEvent(this->parameter.m_hKillEvent);
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(FALSE);
	this->PostMessageToLog("Wait for work thread terminate...");
	this->parameter.m_pThread->PostThreadMessageW(WM_QUIT, 0, 0);

	//AfxBeginThread(terminateWorkThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);


	//::WaitForSingleObject(this->parameter.m_pThread, INFINITE);
	//::TerminateThread(this->parameter.m_pThread, 0);
	//CloseHandle(this->parameter.m_pThread->m_hThread);
	this->parameter.m_pThread->m_hThread = 0;
	this->parameter.m_pThread = 0;
	if (!this->parameter.mute)
	{
		this->msgAndLog += "Terminated by user\r\r\n";
		((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);
		MessageBoxAutoScrollToBottom();
	}
	//UpdateData(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(-1, -1);

	return;
}

void CSteelBarASDlg::message()
{
	this->msgAndLog += this->tMsgLog;
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetWindowTextW(this->msgAndLog);

	MessageBoxAutoScrollToBottom();
	return;
}

void CSteelBarASDlg::PostMessageToLog(CString msg)
{
	this->tMsgLog = msg;
	this->tMsgLog += L"\r\r\n";
	this->parameter.m_pMsgThread = AfxBeginThread(msgThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	::WaitForSingleObject(this->parameter.m_pMsgThread, INFINITE);
	Sleep(5);
}

void CSteelBarASDlg::PostMessageToLog(std::string msg)
{
	CString tmp = (CA2CT)msg.c_str();
	this->tMsgLog = tmp;
	this->tMsgLog += L"\r\r\n";
	this->parameter.m_pMsgThread = AfxBeginThread(msgThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	::WaitForSingleObject(this->parameter.m_pMsgThread, INFINITE);
	Sleep(5);
}

void CSteelBarASDlg::OnLbnDblclkListBoxCalibImagePath()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString str;

	UpdateData();
	this->parameter.rectifyBaseImageId = ((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->GetCurSel();
	if (this->parameter.rectifyBaseImageId == LB_ERR)
	{
		return;
	}

	((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->GetText(this->parameter.rectifyBaseImageId, str);

	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_RECTIFY_BASE_IMAGE))->SetWindowTextW(str);

	IplImage* image = NULL;
	if (image) cvReleaseImage(&image);
	std::string path = this->parameter.calibImageFileName[this->parameter.rectifyBaseImageId];
	image = cvLoadImage(path.c_str(), 1);
	DrawImageToHDC(image, IDC_PICTURE_CONTROL_BASE_IMAGE);

	return;
}


void CSteelBarASDlg::DrawImageToHDC(IplImage* img, UINT Id)
{
	CDC* pDC = GetDlgItem(Id)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(Id)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);

	return;
}

void CSteelBarASDlg::OnBnClickedButtonClear()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	VariableClear();
	uiDataClear();


	if (!this->parameter.mute) this->PostMessageToLog("Clear");
	return;
}


void CSteelBarASDlg::OutputCalibrationData()
{
	
	
	char filename[MAX_PATH];
	sprintf_s(filename, "%s\\calibData.xml", this->parameter.outputPath.c_str());
	
	cv::FileStorage cvfsCalibData(filename, CV_STORAGE_WRITE);

	cv::write(cvfsCalibData, "cellSizeDx", this->parameter.cellSizeDx);
	cv::write(cvfsCalibData, "cellSizeDy", this->parameter.cellSizeDy);
	cv::write(cvfsCalibData, "patternSizeNx", this->parameter.patternSizeNx);
	cv::write(cvfsCalibData, "patternSizeNy", this->parameter.patternSizeNy);
	int chessImgPtsSize = this->parameter.chessImagePoints.size();
	cv::write(cvfsCalibData, "chessImagePointsSize", chessImgPtsSize);
	cv::write(cvfsCalibData, "coordR", this->parameter.coordR);
	cv::write(cvfsCalibData, "coordT", this->parameter.coordT);
	cv::write(cvfsCalibData, "camMatrix", this->parameter.camMatrix);
	cv::write(cvfsCalibData, "discoeffs", this->parameter.disCoeffs);

	
	for (int i = 0; i < chessImgPtsSize; i++)
	{
		std::stringstream ss;
		ss << i;
		std::string ciptsX = "chessImagePoints_X_" + ss.str();
		cv::write(cvfsCalibData, ciptsX, this->parameter.chessImagePoints[i].x);
		std::string ciptsY = "chessImagePoints_Y_" + ss.str();
		cv::write(cvfsCalibData, ciptsY, this->parameter.chessImagePoints[i].y);
	}

	cvfsCalibData.release();
	//cv::WriteStructContext ws(cvfsCalibData, "chessImagePoints", CV_NODE_SEQ);
	//for (size_t i = 0; i < this->parameter.chessImagePoints.size(); i++)
	//{
	//	cv::write(cvfsCalibData, "pt", this->parameter.chessImagePoints[i]);
	//}
	//

	return;

}

void CSteelBarASDlg::OnBnClickedButtonLoadExternalCalibData()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString filter("XML File (*.xml)|*.xml||");
	CString filePath, strBuf;
	POSITION pos = NULL;
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	int err = 0, lbErr = 0;
	if (!err)
	{
		try
		{
			selDlg.GetOFN().lpstrFile = strBuf.GetBuffer(MAX_PATH * 100);
			selDlg.GetOFN().nMaxFile = MAX_PATH * 100;
		}
		catch (...)
		{
			err = 1;
		}
	}
	if (!err) if (selDlg.DoModal() != IDOK) err = 1;
	if (!err) if ((pos = selDlg.GetStartPosition()) == NULL) err = 1;
	if (!err)
	{
		while (pos)
		{
			filePath = selDlg.GetNextPathName(pos);
			if (!err)
			{
				std::string tmpName((CT2CA)filePath);
				this->parameter.externalCalibDataPath = tmpName;
				CString filename = filePath.Right(filePath.GetLength() - filePath.ReverseFind(_T('\\')) - 1);
				((CEdit*)GetDlgItem(IDC_EDIT_TEXT_LOAD_EXTERNAL_CALIB_DATA))->SetWindowTextW(filename);
				std::string tmpName2 = (CT2CA)filename;
				this->parameter.externalCalibDataName = tmpName2;
				if (lbErr == LB_ERR || lbErr == LB_ERRSPACE) err = 1;
				if (!this->parameter.mute) this->PostMessageToLog("Improt: " + this->parameter.externalCalibDataName);
				this->parameter.calibDataImprotFromExternal = true;
			}
			if (err) break;
		}
		UpdateData(FALSE);
	}

	strBuf.ReleaseBuffer();

	return;
}


void CSteelBarASDlg::VariableClear()
{
	this->parameter.calibImage.clear();
	this->parameter.expImage.clear();
	this->parameter.rectifyImage.clear();
	this->parameter.chessImagePoints.clear();
	this->parameter.expImageFileName.clear();
	this->parameter.expImageFileNameWithoutPath.clear();
	this->parameter.hPointsData.clear();
	this->parameter.vPointsData.clear();
	this->parameter.rectifyImageFileName.clear();
	
	this->parameter.calibDataImprotFromExternal = FALSE;
	this->parameter.memoryLite = FALSE;
	this->parameter.mute = FALSE;
	this->camCalib.reset();
	VariableInit();

	return;
}

void CSteelBarASDlg::uiDataClear()
{
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_CALIB_IMAGE_PATH))->SetWindowTextW(L"");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_EXP_IMAGE_PATH))->SetWindowTextW(L"");
	((CListBox*)GetDlgItem(IDC_LIST_BOX_CALIB_IMAGE_PATH))->ResetContent();
	((CListBox*)GetDlgItem(IDC_LIST_BOX_EXP_IMAGE_PATH))->ResetContent();
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_LOAD_EXTERNAL_CALIB_DATA))->SetWindowTextW(L"");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_OUTPUT_PATH))->SetWindowTextW(L"");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_RECTIFY_BASE_IMAGE))->SetWindowTextW(L"");
	((CStatic*)GetDlgItem(IDC_PICTURE_CONTROL_BASE_IMAGE))->SetBitmap(0);
	((CStatic*)GetDlgItem(IDC_PICTURE_CONTROL_BASE_IMAGE))->Invalidate();
	((CStatic*)GetDlgItem(IDC_PICTURE_CONTROL_BASE_IMAGE))->UpdateWindow();
	((CButton*)GetDlgItem(IDC_CHECK_BOX_MEMORY_LITE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_BOX_MUTE))->SetCheck(FALSE);
	Invalidate();
	UpdateWindow();

	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_PARTIAL))->SetPos(0);
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_BAR_GLOBAL))->SetPos(0);
	((CButton*)GetDlgItem(IDC_CHECK_BOX_MEMORY_LITE))->SetState(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_CLEAR))->EnableWindow(FALSE);

	return;
}

void CSteelBarASDlg::MessageBoxAutoScrollToBottom()
{
	int line = ((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->GetLineCount() - 1;
	int index = ((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->LineIndex(line);
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_MESSAGE_AND_LOG))->SetSel(index, index);
}