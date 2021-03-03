#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	alfa = 45;
	beta = 30;
	svetlo = false;
	rotacija = 0;
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
	glClearColor(0.8,0.8,0.8,1.0);
	glEnable(GL_DEPTH_TEST);

	slika = LoadTexture("OpenGL.bmp");
	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//---------------------------------
	glTranslatef(0.0,0.0,-10);
	glRotatef(alfa,1.0,0.0,0.0);
	glRotatef(-beta, 0.0, 1.0, 0.0);

	DrawAxes();

	glDisable(GL_LIGHTING);
	if (svetlo)
	{	glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);	}

	GLfloat pozicija[] = {5.0,20.0,0.0,0.0};
	glLightfv(GL_LIGHT0,GL_POSITION,pozicija);

	glPushMatrix();
	glRotatef(rotacija,0.0,1.0,0.0);
	DrawRubikCube(1, 3);
	glPopMatrix();

	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,(double)w/(double)h,0.1,2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ---------------------------------
	glDeleteTextures(1,&slika);

	//-----------------------------------
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char * fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString("res/")+CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1,&texID);

	glBindTexture(GL_TEXTURE_2D,texID);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,img.Width(),img.Height(),GL_BGRA_EXT,GL_UNSIGNED_BYTE,img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawCube(float a)
{	
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	NacrtajStranu(a);

	glRotatef(90,0.0,1.0,0.0);
	glColor3f(0.0, 0.0, 1.0);
	NacrtajStranu(a);

	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(1.0, 0.5, 0.0);
	NacrtajStranu(a);

	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.5, 0.0);
	NacrtajStranu(a);

	glRotatef(90, 1.0, 0.0, 0.0);
	glColor3f(1, 1, 0);
	glPushMatrix();
	glRotatef(90, 0.0, 0.0, 1.0);
	NacrtajStranu(a);
	glPopMatrix();

	glRotatef(-180, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	glColor3f(1, 1, 1);
	NacrtajStranu(a);
	glPopMatrix();
}

void CGLRenderer::NacrtajStranu(float a)
{

	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a/2, -a/2, a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a/2, -a/2, a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(a/2, a/2, a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a/2, a/2, a / 2);
	glEnd();
}

void CGLRenderer::DrawRubikCube(double a, int count)
{
	
	glTranslatef(-a, -a, -a);
	
	glPushMatrix();
	for (int z = 0; z < count; z++)
	{
		glPushMatrix();
		for (int i = 0; i < count; i++)
		{
			glPushMatrix();
			for (int j = 0; j < count; j++)
			{
				DrawCube(a);
				glTranslatef(a, 0, 0);
				glTranslatef(a/100*5, 0, 0);
			}
			glPopMatrix();
			glTranslatef(0, 0, a);
			glTranslatef(0, 0, a / 100 * 5);
		}
		glPopMatrix();

		glTranslatef(0, a, 0);
		glTranslatef(0, a / 100 * 5, 0);
	}
	glPopMatrix();
}

void CGLRenderer::DrawAxes()
{
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,0.0);

	glVertex3f(0.0,0.0,0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}
