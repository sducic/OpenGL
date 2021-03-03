#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	daljina = -10;
	ugao1 = 20;
	ugao2 = 30;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0,1.0,1.0,0.0);
	glEnable(GL_DEPTH_TEST);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//***************
	//gluLookAt(5.0,5.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);

	glTranslatef(0.0, 0.0, daljina);
	glRotatef(ugao1,1.0,0.0,0.0);
	glRotatef(ugao2, 0.0, 1.0, 0.0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(0.0,0.0,0.0);
	glLineWidth(1.0);
	DrawGrid(5.0,20);

	glLineWidth(3.0);
	DrawAxes(4);

	glColor3f(0.5, 0.5, 0.5);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPushMatrix();
//	glTranslatef(0.0,2.0,0.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 2.0, 0.0);
	DrawCube(1.0);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT, GL_LINE);
	glLineWidth(2.0);
	//DrawCube(1.0);
	//***************
	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0,0,(GLsizei) w,(GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,(double)w/(double)h,1,100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes(double len)
{
	glBegin(GL_LINES);

	//x
	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(len, 0.0, 0.0);

	//y
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, len, 0.0);

	//z
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, len);

	glEnd();

}

void CGLRenderer::DrawGrid(double dSize, int nSteps)
{
	double stepSize = dSize / nSteps;
	double Zmin = -dSize / 2.0;

	for (int i=0;i<nSteps;i++)
	{
		double Xmin = -dSize / 2.0;
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j < nSteps; j++)
		{
			glVertex3d(Xmin,0.0,Zmin);
			glVertex3d(Xmin,0.0,Zmin+stepSize);
			Xmin += stepSize;
		}
		glEnd();
		Zmin += stepSize;
	}
}

void CGLRenderer::DrawCube(double a)
{
	glBegin(GL_QUAD_STRIP);

	glVertex3f(-a / 2, a / 2, a / 2);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glVertex3f(a / 2, a / 2, a / 2);
	glVertex3f(a / 2, -a / 2, a / 2);

	glVertex3f(a / 2, a / 2, -a / 2);
	glVertex3f(a / 2, -a / 2, -a / 2);

	glVertex3f(-a / 2, a / 2, -a / 2);
	glVertex3f(-a / 2, -a / 2,- a / 2);

	glVertex3f(-a / 2, a / 2, a / 2);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3f(-a / 2, a / 2, a / 2);
	glVertex3f(a / 2, a / 2, a / 2);
	glVertex3f(a / 2, a / 2, -a / 2);
	glVertex3f(-a / 2, a / 2, -a / 2);

	glVertex3f(-a / 2, -a / 2, -a / 2);
	glVertex3f(a / 2, -a / 2, -a / 2);
	glVertex3f(a / 2, -a / 2, a / 2);
	glVertex3f(-a / 2, -a / 2, a / 2);
		
	glEnd();

}

UINT CGLRenderer::LoadTexture(char * fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1,&texID);

	glBindTexture(GL_TEXTURE_2D,texID);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,img.Width(),img.Height(),GL_BGRA_EXT,GL_UNSIGNED_BYTE,img.GetDIBBits());

	return texID;



}
