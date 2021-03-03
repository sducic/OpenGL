#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#define PI 3.1415926
CGLRenderer::CGLRenderer(void)
{
	alfa = 20;
	beta = 0;
	rotacija = 30;
	svetlo = true;
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
	glClearColor(1.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);
	
	brick = LoadTexture("brick.png");
	env = LoadTexture("env.png");

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
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glRotatef(alfa, 1.0, 0.0, 0.0);
	glRotatef(beta, 0.0, 1.0, 0.0);
	DrawCube(1);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	if (svetlo)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	GLfloat pozicija[] = {0.0,1.0,1.0,0.0};
	

	glTranslatef(0.0,0.0,-10.0);
	glRotatef(alfa,1.0,0.0,0.0);
	glRotatef(beta,0.0,1.0,0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, pozicija);

	glBindTexture(GL_TEXTURE_2D,brick);
	glPushMatrix();
	glTranslatef(0,-2,0);
	DrawTube(1,1.5,4,8);			//valjak
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(0.8,0.8,0.8);
	glPushMatrix();
	glTranslatef(0, 0.4, 0);
	DrawCone(1.5, 0.8, 20);		//kupa
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);		//paddle
	glPushMatrix();
	glRotatef(rotacija, 0, 0, 1);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.5);
	DrawPaddle(3, 0.6);
	glRotatef(90,0,0,1);
	DrawPaddle(3, 0.6);
	glRotatef(90, 0, 0, 1);
	DrawPaddle(3, 0.6);
	glRotatef(90, 0, 0, 1);
	DrawPaddle(3, 0.6);
	glPopMatrix();
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
	// ... 
	glDeleteTextures(1,&brick);
	glDeleteTextures(1, &env);
	//
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
	img.Load(CString("res/") + CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawCube(double a)
{
	glBindTexture(GL_TEXTURE_2D,env);
	//front
	glBegin(GL_QUADS);
	glTexCoord2f(0.25,1.0/3);
	glVertex3f(-a/2,-a/2,a/2);				//0
	glTexCoord2f(0.5, 1.0 / 3);
	glVertex3f(a / 2, -a / 2, a / 2);		//1
	glTexCoord2f(0.5, 0.0);
	glVertex3f(a / 2, a / 2, a / 2);		//2
	glTexCoord2f(0.25, 0.0);
	glVertex3f(-a / 2, a / 2, a / 2);		//3
	glEnd();
	//back
	glPushMatrix();
	glRotatef(90,0.0,0.0,1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.25, 2.0/3);
	glVertex3f(-a / 2, -a / 2, -a / 2);		//4
	glTexCoord2f(0.25, 1.0 );
	glVertex3f(a / 2, -a / 2, -a / 2);		//5
	glTexCoord2f(0.5,1.0);
	glVertex3f(a / 2, a / 2,- a / 2);		//6
	glTexCoord2f(0.5, 2.0/3);
	glVertex3f(-a / 2, a / 2,- a / 2);		//7
	glEnd();
	glPopMatrix();
	//bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0.25, 1.0/3);
	glVertex3f(-a / 2, -a / 2, a / 2);		//0
	glTexCoord2f(0.5, 1.0/3);
	glVertex3f(a / 2, -a / 2, a / 2);		//1
	glTexCoord2f(0.5,2.0/3);
	glVertex3f(a / 2, -a / 2, -a / 2);		//5
	glTexCoord2f(0.25,2.0/3);
	glVertex3f(-a / 2, -a / 2, -a / 2);		//4
	glEnd();
	//top
	glBegin(GL_QUADS);
	glTexCoord2f(0.75, 1.0/3);
	glVertex3f(-a / 2, a / 2, a / 2);		//3
	glTexCoord2f(1.0, 1.0/3);
	glVertex3f(a / 2, a / 2, a / 2);		//2
	glTexCoord2f(1.0, 2.0/3);
	glVertex3f(a / 2, a / 2, -a / 2);		//6
	glTexCoord2f(0.75, 2.0/3);
	glVertex3f(-a / 2, a / 2, -a / 2);		//7
	glEnd();
	//left
	glPushMatrix();
	glRotatef(-90,1,0,0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.5, 1.0 / 3);
	glVertex3f(-a / 2, -a / 2, -a / 2);		//4
	glTexCoord2f(0.75, 1.0 / 3);
	glVertex3f(-a / 2, -a / 2, a / 2);		//0
	glTexCoord2f(0.75, 2.0 / 3);
	glVertex3f(-a / 2, a / 2, a / 2);		//3
	glTexCoord2f(0.5, 2.0 / 3);
	glVertex3f(-a / 2, a / 2, -a / 2);		//7
	glEnd();
	glPopMatrix();
	//right
	glPushMatrix();
	glRotatef(-90,1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0 / 3);
	glVertex3f(a / 2, -a / 2, a / 2);		//1
	glTexCoord2f(0.25, 1.0 / 3);
	glVertex3f(a / 2, -a / 2, -a / 2);		//5
	glTexCoord2f(0.25, 2.0 / 3);
	glVertex3f(a / 2, a / 2, -a / 2);		//6
	glTexCoord2f(0.0, 2.0 / 3);
	glVertex3f(a / 2, a / 2, a / 2);		//2
	glEnd();
	glPopMatrix();
}

void CGLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	float i = 0;
	glBegin(GL_QUAD_STRIP);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		glNormal3f(cos(angle*PI/180),0.0,sin(angle*PI/180));
		glTexCoord2f(i/n,1.0);
		glVertex3f(r1*cos(angle*PI / 180), h/2, r1*sin(angle*PI / 180));

		glNormal3f(cos(angle*PI / 180), 0.0, sin(angle*PI / 180));
		glTexCoord2f(i/n, 0.0);
		glVertex3f(r2*cos(angle*PI / 180), -h / 2, r2*sin(angle*PI / 180));

		i++;
	}
	glEnd();

	i = 0;
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0,1.0,0.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(0.0,h/2,0.0);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(i / n, 1.0);
		glVertex3f(r1*cos(angle*PI / 180), h/2, r1*sin(angle*PI / 180));

		i++;
	}
	glEnd();

	i = 0;
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -h/2, 0.0);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(i / n, 1.0);
		glVertex3f(r2*cos(angle*PI / 180), -h/2, r2*sin(angle*PI / 180));

		i++;
	}
	glEnd();

	
}

void CGLRenderer::DrawCone(double r, double h, int n)
{
	DrawTube(0, r, h, n);
}

void CGLRenderer::DrawPaddle(double length, double width)
{
	glBegin(GL_POLYGON);
	glNormal3f(0.0,0.0,1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(length, 0.0, 0.0);
	glVertex3f(length, width, 0.0);
	glVertex3f(length/8*2, width, 0.0);
	glVertex3f(length / 8, width/8, 0.0);
	glVertex3f(0.0, width / 8, 0.0);
	glEnd();
}
