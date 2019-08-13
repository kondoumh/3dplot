// OpenGLView.h :
//
/////////////////////////////////////////////////////////////////////////////

#include "gl\gl.h"
#include "gl\glu.h"

class COpenGLView : public CView
{
protected:
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

public:
	C3dplotDoc* GetDocument();

public:

	//{{AFX_VIRTUAL(COpenGLView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

protected:
	//{{AFX_MSG(COpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual BOOL InitializeOpenGL(void);
	virtual BOOL SetupPixelFormat(PIXELFORMATDESCRIPTOR* pPFD = 0);
	virtual	BOOL SetupViewingFrustum(void);
	virtual	BOOL SetupViewingTransform(void);

	virtual void PreRenderScene(void) // just clear the viewport
	{
		::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	virtual void RenderStockScene(void);
	virtual void RenderScene(void);
	virtual void PostRenderScene(void) { return; }


	void		GenerateDisplayListForFont(const char* const fontname, double xt);
	void		GLTextOut(const char* const textstring);


	GLdouble	m_AspectRatio;
	GLsizei		m_WindowWidth, m_WindowHeight;
	GLuint		m_TextID;

protected:

	HGLRC		m_hRC;
	CDC* m_pDC;

};

#ifndef _DEBUG
inline C3dplotDoc* COpenGLView::GetDocument()
{
	return (C3dplotDoc*)m_pDocument;
}
#endif


