
// steelBarTensileTestMFCDlg.h : ヘッダー ファイル
//

#pragma once


#include "function.h"


// CsteelBarTensileTestMFCDlg ダイアログ
class CsteelBarTensileTestMFCDlg : public CDialogEx
{
// コンストラクション
public:
	CsteelBarTensileTestMFCDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_STEELBARTENSILETESTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRun();
	afx_msg void Init();
	afx_msg void ValueInit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonTest();
};
