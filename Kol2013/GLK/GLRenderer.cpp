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
	alfa =-10;
	beta = 15;
	svetlo = true;
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
	glClearColor(0.0,0.0,0.9,1.0);
	glEnable(GL_DEPTH_TEST);

	e1 = LoadTexture("E1.bmp");
	e2 = LoadTexture("E2.bmp");
	e3 = LoadTexture("E3.bmp");
	e4 = LoadTexture("E4.bmp");
	metal = LoadTexture("Metal1.bmp");

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
	//-----------------------------
	glTranslatef(0,-3,-12);
	glRotatef(alfa,1,0,0);
	glRotatef(beta, 0, 1, 0);

	glDisable(GL_LIGHTING);
	if (svetlo)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	
	GLfloat pozicija[] = { 10,15,10,0 };
	glLightfv(GL_LIGHT0,GL_POSITION,pozicija);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,e1);
	glRotatef(rotacija,0,1,0);

	DrawFPyramid(4, 2.4, 1.5);

	glBindTexture(GL_TEXTURE_2D, e2);
	glTranslatef(0,1.5,0);
	DrawFPyramid(2.4, 1.2, 1.5);

	glBindTexture(GL_TEXTURE_2D, e4);
	glTranslatef(0, -0.75, 0);
	DrawFPyramid(2.4, 2.64, 0.25);

	glPushMatrix();
	glTranslatef(0, 1.5, 0);
	glScalef(0.5,1.0,0.5);
	DrawFPyramid(2.4, 2.64, 0.25);
	glPopMatrix();
	glTranslatef(0,4.5,0);
	glBindTexture(GL_TEXTURE_2D, e3);
	DrawFPyramid(1.2, 0.0,6.0);
	glPopMatrix();
	//Zarubljena(0,1,2,6);
	//-----------------------------
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
	gluPerspective(45,(double)w/(double)h,0.1,2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ---------------------------------
	glDeleteTextures(1,&e1);
	glDeleteTextures(1, &e2);
	glDeleteTextures(1, &e3);
	glDeleteTextures(1, &e4);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());


	return texID;
}

void CGLRenderer::Zarubljena(float r1, float r2, float h, int n)
{
	float i = 0.0;
	
	//telo
	glBegin(GL_QUAD_STRIP);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		//gore
		glNormal3f(cos(angle*PI / 180), 0.0, sin(angle*PI / 180));
		glTexCoord2f(i/n , 1.0);
		glVertex3f(r1 *cos(angle*PI / 180), h / 2, r1*sin(angle*PI / 180));

		//dole
		glNormal3f(cos(angle*PI / 180), 0.0, sin(angle*PI / 180));
		glTexCoord2f(i/n , 0.0);
		glVertex3f(r2* cos(angle*PI / 180), -h / 2, r2 * sin(angle*PI / 180));

		i++;
		
	}
	glEnd();

	//osnova gore
	i = 0;
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, h / 2, 0.0);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(i / n, 1.0);
		glVertex3f(r1 * cos(angle*PI / 180), h / 2, r1 * sin(angle*PI / 180));

		i++;
	}
	glEnd();

	//osnova dole
	i = 0;
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -h / 2, 0.0);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(i / n, 1.0);
		glVertex3f(r2 * cos(angle*PI / 180), -h / 2, r2 *sin(angle*PI / 180));

		i++;
	}
	glEnd();
}

void CGLRenderer::DrawFPyramid(float a, float b, float h)
{
	if (h < 5)
	{
		DrawStranicu(a, b, h);

		glRotatef(90, 0, 1, 0);

		DrawStranicu(a, b, h);
		glRotatef(90, 0, 1, 0);

		DrawStranicu(a, b, h);
		glRotatef(90, 0, 1, 0);

		DrawStranicu(a, b, h);
	}

	if (h > 5)
	{
		DrawStranicu1(a, b, h);

		glRotatef(90, 0, 1, 0);

		DrawStranicu1(a, b, h);
		glRotatef(90, 0, 1, 0);

		DrawStranicu1(a, b, h);
		glRotatef(90, 0, 1, 0);

		DrawStranicu1(a, b, h);
	}

	/*
	glBegin(GL_TRIANGLES);//note that we can draw more than one figure between Begin and End
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-b / 2, h / 2, b / 2);	 //bottom left
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(b / 2, h / 2, b / 2); //bottom right
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-a / 2, -h / 2, a / 2); //top left
															  //TexCoords
															  //0,1----1,1
															  //| \      |
															  //|  \     |
															  //|   \    |
															  //|    \   |
															  //|     \  |
															  //|      \ |
															  //0,0----1,0
															  //VertexCoords
															  //-x,-y---- x,-y
															  //  | \      |
															  //  |  \     |
															  //  |   \    |
															  //  |    \   |
															  //  |     \  |
															  //  |      \ |
															  //-x, y---- x, y
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(b / 2, h / 2, b / 2); //bottom right
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-a / 2, -h / 2, a / 2); //top left
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(a / 2, -h / 2, a / 2);	 //top right
	glEnd();*/
}

void CGLRenderer::DrawStranicu(float a, float b, float h)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.2, 0.0);
	glVertex3f(-b / 2, h / 2, b / 2);
	glTexCoord2f(0.8, 0.0);
	glVertex3f(b / 2, h / 2, b / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a / 2, -h / 2, a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, -h / 2, a / 2);
	glEnd();
}

void CGLRenderer::DrawStranicu1(float a, float b, float h)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(-b / 2, h / 2, b / 2);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(b / 2, h / 2, b / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a / 2, -h / 2, a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, -h / 2, a / 2);
	glEnd();
}
