
// steelBarTensileTestMFC.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CsteelBarTensileTestMFCApp:
// このクラスの実装については、steelBarTensileTestMFC.cpp を参照してください。
//

class CsteelBarTensileTestMFCApp : public CWinApp
{
public:
	CsteelBarTensileTestMFCApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CsteelBarTensileTestMFCApp theApp;