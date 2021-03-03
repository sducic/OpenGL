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
	beta = 30;
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

	slika = LoadTexture("Kol2011.bmp");
	//glEnable(GL_TEXTURE_2D);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//---------------------------------
	glDisable(GL_LIGHTING);
	if (svetlo)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	GLfloat pozicija[] = { 0.0,0.0,1.0,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pozicija);
	
	glTranslatef(0, 0, -10);
	glRotatef(alfa, 1.0, 0.0, 0.0);
	glRotatef(beta, 0.0, 1.0, 0.0);

	//DrawAxes();

	glPushMatrix();
	glRotatef(90, 0.0, 0.0, 1.0);
	DrawBody(1.0, 3.0, 2.0, 8.0);		//telo1
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawBody(0.5, 3.5, 3.0, 8.0);		//telo2
	DrawBody(0.25, 5.1, 5.0, 8.0);		//telo3
	glPopMatrix();
	
	glColor3f(1,0,0);
	DrawWings();					//krila
	//----------------------------------
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
	// ---------------------
		glDeleteTextures(1,&slika);
	//-------------------------
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

void CGLRenderer::DrawBody(double r, double h1, double h2, int n)
{
	float Hpiramide = (h1 - h2) / 2;

	glColor3f(1.0, 0.0, 0.0);
	DrawFigura(r,r,h2,n);

	glPushMatrix();
	glTranslatef(0.0,h2/2+Hpiramide/2,0.0);		//piramida gore
	glColor3f(0.0,1.0,0.0);
	DrawFigura(0, r, Hpiramide, n);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -h2 / 2 - Hpiramide / 2, 0.0);		//piramida dole
	glColor3f(0.0, 1.0, 0.0);
	DrawFigura(r, 0, Hpiramide, n);
	glPopMatrix();
}

void CGLRenderer::DrawFigura(double r1, double r2, double h, int n)
{
	//--------------------------------------telo
	float i = 0;
	glBegin(GL_QUAD_STRIP);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		//gore tacka
		glNormal3f(cos(angle*PI/180),0.0,sin(angle*PI/180));
		glTexCoord2f(i/n,1.0);
		glVertex3f(r1*cos(angle*PI / 180), h/2, r1*sin(angle*PI / 180));

		//dole tacka
		glNormal3f(cos(angle*PI/180),0.0,sin(angle*PI/180));
		glTexCoord2f(i/n,0.0);
		glVertex3f(r2*cos(angle*PI/180),-h/2,r2*sin(angle*PI/180));

		i++;
	}
	glEnd();
	//-----------------------------------------osnova gore
	i = 0;
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0,1.0,0.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(0.0,h/2,0.0);
	for (float angle=0;	angle<=360;	angle+=(360/n))
	{
		glNormal3f(0.0,1.0,0.0);
		glTexCoord2f(i/n, 1.0);
		glVertex3f(r1*cos(angle*PI/180),h/2,r1*sin(angle*PI/180));

		i++;
	}
	glEnd();
	//----------------------------------------osnova dole
	 i = 0;
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -h / 2, 0.0);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(i / n, 1.0);
		glVertex3f(r2*cos(angle*PI / 180),- h / 2, r2*sin(angle*PI / 180));

		i++;
	}
	glEnd();
}

void CGLRenderer::DrawWing()
{
	glBegin(GL_QUADS);
	glVertex3f(-1,-1,0);
	glVertex3f(1, -1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(-1, 1, 0);
	glEnd();
	
	glPushMatrix();
	glTranslatef(0.0,2.0, 0.0);
	glRotatef(5, 1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(-4, -1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(-1,1, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,-2.0,0.0);
	glRotatef(-5, 1.0, 0.0,0.0);
	glBegin(GL_QUADS);
	glVertex3f(-4, 1, 0);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(1, 1, 0);
	glEnd();
	glPopMatrix();

}

void CGLRenderer::DrawWings()
{
	glPushMatrix();
	glRotatef(180,0,1,0);
	glTranslatef(0,0,-2.5);
	DrawWing();
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, 2.5);
	DrawWing();
	glPopMatrix();
}

void CGLRenderer::DrawAxes()
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0,10);

	glEnd();
}
