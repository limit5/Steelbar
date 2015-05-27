
// steelBarTensileTestMFCDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "steelBarTensileTestMFC.h"
#include "steelBarTensileTestMFCDlg.h"
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


// CsteelBarTensileTestMFCDlg ダイアログ



CsteelBarTensileTestMFCDlg::CsteelBarTensileTestMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsteelBarTensileTestMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsteelBarTensileTestMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsteelBarTensileTestMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CsteelBarTensileTestMFCDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDOK, &CsteelBarTensileTestMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CsteelBarTensileTestMFCDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CsteelBarTensileTestMFCDlg メッセージ ハンドラー

BOOL CsteelBarTensileTestMFCDlg::OnInitDialog()
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
	ValueInit();
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CsteelBarTensileTestMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CsteelBarTensileTestMFCDlg::OnPaint()
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
HCURSOR CsteelBarTensileTestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsteelBarTensileTestMFCDlg::ValueInit()
{
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_CHESSBOARD_SIZE_X))->SetWindowTextW(L"5.0");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_CHESSBOARD_SIZE_Y))->SetWindowTextW(L"5.0");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_DIAMETER))->SetWindowTextW(L"12.4");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_NUMBER_OF_H_TEMPLATE))->SetWindowTextW(L"10");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_NUMBER_OF_V_TEMPLATE))->SetWindowTextW(L"2");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_PATTERN_SIZE_X))->SetWindowTextW(L"7");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_PATTERN_SIZE_Y))->SetWindowTextW(L"25");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_H_TEMP_SIZE_X))->SetWindowTextW(L"210");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_H_TEMP_SIZE_Y))->SetWindowTextW(L"40");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_V_TEMP_SIZE_X))->SetWindowTextW(L"40");
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_V_TEMP_SIZE_Y))->SetWindowTextW(L"40");
}


void CsteelBarTensileTestMFCDlg::OnBnClickedButtonRun()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	parameter param;
	CString cs;

	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_V_TEMP_SIZE_X))->GetWindowText(cs);
	param.vTemplateSizeX = _ttoi(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_V_TEMP_SIZE_Y))->GetWindowText(cs);
	param.vTemplateSizeY = _ttoi(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_H_TEMP_SIZE_X))->GetWindowText(cs);
	param.hTemplateSizeX = _ttoi(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_H_TEMP_SIZE_Y))->GetWindowText(cs);
	param.hTemplateSizeY = _ttoi(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_NUMBER_OF_V_TEMPLATE))->GetWindowText(cs);
	param.nVtemplate = _ttoi(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_NUMBER_OF_H_TEMPLATE))->GetWindowText(cs);
	param.nHtemplate = _ttoi(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_CHESSBOARD_SIZE_X))->GetWindowText(cs);
	param.cellSizeDx = _ttof(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_CHESSBOARD_SIZE_Y))->GetWindowText(cs);
	param.cellSizeDy = _ttof(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_PATTERN_SIZE_X))->GetWindowText(cs);
	param.patternSizeNx = _ttoi(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_PATTERN_SIZE_Y))->GetWindowText(cs);
	param.patternSizeNy = _ttoi(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT_DIAMETER))->GetWindowText(cs);
	param.diameter = _ttof(cs);
	if (cs == L"") {MessageBoxA(NULL, "value can not be null.", "!!!", NULL);return;}
	
	//MessageBoxA(NULL, "Check", "Checkbox", NULL);

	int d = zz_SteelBarTensileTest(param);
	if (d == EXIT_SUCCESS)
	{
		MessageBoxA(NULL, "DONE!!!", "(´・ω・｀)", NULL);
	}

}

void CsteelBarTensileTestMFCDlg::Init()
{
	LPSECURITY_ATTRIBUTES lpSecurity;
	lpSecurity = NULL;
	if (CreateDirectory(L"template", lpSecurity) == FALSE)
	{
		MessageBoxA(NULL, "can not create template folder.", "Warning!", NULL);
	}

	LPSECURITY_ATTRIBUTES lpSecurity2;
	lpSecurity2 = NULL;
	if (CreateDirectory(L"ctrlPoints", lpSecurity2) == FALSE)
	{
		MessageBoxA(NULL, "can not create ctrlPoints folder.", "Warning!", NULL);
	}

	LPSECURITY_ATTRIBUTES lpSecurity3;
	lpSecurity3 = NULL;
	if (CreateDirectory(L"chessboardImage", lpSecurity3) == FALSE)
	{
		MessageBoxA(NULL, "can not create chessboardImage folder.", "Warning!", NULL);
	}

	LPSECURITY_ATTRIBUTES lpSecurity4;
	lpSecurity4 = NULL;
	if (CreateDirectory(L"rectf", lpSecurity4) == FALSE)
	{
		MessageBoxA(NULL, "can not create rectf folder.", "Warning!", NULL);
	}

	LPSECURITY_ATTRIBUTES lpSecurity5;
	lpSecurity5 = NULL;
	if (CreateDirectory(L"trace", lpSecurity5) == FALSE)
	{
		MessageBoxA(NULL, "can not create trace folder.", "Warning!", NULL);
	}

}

void CsteelBarTensileTestMFCDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	MessageBoxA(NULL, "Developer: Yuan-Sen Yang Lab.\r\nVersion: 0.1beta\r\nDate: 2014/02/18\r\nOh...\r\r\nBye bye...($・・)/~~~", "do not press this button", NULL);
	CDialogEx::OnOK();
}


void CsteelBarTensileTestMFCDlg::OnBnClickedButtonTest()
{
	// TODO: Add your control notification handler code here
	MessageBoxA(NULL, "This is a test\n", "test", NULL);
}
