// 3dplot.cpp
//

#include "stdafx.h"
#include "3dplot.h"

#include "MainFrm.h"
#include "3dplotDoc.h"
#include "3dplotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REGS_ROOT _T("KondohMH")

/////////////////////////////////////////////////////////////////////////////
// C3dplotApp

BEGIN_MESSAGE_MAP(C3dplotApp, CWinApp)
	//{{AFX_MSG_MAP(C3dplotApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

C3dplotApp::C3dplotApp()
{
}

C3dplotApp theApp;

BOOL C3dplotApp::InitInstance()
{
	AfxEnableControlContainer();
	SetRegistryKey(REGS_ROOT);

	LoadStdProfileSettings(10);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(C3dplotDoc),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(C3dplotView));
	AddDocTemplate(pDocTemplate);

	EnableShellOpen();
	RegisterShellFileTypes(FALSE);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_Series;
	CStatic	m_Point;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_SERIES, m_Series);
	DDX_Control(pDX, IDC_POINT, m_Point);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void C3dplotApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// C3dplotApp コマンド

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	C3dplotApp* pApp = (C3dplotApp*)AfxGetApp();
	CString points; points.Format(" Points : %d", pApp->nPoints);
	CString series; series.Format(" Series : %d", pApp->nSeries);
	m_Point.SetWindowText(points);
	m_Series.SetWindowText(series);
	return TRUE;
}
