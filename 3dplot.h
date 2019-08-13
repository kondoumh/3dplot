// 3dplot.h
//

#if !defined(AFX_3DPLOT_H__169CF8F5_5D32_11D2_B747_000000000000__INCLUDED_)
#define AFX_3DPLOT_H__169CF8F5_5D32_11D2_B747_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"


class C3dplotApp : public CWinApp
{
public:
	int nSeries;
	int nPoints;
	C3dplotApp();

	//{{AFX_VIRTUAL(C3dplotApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL


	//{{AFX_MSG(C3dplotApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_3DPLOT_H__169CF8F5_5D32_11D2_B747_000000000000__INCLUDED_)
