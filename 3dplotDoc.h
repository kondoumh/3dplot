// 3dplotDoc.h
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DPLOTDOC_H__169CF8FB_5D32_11D2_B747_000000000000__INCLUDED_)
#define AFX_3DPLOTDOC_H__169CF8FB_5D32_11D2_B747_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>
using namespace std;

typedef struct {
	float x;
	float y;
	float z;
} TPoint;

class C3dplotDoc : public CDocument
{
protected:
	C3dplotDoc();
	DECLARE_DYNCREATE(C3dplotDoc)

public:
	vector<TPoint> vecP;
	vector<TPoint> vecPorg;
	vector<TPoint> vecP_backup;
	float m_axlength;
	float m_axlengthorg;
	float m_axlength_backup;
	vector<int> m_series;

public:

	//{{AFX_VIRTUAL(C3dplotDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

public:
	void AdjustScale();
	virtual ~C3dplotDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

protected:
	//{{AFX_MSG(C3dplotDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void normlizeData(bool bNorm = true);
	void backupOrgData(void);
	void restoreOrgData(void);
protected:
	TPoint maxPoint(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_3DPLOTDOC_H__169CF8FB_5D32_11D2_B747_000000000000__INCLUDED_)
