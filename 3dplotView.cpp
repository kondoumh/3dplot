// 3dplotView.cpp
//

#include "stdafx.h"
#include "3dplot.h"

#include "3dplotDoc.h"
#include "3dplotView.h"
#include ".\3dplotview.h"

#include "AccelerometerThinkpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INVALIDATE_TIMER 500
#define TILT_TIMER 1234

// Material Colors
GLfloat material_diffuse[14][4] = { 1.0f, 0.0f, 0.0f, 1.0f, // Red
								  0.0f, 1.0f, 0.0f, 1.0f, // Green
								  0.0f, 0.0f, 1.0f, 1.0f, // Blue
								  1.0f, 1.0f, 1.0f, 1.0f, // White
								  1.0f, 1.0f, 0.0f, 1.0f, // Yellow
								  1.0f, 0.0f, 1.0f, 1.0f, // Parple
								  0.0f, 1.0f, 1.0f, 1.0f, // emerald
								  0.5f, 0.0f, 0.0f, 1.0f,
								  0.0f, 0.5f, 0.0f, 1.0f,
								  0.0f, 0.0f, 0.5f, 1.0f,
								  0.5f, 0.5f, 0.5f, 1.0f,
								  0.5f, 0.5f, 0.0f, 1.0f,
								  0.5f, 0.0f, 0.5f, 1.0f,
								  0.0f, 0.5f, 0.5f, 1.0f };

/////////////////////////////////////////////////////////////////////////////
// C3dplotView

IMPLEMENT_DYNCREATE(C3dplotView, COpenGLView)

BEGIN_MESSAGE_MAP(C3dplotView, COpenGLView)
	//{{AFX_MSG_MAP(C3dplotView)
	ON_COMMAND(ID_RIGHT, OnRight)
	ON_COMMAND(ID_LEFT, OnLeft)
	ON_COMMAND(ID_UP, OnUp)
	ON_COMMAND(ID_DOWN, OnDown)
	ON_WM_SIZE()
	ON_COMMAND(ID_CRIGHT, OnCright)
	ON_COMMAND(ID_CLEFT, OnCleft)
	ON_COMMAND(ID_RESET_VIEW, OnResetView)
	ON_COMMAND(ID_DATA_SCALE_UP, OnDataScaleUp)
	ON_COMMAND(ID_DATA_SCALE_DOWN, OnDataScaleDown)
	ON_COMMAND(ID_SET_CONDITION, OnSetCondition)
	ON_UPDATE_COMMAND_UI(ID_SET_CONDITION, OnUpdateSetCondition)
	ON_COMMAND(ID_PLOT_PT, OnPlotPt)
	ON_UPDATE_COMMAND_UI(ID_PLOT_PT, OnUpdatePlotPt)
	ON_COMMAND(ID_PLOT_SPHERE, OnPlotSphere)
	ON_UPDATE_COMMAND_UI(ID_PLOT_SPHERE, OnUpdatePlotSphere)
	ON_COMMAND(ID_PLOT_CUBE, OnPlotCube)
	ON_UPDATE_COMMAND_UI(ID_PLOT_CUBE, OnUpdatePlotCube)
	ON_COMMAND(ID_FOG_EFFECT, OnFogEffect)
	ON_UPDATE_COMMAND_UI(ID_FOG_EFFECT, OnUpdateFogEffect)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_RIGHT_FAST, OnRightFast)
	ON_COMMAND(ID_LEFT_FAST, OnLeftFast)
	ON_COMMAND(ID_UP_FAST, OnUpFast)
	ON_COMMAND(ID_DOWN_FAST, OnDownFast)
	ON_COMMAND(ID_ANIMATE, OnAnimate)
	ON_UPDATE_COMMAND_UI(ID_ANIMATE, OnUpdateAnimate)
	ON_WM_TIMER()
	ON_COMMAND(ID_SHOW_ZOOM, OnShowZoom)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ZOOM, OnUpdateShowZoom)
	ON_COMMAND(ID_SHOW_SCALE, OnShowScale)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SCALE, OnUpdateShowScale)
	ON_COMMAND(ID_XAXES, OnXaxes)
	ON_UPDATE_COMMAND_UI(ID_XAXES, OnUpdateXaxes)
	ON_COMMAND(ID_YAXES, OnYaxes)
	ON_UPDATE_COMMAND_UI(ID_YAXES, OnUpdateYaxes)
	ON_COMMAND(ID_ZAXES, OnZaxes)
	ON_UPDATE_COMMAND_UI(ID_ZAXES, OnUpdateZaxes)
	ON_COMMAND(ID_SHOW_AXES, OnShowAxes)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AXES, OnUpdateShowAxes)
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, COpenGLView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, COpenGLView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, COpenGLView::OnFilePrintPreview)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_DATA_NORMARIZE, OnDataNormarize)
	ON_UPDATE_COMMAND_UI(ID_DATA_NORMARIZE, OnUpdateDataNormarize)
	ON_COMMAND(ID_PROC_TILT_THINKPAD, &C3dplotView::OnProcTiltThinkpad)
	ON_UPDATE_COMMAND_UI(ID_PROC_TILT_THINKPAD, &C3dplotView::OnUpdateProcTiltThinkpad)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

C3dplotView::C3dplotView() : m_xRot(0.0f), m_yRot(-38.0f), m_zRot(10.0f),
m_clip(2.5f), m_b2Colors(TRUE), m_bFog(FALSE),
m_bCubes(FALSE), m_bPlots(TRUE), m_bSpheres(FALSE),
m_bAnimate(FALSE), m_bShowZoom(false), m_bShowScale(false),
m_bShowX(true), m_bShowY(true), m_bShowZ(true), m_bAnimeAxes(true),
m_hRollCursor(NULL), m_bRolling(false)
, m_bNormalize(false)
, m_bTiltMode(FALSE)
, m_pAccelerometer(NULL)
{
	if (m_hRollCursor == NULL) {
		m_hRollCursor = AfxGetApp()->LoadCursor(IDC_ROLLING);
	}
}

C3dplotView::~C3dplotView()
{
}

void C3dplotView::OnDraw(CDC* pDC)
{
	C3dplotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	COpenGLView::OnDraw(pDC);
}

#ifdef _DEBUG
void C3dplotView::AssertValid() const
{
	COpenGLView::AssertValid();
}

void C3dplotView::Dump(CDumpContext& dc) const
{
	COpenGLView::Dump(dc);
}

C3dplotDoc* C3dplotView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C3dplotDoc)));
	return (C3dplotDoc*)m_pDocument;
}
#endif //_DEBUG

void C3dplotView::RenderStockScene()
{
	return; // NOP
}

void C3dplotView::RenderScene()
{
	::glViewport(0, 0, m_width, m_height);

	::glLoadIdentity();

	::glTranslatef(0.0f, -1.0f, -8.0f);

	::glRotatef(m_zRot, 1.0f, 0.0f, 0.0f);
	::glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);
	::glRotatef(m_xRot, 0.0f, 0.0f, 1.0f);

	if (m_bAnimeAxes) {
		::glDisable(GL_LIGHTING);
		Draw3DAxes(-30.0f, 30.0f, 20);

		::glPushMatrix();
		::glTranslatef(4.0f, 0.0f, 0.0f);
		GLTextOut("x");
		::glPopMatrix();

		::glPushMatrix();
		::glTranslatef(0.0f, 4.0f, 0.0f);
		GLTextOut("y");
		::glPopMatrix();

		::glPushMatrix();
		::glTranslatef(0.0f, 0.0f, 4.0f);
		::glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		GLTextOut("z");
		::glPopMatrix();
	}
	plotData();
}

void C3dplotView::OnInitialUpdate()
{
	COpenGLView::OnInitialUpdate();

	GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_position[] = { 0.0f, 1.0f, 2.0f, 0.0f };

	::glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	::glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	::glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	::glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	::glEnable(GL_LIGHTING);
	::glEnable(GL_LIGHT0);
	::glEnable(GL_AUTO_NORMAL);
	::glEnable(GL_NORMALIZE);
	::glDepthFunc(GL_LEQUAL);
	::glEnable(GL_DEPTH_TEST);
	::glShadeModel(GL_FLAT);

	GLfloat material_ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat material_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };

	GLfloat material_shininess[] = { 100.0f };
	GLfloat material_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	::glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	::glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse[0]);
	::glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);

	::glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
	::glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);

	::glPointSize(2.0f);
}

void C3dplotView::OnSize(UINT nType, int cx, int cy)
{
	if (0 >= cx || 0 >= cy)
	{
		return;
	}

	m_width = cx;
	m_height = cy;

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	GLfloat aspectRatio;

	if (m_width <= m_height)
	{
		aspectRatio = (GLfloat)m_height / (GLfloat)m_width;
		::glFrustum(-m_clip, m_clip, -m_clip * aspectRatio, m_clip * aspectRatio, 4.0, 25.0);
	}
	else
	{
		aspectRatio = (GLfloat)m_width / (GLfloat)m_height;
		::glFrustum(-m_clip * aspectRatio, m_clip * aspectRatio, -m_clip, m_clip, 4.0, 25.0);
	}

	::glMatrixMode(GL_MODELVIEW);
}

void C3dplotView::Draw3DAxes(float start, float finish, int ticks)
{
	if (start > finish)
	{
		float temp = start;
		start = finish;
		finish = start;
	}

	if (0 > ticks)
	{
		float delta = finish - start;
		ticks = delta > 1.0 ? (int)delta : 0;
	}

	Draw3DAxesLine(start, finish, 0, ticks);
	Draw3DAxesLine(start, finish, 1, ticks);
	Draw3DAxesLine(start, finish, 2, ticks);
}


void C3dplotView::Draw3DAxesLine(float start, float finish, int axis_id, int ticks)
{
	float* px, * py, * pz, zero = 0.0f;
	float tickx, ticky, tickz;
	float* pdx, * pdy, * pdz, tinytick, delta = (finish - start) / (ticks < 1 ? 1 : ticks);
	GLfloat negativeColor[3] = { 0.0f, 0.0f, 0.7f };
	GLfloat positiveColor[3] = { 0.0f, 0.7f, 0.7f };

	pdx = pdy = pdz = px = py = pz = &zero;
	tickx = ticky = tickz = 0.0f;
	tinytick = 0.05f;

	if (0 == axis_id)
	{
		pdx = &delta;
		ticky = tinytick;
		px = &start;
	}
	else if (1 == axis_id)
	{
		pdy = &delta;
		tickx = tinytick;
		py = &start;
	}
	else
	{
		pdz = &delta;
		ticky = tinytick;
		pz = &start;
	}

	::glBegin(GL_LINES);

	::glColor3fv(negativeColor);
	::glVertex3f(*px, *py, *pz);
	::glVertex3f(0.0f, 0.0f, 0.0f);

	::glColor3fv(positiveColor);
	::glVertex3f(0.0f, 0.0f, 0.0f);
	::glVertex3f(*px + *pdx * ticks, *py + *pdy * ticks, *pz + *pdz * ticks);

	for (int i = 0; i < ticks; i++)
	{
		if (i < ticks / 2)
		{
			::glColor3fv(negativeColor);
		}
		else
		{
			::glColor3fv(positiveColor);
		}

		::glVertex3f(*px - tickx, *py - ticky, *pz - tickz);
		::glVertex3f(*px + tickx, *py + ticky, *pz + tickz);

		*px += *pdx;
		*py += *pdy;
		*pz += *pdz;
	}

	::glEnd();

}

void C3dplotView::OnRight()
{
	m_yRot += 2.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnLeft()
{
	m_yRot -= 2.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnUp()
{
	m_zRot -= 2.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnDown()
{
	m_zRot += 2.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnRightFast()
{
	m_yRot += 10.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnLeftFast()
{
	m_yRot -= 10.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnUpFast()
{
	m_zRot -= 10.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnDownFast()
{
	m_zRot += 10.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnCright()
{
	m_xRot -= 2.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnCleft()
{
	m_xRot += 2.0f;
	Invalidate(FALSE);
}

void C3dplotView::OnResetView()
{
	m_xRot = 0.0f;
	m_yRot = -38.0f;
	m_zRot = 10.0f;
	m_clip = 2.5f;

	m_xRotOrg = m_xRot;
	m_yRotOrg = m_yRot;
	m_zRotOrg = m_zRot;

	if (m_bNormalize) {
		m_bNormalize = false;
		GetDocument()->restoreOrgData();
	}

	GetDocument()->m_axlength = GetDocument()->m_axlengthorg;
	GetDocument()->AdjustScale();

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	GLfloat aspectRatio;

	if (m_width <= m_height)
	{
		aspectRatio = (GLfloat)m_height / (GLfloat)m_width;
		::glFrustum(-m_clip, m_clip, -m_clip * aspectRatio, m_clip * aspectRatio, 4.0, 25.0);
	}
	else
	{
		aspectRatio = (GLfloat)m_width / (GLfloat)m_height;
		::glFrustum(-m_clip * aspectRatio, m_clip * aspectRatio, -m_clip, m_clip, 4.0, 25.0);
	}

	::glMatrixMode(GL_MODELVIEW);
	Invalidate(FALSE);
}

void C3dplotView::OnDataScaleUp()
{
	C3dplotDoc* pDoc = GetDocument();
	pDoc->m_axlength = pDoc->m_axlength * 0.99f;
	pDoc->AdjustScale();
	Invalidate(FALSE);
}

void C3dplotView::OnDataScaleDown()
{
	C3dplotDoc* pDoc = GetDocument();
	pDoc->m_axlength = pDoc->m_axlength * 1.01f;
	pDoc->AdjustScale();
	Invalidate(FALSE);
}

void C3dplotView::OnSetCondition()
{
	m_b2Colors = !m_b2Colors;
	Invalidate(FALSE);
}

void C3dplotView::OnUpdateSetCondition(CCmdUI* pCmdUI)
{
	C3dplotDoc* pDoc = GetDocument();
	pCmdUI->Enable((pDoc->m_series.size() > 2));
	pCmdUI->SetCheck(!m_b2Colors);
}

void C3dplotView::plotData()
{
	C3dplotDoc* pDoc = GetDocument();

	// Data Plotting
	beginPlot();
	if (pDoc->vecP.size() == 0) {
		::glColor3fv(material_diffuse[0]);
		for (float f = -30.0f; f < 30.0f; f += 0.3f) {
			TPoint t; t.x = f; t.y = f; t.z = f;
			plotPoint(t);
		}
	}
	else {
		if (pDoc->m_series.size() == 1) {
			::glColor3fv(material_diffuse[0]);
		}
		unsigned int i = 0;
		unsigned int j = 0;
		for (; i < pDoc->vecP.size(); i++) {
			TPoint t = pDoc->vecP[i];
			if (pDoc->m_series.size() > 1 && j < pDoc->m_series.size()) {
				if (!m_b2Colors && i == pDoc->m_series[j] && j < 14) {
					setMaterialColoer(material_diffuse[j]);
					j++;
				}
				else if (m_b2Colors && i == pDoc->m_series[j]) {
					setMaterialColoer(material_diffuse[j % 2]);
					j++;
				}
			}
			plotPoint(t);
		}
	}
	endPlot();
}

void C3dplotView::beginPlot()
{
	if (m_bPlots) {
		glBegin(GL_POINTS);
	}
	else {
		::glEnable(GL_LIGHTING);
	}
}

void C3dplotView::endPlot()
{
	if (m_bPlots) {
		glEnd();
	}
}

void C3dplotView::setMaterialColoer(const GLfloat* colorInfo)
{
	if (m_bPlots) {
		::glColor3fv(colorInfo);
	}
	else {
		::glMaterialfv(GL_FRONT, GL_DIFFUSE, colorInfo);
	}
}

void C3dplotView::plotPoint(const TPoint& t)
{
	if (m_bPlots) {
		glVertex3f(t.x, t.y, t.z);
	}
	else {
		glPushMatrix();
		::glTranslatef(t.x, t.y, t.z);
		GLUquadricObj* obj;
		obj = gluNewQuadric();
		if (m_bCubes) {
			gluSphere(obj, 0.05, 3, 3);
		}
		else if (m_bSpheres) {
			gluSphere(obj, 0.05, 20, 20);
		}
		glPopMatrix();
	}
}


void C3dplotView::OnPlotPt()
{
	m_bPlots = TRUE;
	m_bSpheres = m_bCubes = FALSE;
	Invalidate(FALSE);
}

void C3dplotView::OnUpdatePlotPt(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bPlots);
}

void C3dplotView::OnPlotSphere()
{
	m_bSpheres = TRUE;
	m_bPlots = m_bCubes = FALSE;
	Invalidate(FALSE);
}

void C3dplotView::OnUpdatePlotSphere(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bSpheres);
}

void C3dplotView::OnPlotCube()
{
	m_bCubes = TRUE;
	m_bPlots = m_bSpheres = FALSE;
	Invalidate(FALSE);
}

void C3dplotView::OnUpdatePlotCube(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bCubes);
}

void C3dplotView::OnFogEffect()
{
	m_bFog = !m_bFog;
	if (m_bFog) {
		::glEnable(GL_FOG);
		GLfloat fogColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
		::glFogi(GL_FOG_MODE, GL_LINEAR);
		::glFogfv(GL_FOG_COLOR, fogColor);
		::glFogf(GL_FOG_START, 2.0f);
		::glFogf(GL_FOG_END, 20.0f);
		::glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	}
	else {
		::glDisable(GL_FOG);
		::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	Invalidate(FALSE);
}

void C3dplotView::OnUpdateFogEffect(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bFog);
}

void C3dplotView::OnZoomIn()
{
	zoomIn();
}

void C3dplotView::OnZoomOut()
{
	zoomOut();
}

void C3dplotView::OnAnimate()
{
	m_bAnimate = !m_bAnimate;
	if (m_bAnimate) {
		SetTimer(INVALIDATE_TIMER, 1, NULL);
		m_zoomCnt = 0;
		m_bZoomIn = true;
		m_scaleCnt = 0;
		m_bScaleUp = true;
	}
	else {
		KillTimer(INVALIDATE_TIMER);
		m_zoomCnt = 0;
		Invalidate(FALSE);
	}
}

void C3dplotView::OnUpdateAnimate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_bTiltMode);
	pCmdUI->SetCheck(m_bAnimate);
}

void C3dplotView::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == INVALIDATE_TIMER) {
		if (m_bShowY) m_yRot += 2.0f;
		if (m_bShowZ) m_xRot += 2.0f;
		if (m_bShowX) m_zRot += 2.0f;

		if (m_bShowScale && m_scaleCnt <= 100 && m_bScaleUp) {
			m_scaleCnt++;
			C3dplotDoc* pDoc = GetDocument();
			pDoc->m_axlength = pDoc->m_axlength * 1.01f;
			pDoc->AdjustScale();
			if (m_scaleCnt == 100) {
				m_bScaleUp = false;
			}
		}
		else if (m_bShowScale && m_scaleCnt >= -100 && !m_bScaleUp) {
			m_scaleCnt--;
			C3dplotDoc* pDoc = GetDocument();
			pDoc->m_axlength = pDoc->m_axlength * 0.99f;
			pDoc->AdjustScale();
			if (m_scaleCnt == -100) {
				m_bScaleUp = true;
			}
		}

		if (m_bShowZoom && m_zoomCnt <= 100 && m_bZoomIn) {
			m_clip -= 0.1f;
			m_zoomCnt++;
			if (m_zoomCnt == 100) {
				m_bZoomIn = false;
			}
		}
		else if (m_bShowZoom && m_zoomCnt >= -100 && !m_bZoomIn) {
			m_clip += 0.1f;
			m_zoomCnt--;
			if (m_zoomCnt == -100) {
				m_bZoomIn = true;
			}
		}

		if (m_bShowZoom) {
			::glMatrixMode(GL_PROJECTION);
			::glLoadIdentity();

			GLfloat aspectRatio;

			if (m_width <= m_height)
			{
				aspectRatio = (GLfloat)m_height / (GLfloat)m_width;
				::glFrustum(-m_clip, m_clip, -m_clip * aspectRatio, m_clip * aspectRatio, 4.0, 25.0);
			}
			else
			{
				aspectRatio = (GLfloat)m_width / (GLfloat)m_height;
				::glFrustum(-m_clip * aspectRatio, m_clip * aspectRatio, -m_clip, m_clip, 4.0, 25.0);
			}

			::glMatrixMode(GL_MODELVIEW);
		}
		Invalidate(FALSE);
	}
	else if (nIDEvent = TILT_TIMER) {
		m_pAccelerometer->GetAccelerometerData();
		CPoint pt(m_pAccelerometer->Y, m_pAccelerometer->X);
		CSize diff = m_tiltPrev - pt;

		if (diff.cx > 0) {
			m_yRot -= (float)abs(diff.cx);
		}
		else if (diff.cx < 0) {
			m_yRot += (float)abs(diff.cx);
		}
		if (diff.cy > 0) {
			m_zRot -= (float)abs(diff.cy);
		}
		else if (diff.cy < 0) {
			m_zRot += (float)abs(diff.cy);
		}

		m_tiltPrev = pt;

		Invalidate(FALSE);
	}

	COpenGLView::OnTimer(nIDEvent);
}

void C3dplotView::OnShowZoom()
{
	m_bShowZoom = !m_bShowZoom;
}

void C3dplotView::OnUpdateShowZoom(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowZoom);
}

void C3dplotView::OnShowScale()
{
	m_bShowScale = !m_bShowScale;
}

void C3dplotView::OnUpdateShowScale(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowScale);
}

void C3dplotView::OnXaxes()
{
	m_bShowX = !m_bShowX;
}

void C3dplotView::OnUpdateXaxes(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowX);
}

void C3dplotView::OnYaxes()
{
	m_bShowY = !m_bShowY;
}

void C3dplotView::OnUpdateYaxes(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowY);
}

void C3dplotView::OnZaxes()
{
	m_bShowZ = !m_bShowZ;
}

void C3dplotView::OnUpdateZaxes(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowZ);
}

void C3dplotView::OnShowAxes()
{
	m_bAnimeAxes = !m_bAnimeAxes;
	Invalidate(FALSE);
}

void C3dplotView::OnUpdateShowAxes(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bAnimeAxes);
}

void C3dplotView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_CONTEXT);
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	if (point.x == -1 && point.y == -1) {
		CRect rc; GetWindowRect(&rc);
		point = rc.TopLeft();
	}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}

void C3dplotView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bRolling) {
		if (!nFlags & MK_LBUTTON) {
			m_bRolling = false;
		}
		CSize diff = m_prePt - point;

		if (diff.cx > 0) {
			m_yRot -= (float)abs(diff.cx);
		}
		else if (diff.cx < 0) {
			m_yRot += (float)abs(diff.cx);
		}
		if (diff.cy > 0) {
			m_zRot -= (float)abs(diff.cy);
		}
		else if (diff.cy < 0) {
			m_zRot += (float)abs(diff.cy);
		}


		m_prePt = point;

		m_xRotOrg = m_xRot;
		m_yRotOrg = m_yRot;
		m_zRotOrg = m_zRot;

		Invalidate(FALSE);
	}

	COpenGLView::OnMouseMove(nFlags, point);
}

void C3dplotView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bRolling = true;
	m_prePt = point;
	COpenGLView::OnLButtonDown(nFlags, point);
}

void C3dplotView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bRolling = false;

	COpenGLView::OnLButtonUp(nFlags, point);
}

BOOL C3dplotView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bRolling) {
		::SetCursor(m_hRollCursor);
		return TRUE;
	}
	return COpenGLView::OnSetCursor(pWnd, nHitTest, message);
}

BOOL C3dplotView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0) {
		zoomIn();
	}
	else {
		zoomOut();
	}
	return COpenGLView::OnMouseWheel(nFlags, zDelta, pt);
}

void C3dplotView::zoomIn(void)
{
	if (m_clip > 0.1f) {
		m_clip -= 0.1f;
	}
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	GLfloat aspectRatio;

	if (m_width <= m_height)
	{
		aspectRatio = (GLfloat)m_height / (GLfloat)m_width;
		::glFrustum(-m_clip, m_clip, -m_clip * aspectRatio, m_clip * aspectRatio, 4.0, 25.0);
	}
	else
	{
		aspectRatio = (GLfloat)m_width / (GLfloat)m_height;
		::glFrustum(-m_clip * aspectRatio, m_clip * aspectRatio, -m_clip, m_clip, 4.0, 25.0);
	}

	::glMatrixMode(GL_MODELVIEW);
	Invalidate(FALSE);
}

void C3dplotView::zoomOut(void)
{
	m_clip += 0.1f;
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	GLfloat aspectRatio;

	if (m_width <= m_height)
	{
		aspectRatio = (GLfloat)m_height / (GLfloat)m_width;
		::glFrustum(-m_clip, m_clip, -m_clip * aspectRatio, m_clip * aspectRatio, 4.0, 25.0);
	}
	else
	{
		aspectRatio = (GLfloat)m_width / (GLfloat)m_height;
		::glFrustum(-m_clip * aspectRatio, m_clip * aspectRatio, -m_clip, m_clip, 4.0, 25.0);
	}

	::glMatrixMode(GL_MODELVIEW);
	Invalidate(FALSE);
}

void C3dplotView::OnDataNormarize()
{
	m_bNormalize = !m_bNormalize;
	C3dplotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->normlizeData(m_bNormalize);
	Invalidate(FALSE);
}

void C3dplotView::OnUpdateDataNormarize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetDocument()->vecP.size() != 0);
	pCmdUI->SetCheck(m_bNormalize);
}

void C3dplotView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint == 777) {
		m_bNormalize = false;
		Invalidate(FALSE);
	}
}


void C3dplotView::OnProcTiltThinkpad()
{
	m_bTiltMode = !m_bTiltMode;
	if (m_bTiltMode) {
		m_pAccelerometer = new Accelerometer();
		if (!m_pAccelerometer->GetAccelerometerData()) {
			MessageBox("This machine does not support the function.");
			m_bTiltMode = FALSE;
			return;
		}
		m_xRotOrg = m_xRot;
		m_yRotOrg = m_yRot;
		m_zRotOrg = m_zRot;
		m_tiltPrev = CPoint(0, 0);
		SetTimer(TILT_TIMER, 50, NULL);
	}
	else {
		KillTimer(TILT_TIMER);
		delete m_pAccelerometer;
	}
}

void C3dplotView::OnUpdateProcTiltThinkpad(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_bAnimate);
	pCmdUI->SetCheck(m_bTiltMode);
}

void C3dplotView::OnDestroy()
{
	COpenGLView::OnDestroy();

	if (m_bAnimate) {
		KillTimer(INVALIDATE_TIMER);
	}
	if (m_bTiltMode) {
		KillTimer(TILT_TIMER);
		if (m_pAccelerometer != NULL) {
			delete m_pAccelerometer;
		}
	}
}
