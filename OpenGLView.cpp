// OpenGLView.cpp
//

#include "stdafx.h"
#include "3dplot.h"

#include "3dplotDoc.h"
#include "OpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COpenGLView::COpenGLView() :
	m_hRC(0), m_pDC(0),
	m_AspectRatio(0.0f),
	m_WindowWidth(0), m_WindowHeight(0),
	m_TextID(0)
{

}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

void COpenGLView::OnDraw(CDC* pDC)
{
	C3dplotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	PreRenderScene();

	::glPushMatrix();
	RenderStockScene();
	::glPopMatrix();

	::glPushMatrix();
	RenderScene();
	::glPopMatrix();

	::glFinish();


	if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc()))
	{
		::AfxMessageBox("SwapBuffers failed.");
	}

	PostRenderScene();
}


void COpenGLView::RenderScene()
{
	::glTranslatef(0.0f, 0.5f, 0.0f);
	::glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	::glColor3f(1.0f, 0.0f, 0.0f);
	::glColor3f(0.5f, 0.5f, 1.0f);
	::glLineWidth(4.0f);
}

void COpenGLView::RenderStockScene()
{
	GLfloat v0[3], v1[3], v2[3], v3[3], delta;
	int color = 0;

	delta = 0.5f;

	GLfloat color1[3] = { 0.9f, 0.9f, 0.9f };
	GLfloat color2[3] = { 0.05f, 0.05f, 0.05f };

	v0[1] = v1[1] = v2[1] = v3[1] = 0.0f;

	::glBegin(GL_QUADS);

	for (int x = -5; x <= 5; x++)
	{
		for (int z = -5; z <= 5; z++)
		{
			::glColor3fv((color++) % 2 ? color1 : color2);

			v0[0] = 0.0f + delta * z;
			v0[2] = 0.0f + delta * x;

			v1[0] = v0[0] + delta;
			v1[2] = v0[2];

			v2[0] = v0[0] + delta;
			v2[2] = v0[2] + delta;

			v3[0] = v0[0];
			v3[2] = v0[2] + delta;

			::glVertex3fv(v0);
			::glVertex3fv(v1);
			::glVertex3fv(v2);
			::glVertex3fv(v3);
		}
	}
	::glEnd();

}


#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C3dplotDoc* COpenGLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C3dplotDoc)));
	return (C3dplotDoc*)m_pDocument;
}
#endif //_DEBUG

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeOpenGL();

	return 0;
}


void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	if (FALSE == ::wglMakeCurrent(0, 0))
	{
		::AfxMessageBox("wglMakeCurrent failed.");
	}

	if (m_hRC && (FALSE == ::wglDeleteContext(m_hRC)))
	{
		::AfxMessageBox("wglDeleteContext failed.");
	}

	if (m_pDC)
	{
		delete m_pDC;
	}
}


BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void COpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy)
	{
		return;
	}

	m_WindowWidth = cx;
	m_WindowHeight = cy;

	m_AspectRatio = (GLdouble)m_WindowWidth / (GLdouble)m_WindowHeight;

	::glViewport(0, 0, m_WindowWidth, m_WindowHeight);

	if (GL_NO_ERROR != ::glGetError())
	{
		::AfxMessageBox("Error while trying to set viewport.");
	}

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	SetupViewingFrustum();

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();

	SetupViewingTransform();
}


BOOL COpenGLView::SetupViewingFrustum(void)
{
	// select a default perspective viewing volumn
	::gluPerspective(40.0f, m_AspectRatio, 0.1f, 20.0f);

	// NOTE: Other commands you could have here are
	// glFrustum, which gives you much more control over
	// the perspective view, or glOrtho which is used for
	// parallel projections. No matter what you use, check
	// the error status when you set the viewing frustum!

	if (GL_NO_ERROR != ::glGetError())
	{
		::AfxMessageBox("Error while trying to set viewing frustum.");
		return FALSE;
	}

	return TRUE;
}


BOOL COpenGLView::SetupViewingTransform()
{
	// select a default viewing transformation
	// of a 20 degree rotation about the X axis
	// then a -5 unit transformation along Z
	::glTranslatef(0.0f, 0.0f, -5.0f);
	::glRotatef(20.0f, 1.0f, 0.0f, 0.0f);

	return TRUE;
}


BOOL COpenGLView::InitializeOpenGL()
{
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) // failure to get DC
	{
		::AfxMessageBox("Couldn't get a valid DC.");
		return FALSE;
	}

	if (!SetupPixelFormat())
	{
		::AfxMessageBox("SetupPixelFormat failed.\n");
		return FALSE;
	}

	if (0 == (m_hRC =
		::wglCreateContext(m_pDC->GetSafeHdc())))
	{
		::AfxMessageBox("wglCreateContext failed.");
		return FALSE;
	}

	if (FALSE ==
		::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		::AfxMessageBox("wglMakeCurrent failed.");
		return FALSE;
	}

	// specify black as clear color
	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// specify the back of the buffer as clear depth
	::glClearDepth(1.0f);
	// enable depth testing
	::glEnable(GL_DEPTH_TEST);

	return TRUE;
}


BOOL COpenGLView::SetupPixelFormat(
	PIXELFORMATDESCRIPTOR* pPFD)
{
	// default pixel format for a single-buffered,
	// OpenGL-supporting, hardware-accelerated, 
	// RGBA-mode format. Pass in a pointer to a different
	// pixel format if you want something else
	PIXELFORMATDESCRIPTOR pfd =
	{
	sizeof(PIXELFORMATDESCRIPTOR),// size of this pfd
	1,                      // version number
	PFD_DRAW_TO_WINDOW |    // support window
	  PFD_SUPPORT_OPENGL |   // support OpenGL
		PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,          // RGBA type
	8,                     // 24-bit color depth
	0, 0, 0, 0, 0, 0,       // color bits ignored
	0,                      // no alpha buffer
	0,                      // shift bit ignored
	0,                      // no accumulation buffer
	0, 0, 0, 0,             // accum bits ignored
	16,                     // 16-bit z-buffer
	0,                      // no stencil buffer
	0,                      // no auxiliary buffer
	PFD_MAIN_PLANE,         // main layer
	0,                      // reserved
	0, 0, 0                 // layer masks ignored
	};

	int pixelformat;

	PIXELFORMATDESCRIPTOR* pPFDtoUse;

	// let the user override the default pixel format
	pPFDtoUse = (0 == pPFD) ? &pfd : pPFD;

	if (0 == (pixelformat =
		::ChoosePixelFormat(m_pDC->GetSafeHdc(), pPFDtoUse)))
	{
		::AfxMessageBox("ChoosePixelFormat failed.");
		return FALSE;
	}

	if (FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(),
		pixelformat, pPFDtoUse))
	{
		::AfxMessageBox("SetPixelFormat failed.");
		return FALSE;
	}

	return TRUE;
}

// Output some outline text. If we haven't generated a display
// list for the text, then do it first.
void COpenGLView::GLTextOut(const char* const textstring)
{
	// lazy evaluation of the character display list
	if (0 == m_TextID)
	{
		GenerateDisplayListForFont("Ariel", 0.0f);
	}

	if (0 == m_TextID || 0 == textstring)
	{
		return;
	}

	GLsizei size = strlen(textstring);

	::glListBase(m_TextID);
	::glCallLists(size, GL_UNSIGNED_BYTE, (const GLvoid*)textstring);

}

// The routines used for generating 3D text
void COpenGLView::GenerateDisplayListForFont(const char* const fontname, double xt)
{
	GLuint id;

	if (0 == m_pDC ||
		0 == fontname ||
		(GLuint)0 == (id = ::glGenLists(126)))
	{
		return;
	}

	LOGFONT     logfont;
	GLYPHMETRICSFLOAT gmf[126];

	// lfHeight can't be used to change the font size
	logfont.lfHeight = -12; // use glScale to change size
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = logfont.lfEscapement;
	logfont.lfWeight = FW_NORMAL;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;
	lstrcpy(logfont.lfFaceName, fontname);

	CFont newfont;
	// returns 0 if it fails
	BOOL success = newfont.CreateFontIndirect(&logfont);
	CFont* oldfont = m_pDC->SelectObject(&newfont);
	ASSERT(0 != oldfont);

	// Create a set of display lists based on the glyphs of the TrueType font 
	// notice that we really waste the first 32 spaces....
	// if there's a problem delete the display lists
	if (0 == success ||
		FALSE == wglUseFontOutlines(m_pDC->m_hDC,
			0, 126, id, 0.0f, (float)xt, WGL_FONT_POLYGONS, gmf))
	{
		::glDeleteLists(id, 126);
		id = 0;
	}
	else
	{
		m_pDC->SelectObject(oldfont);
	}

	m_TextID = id;
}

