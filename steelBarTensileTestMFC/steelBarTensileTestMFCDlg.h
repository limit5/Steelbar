
// steelBarTensileTestMFCDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once


#include "function.h"


// CsteelBarTensileTestMFCDlg �_�C�A���O
class CsteelBarTensileTestMFCDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CsteelBarTensileTestMFCDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_STEELBARTENSILETESTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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
