// 3dplotDoc.cpp
//

#include "stdafx.h"
#include "3dplot.h"

#include "3dplotDoc.h"
#include ".\3dplotdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(C3dplotDoc, CDocument)

BEGIN_MESSAGE_MAP(C3dplotDoc, CDocument)
	//{{AFX_MSG_MAP(C3dplotDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

C3dplotDoc::C3dplotDoc()
{
}

C3dplotDoc::~C3dplotDoc()
{
}

BOOL C3dplotDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

void C3dplotDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
		vecP.clear();
		vecPorg.clear();
		vecP_backup.clear();
		m_series.clear();

		vecP.resize(0);
		vecPorg.resize(0);
		vecP_backup.resize(0);
		m_series.resize(0);

		CString str;
		m_series.push_back(0);
		int cnt = 0;
		while (ar.ReadString(str) != NULL) {
			str = str.TrimLeft();
			if (str[0] == '/' || str[0] == '#' || str[0] == '\n' || str[0] == '\r') continue;
			if (str[0] == ':' && vecP.size() > 0) {
				m_series.push_back(cnt);
				continue;
			}
			TPoint tp;
			int r = sscanf_s(str, "%f %f %f", &tp.x, &tp.y, &tp.z);

			if (r == 3) {
				vecP.push_back(tp);
				vecPorg.push_back(tp);
				cnt++;
			}
		}
		C3dplotApp* pApp = (C3dplotApp*)AfxGetApp();
		pApp->nPoints = vecP.size();
		pApp->nSeries = m_series.size();
		UpdateAllViews(NULL, 777, NULL);
	}
}

#ifdef _DEBUG
void C3dplotDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void C3dplotDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL C3dplotDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (vecP.size() == 0) return TRUE;
	TPoint Max = maxPoint();
	float length = (Max.x > Max.y) ? Max.x : Max.y;
	m_axlength = m_axlengthorg = (length > Max.z) ? length : Max.z;

	AdjustScale();
	return TRUE;
}

void C3dplotDoc::AdjustScale()
{
	if (!vecP.size()) return;
	for (unsigned int i = 0; i < vecP.size(); i++) {
		vecP[i].x = 4.0f * vecPorg[i].x / m_axlength;
		vecP[i].y = 4.0f * vecPorg[i].y / m_axlength;
		vecP[i].z = 4.0f * vecPorg[i].z / m_axlength;
	}
}

void C3dplotDoc::normlizeData(bool bNorm)
{
	if (bNorm) {
		backupOrgData();
		TPoint Max = maxPoint();
		for (unsigned int i = 0; i < vecPorg.size(); i++) {
			vecPorg[i].x = (float)vecPorg[i].x / (float)Max.x;
			vecPorg[i].y = (float)vecPorg[i].y / (float)Max.y;
			vecPorg[i].z = (float)vecPorg[i].z / (float)Max.z;
		}
		m_axlength = 1;
		AdjustScale();
	}
	else {
		restoreOrgData();
		m_axlength = m_axlengthorg;
		AdjustScale();
	}
}

TPoint C3dplotDoc::maxPoint(void)
{
	TPoint Max;
	Max = vecPorg[0];
	for (unsigned int i = 0; i < vecP.size(); i++) {
		if (vecPorg[i].x > Max.x) { Max.x = vecPorg[i].x; }
		if (vecPorg[i].y > Max.y) { Max.y = vecPorg[i].y; }
		if (vecPorg[i].z > Max.z) { Max.z = vecPorg[i].z; }
	}
	return Max;
}

void C3dplotDoc::backupOrgData(void)
{
	m_axlength_backup = m_axlengthorg;

	vecP_backup.clear();
	vecP_backup.resize(0);
	for (unsigned int i = 0; i < vecPorg.size(); i++) {
		vecP_backup.push_back(vecPorg[i]);
	}
}

void C3dplotDoc::restoreOrgData(void)
{
	m_axlengthorg = m_axlength_backup;

	vecPorg.clear();
	vecPorg.resize(0);
	for (unsigned int i = 0; i < vecP_backup.size(); i++) {
		vecPorg.push_back(vecP_backup[i]);
	}
}
