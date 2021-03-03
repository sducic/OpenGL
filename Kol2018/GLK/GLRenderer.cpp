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
	beta = 10;
	daljina = -5;
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

	Ship = LoadTexture("ShipT1.png");
	Space[0] = LoadTexture("front.jpg");
	Space[1] = LoadTexture("left.jpg");
	Space[2] = LoadTexture("right.jpg");
	Space[3] = LoadTexture("back.jpg");
	Space[4] = LoadTexture("top.jpg");
	Space[5] = LoadTexture("bot.jpg");
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
	//------------
	//gluLookAt(10,10,10,0,0,0,0,1,0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glRotatef(alfa,1.0,0.0,0.0);
	glRotatef(beta, 0.0, 1.0, 0.0);
	DrawSpace(1.0);
	glPopMatrix();


	glEnable(GL_DEPTH_TEST);

	
	glTranslatef(0.0,0.0,daljina);
	glRotatef(alfa, 1.0, 0.0, 0.0);
	glRotatef(beta, 0.0, 1.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	//osveteljenjeeeeeeeeeeee**************
	GLfloat light_position[] = { 0.0,0.0,1.0,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	if (svetlo)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	//------------******************
	double d = sqrt(5.8*5.8+2.15*2.15);
	glTranslatef(-d/2.,0,0);
	glRotatef(90,1,0,0);
	DrawShip();


	
	
	//------------
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
	gluPerspective(50,(double)w/(double)h,0.1,2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(1,&Ship);
	glDeleteTextures(6,Space);

	//----
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString("res/")+CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1,&texID);

	glBindTexture(GL_TEXTURE_2D,texID);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,img.Width(),img.Height(),GL_BGRA_EXT,GL_UNSIGNED_BYTE,img.GetDIBBits());

	return texID;
	
}

void CGLRenderer::DrawSpace(float a)
{
	//front
	glBindTexture(GL_TEXTURE_2D, Space[0]);
	glBegin(GL_QUADS);
	glNormal3f(0.0,0.0,1.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(-a/2, a/2,a/2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -a / 2, a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a / 2, a / 2, a / 2);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, Space[1]);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, -a / 2, a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-a / 2, a / 2, a / 2);
	glEnd();
	

	//right
	glBindTexture(GL_TEXTURE_2D, Space[2]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(a / 2, a / 2, a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a / 2, a / 2, -a / 2);
	glEnd();
	//back
	glBindTexture(GL_TEXTURE_2D, Space[3]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(a / 2, -a / 2,- a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a / 2, a / 2,- a / 2);
	glEnd();
	//top
	glBindTexture(GL_TEXTURE_2D, Space[4]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, a / 2, a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(a / 2, a / 2, a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a / 2, a / 2, -a / 2);
	glEnd();
	//bot
	glBindTexture(GL_TEXTURE_2D, Space[5]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2,- a / 2, a / 2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(a / 2,- a / 2, a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a / 2,- a / 2, -a / 2);
	glEnd();
}

void CGLRenderer::DrawTriangle(float d1, float d2)
{

	glBegin(GL_TRIANGLES);
	//glColor3f(1.0,1.0,1.0);

	glNormal3f(0.0,0.0,1.0);

	glTexCoord2f(0.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(d1, 0.0, 0.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(d1, d2, 0.0);


	glEnd();
}

void CGLRenderer::DrawShip()
{
	glBindTexture(GL_TEXTURE_2D,Ship);
	double a1 = atan2(2.15,5.8);
	
	glPushMatrix();
	glRotatef(-4.75,0,1,0);
	glRotatef(13,1,0,0);
	glRotatef(-a1 * 180 / PI, 0, 0, 1);
	DrawTriangle(5.8,2.15);
	glPopMatrix();
		
	glPushMatrix();
	glScalef(1,-1,1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1*180/PI,0,0,1);
	DrawTriangle(5.8, 2.15);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180,1,0,0);

	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * 180 / PI, 0, 0, 1);
	DrawTriangle(5.8, 2.15);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1 * 180 / PI, 0, 0, 1);
	DrawTriangle(5.8, 2.15);
	glPopMatrix();

	glPopMatrix();


}

void CGLRenderer::DrawKvadrat(float size)
{

	glBegin(GL_QUADS);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-size/2,size/2,0.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-size / 2, -size / 2, 0.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(size / 2, -size / 2, 0.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(size / 2, size / 2, 0.0);

	glEnd();
}
