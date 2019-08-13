// 3dplotView.h :
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DPLOTVIEW_H__169CF8FD_5D32_11D2_B747_000000000000__INCLUDED_)
#define AFX_3DPLOTVIEW_H__169CF8FD_5D32_11D2_B747_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OpenGLView.h"
#include "atltypes.h"

class Accelerometer;

class C3dplotView : public COpenGLView
{
protected:
	C3dplotView();
	DECLARE_DYNCREATE(C3dplotView)

public:
	C3dplotDoc* GetDocument();

public:

	//{{AFX_VIRTUAL(C3dplotView)
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

public:
	virtual ~C3dplotView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void RenderScene(void);
	virtual void RenderStockScene(void);

	void	Draw3DAxes(float start = -10.0, float finish = 10.0, int ticks = -1);
	void	Draw3DAxesLine(float start, float finish, int axis_id, int ticks);

protected:
	//{{AFX_MSG(C3dplotView)
	afx_msg void OnRight();
	afx_msg void OnLeft();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCright();
	afx_msg void OnCleft();
	afx_msg void OnResetView();
	afx_msg void OnDataScaleUp();
	afx_msg void OnDataScaleDown();
	afx_msg void OnSetCondition();
	afx_msg void OnUpdateSetCondition(CCmdUI* pCmdUI);
	afx_msg void OnPlotPt();
	afx_msg void OnUpdatePlotPt(CCmdUI* pCmdUI);
	afx_msg void OnPlotSphere();
	afx_msg void OnUpdatePlotSphere(CCmdUI* pCmdUI);
	afx_msg void OnPlotCube();
	afx_msg void OnUpdatePlotCube(CCmdUI* pCmdUI);
	afx_msg void OnFogEffect();
	afx_msg void OnUpdateFogEffect(CCmdUI* pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnRightFast();
	afx_msg void OnLeftFast();
	afx_msg void OnUpFast();
	afx_msg void OnDownFast();
	afx_msg void OnAnimate();
	afx_msg void OnUpdateAnimate(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowZoom();
	afx_msg void OnUpdateShowZoom(CCmdUI* pCmdUI);
	afx_msg void OnShowScale();
	afx_msg void OnUpdateShowScale(CCmdUI* pCmdUI);
	afx_msg void OnXaxes();
	afx_msg void OnUpdateXaxes(CCmdUI* pCmdUI);
	afx_msg void OnYaxes();
	afx_msg void OnUpdateYaxes(CCmdUI* pCmdUI);
	afx_msg void OnZaxes();
	afx_msg void OnUpdateZaxes(CCmdUI* pCmdUI);
	afx_msg void OnShowAxes();
	afx_msg void OnUpdateShowAxes(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_bNormalize;
	CPoint m_prePt;
	bool m_bRolling;
	HCURSOR m_hRollCursor;
	bool m_bAnimeAxes;
	bool m_bShowZ;
	bool m_bShowY;
	bool m_bShowX;
	int m_scaleCnt;
	bool m_bScaleUp;
	bool m_bShowScale;
	bool m_bShowZoom;
	bool m_bZoomIn;
	int m_zoomCnt;
	BOOL m_bAnimate;
	int	m_width, m_height; // the size of the window
	GLfloat m_xRot;
	GLfloat m_yRot;
	GLfloat m_zRot;
	GLfloat m_clip;

	GLfloat m_xRotOrg;
	GLfloat m_yRotOrg;
	GLfloat m_zRotOrg;

	BOOL m_b2Colors;
	BOOL m_bCubes;
	BOOL m_bSpheres;
	BOOL m_bPlots;
	BOOL m_bFog;
protected:
	void zoomIn(void);
	void zoomOut(void);
	void plotData(void);
	void beginPlot(void);
	void endPlot(void);
	void setMaterialColoer(const GLfloat* colorInfo);
	void plotPoint(const TPoint& pt);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDataNormarize();
	afx_msg void OnUpdateDataNormarize(CCmdUI* pCmdUI);
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
public:
	afx_msg void OnProcTiltThinkpad();
public:
	afx_msg void OnUpdateProcTiltThinkpad(CCmdUI* pCmdUI);
private:
	Accelerometer* m_pAccelerometer;
private:
	BOOL m_bTiltMode;
	CPoint m_tiltPrev;
	afx_msg void OnDestroy();
};

#ifndef _DEBUG
inline C3dplotDoc* C3dplotView::GetDocument()
{
	return (C3dplotDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_3DPLOTVIEW_H__169CF8FD_5D32_11D2_B747_000000000000__INCLUDED_)
