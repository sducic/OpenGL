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
	daljina = -8;
	alfa = 20;
	beta = 15;
	svetlo = false;
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
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);

	kocka = LoadTexture("Marble.bmp");
	sobaTex = LoadTexture("Soba.bmp");
	lampaTex = LoadTexture("Vase.bmp");
	metal= LoadTexture("Metal1.bmp");
	



	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//-----------------
	//gluLookAt(5.0,5.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(alfa, 1.0, 0.0, 0.0);
	glRotatef(-beta, 0.0, 1.0, 0.0);
	DrawSoba(1.0);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	//-----------------------------osvetljenje-------------			
	if(svetlo)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	GLfloat light_position[] = {6.0,4.0,6.0,0.0};
	//glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	//------------------------------------------------------------------


	glLineWidth(2.0);
	//glPushMatrix();
	glTranslatef(0, 0, daljina);
	glRotatef(alfa, 1.0, 0.0, 0.0);
	glRotatef(beta, 0.0, 1.0, 0.0);
	//glColor3f(1,0,0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);		//ovde bi trebalo da se postavi osvetljenje
	glPushMatrix();
	glTranslatef(0.0,-1.0,0.0);
	DrawKocka(1.5);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D,lampaTex);
	glPushMatrix();
	glTranslatef(0.0,2.0,0.0);
	glRotatef(180,1.0,0.0,0.0);
	Abazur(1,0.5, 1, 4);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D,metal);
	glPushMatrix();
	DrawValjak(0.2, 3.2, 10);
	//glutSolidTeapot(2);
	glPopMatrix();
	//glPopMatrix();
	//-----------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
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
	// ... 

	glDeleteTextures(1,&kocka);
	glDeleteTextures(1, &sobaTex);
	glDeleteTextures(1, &lampaTex);
	glDeleteTextures(1, &metal);

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

void CGLRenderer::DrawKocka(double size)
{
	glBindTexture(GL_TEXTURE_2D,kocka);
	glBegin(GL_QUADS);
	glNormal3f(0.0,0.0,1.0);
	glTexCoord2f(0.0,1.0/3);
	glVertex3f(-size/2,-size/2,size/2);				//0
	glTexCoord2f(1.0/4, 0.0);
	glVertex3f(size / 2, -size / 2, size / 2);		//1
	glTexCoord2f(1.0, 1.0);
	glVertex3f(size / 2, size / 2, size / 2);		//2
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-size / 2, size / 2, size / 2);		//3
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(size / 2, -size / 2, size / 2);		//1
	glTexCoord2f(1.0, 0.0);
	glVertex3f(size / 2, -size / 2, -size / 2);		//5
	glTexCoord2f(1.0, 1.0);
	glVertex3f(size / 2, size / 2, -size / 2);		//6
	glTexCoord2f(0.0, 1.0);
	glVertex3f(size / 2, size / 2, size / 2);		//2
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(size / 2, -size / 2, -size / 2);		//5
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-size / 2, -size / 2, -size / 2);	//4
	glTexCoord2f(1.0,1.0);
	glVertex3f(-size / 2, size / 2, -size / 2);		//7
	glTexCoord2f(0.0, 1.0);
	glVertex3f(size / 2, size / 2, -size / 2);		//6
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-size / 2, -size / 2, -size / 2);	//4
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-size / 2, -size / 2, size / 2);		//0
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-size / 2, size / 2, size / 2);		//3
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-size / 2, size / 2, -size / 2);		//7
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-size / 2, size / 2, size / 2);		//3
	glTexCoord2f(1.0, 0.0);
	glVertex3f(size / 2, size / 2, size / 2);		//2
	glTexCoord2f(1.0, 1.0);
	glVertex3f(size / 2, size / 2, -size / 2);		//6
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-size / 2, size / 2, -size / 2);		//7
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-size / 2, -size / 2, size / 2);		//0
	glTexCoord2f(1.0, 0.0);
	glVertex3f(size / 2, -size / 2, size / 2);		//1
	glTexCoord2f(1.0, 1.0);
	glVertex3f(size / 2, -size / 2, -size / 2);		//5
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-size / 2, -size / 2, -size / 2);	//4
	glEnd();


}

void CGLRenderer::Abazur(double r1, double r2, double h, int n)
{
	float i = 0;
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
		glTexCoord2f(i /n, 0.0);
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

void CGLRenderer::Telo(double r1, double r2, double r3, double h, int step)
{
}

void CGLRenderer::DrawValjak(float r, float h, int n)
{
	float i = 0;
	//telo
	glBegin(GL_QUAD_STRIP);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{

		//gore
		glNormal3f(cos(angle*PI / 180), 0.0, sin(angle*PI / 180));
		glTexCoord2f(i / n, 1.0);
		glVertex3f(r *cos(angle*PI / 180), h / 2, r*sin(angle*PI / 180));

		//dole
		glNormal3f( cos(angle*PI/180), 0.0, sin(angle*PI/180));
		glTexCoord2f(i / n, 0.0);
		glVertex3f(r * cos(angle*PI/180), -h / 2, r * sin(angle*PI/180));


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
		glVertex3f(r * cos(angle*PI/180), h / 2, r * sin(angle*PI/180));

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
		glVertex3f(r * cos(angle*PI/180), -h / 2, r *sin(angle*PI/180));

		i++;
	}
	glEnd();
}

void CGLRenderer::DrawSoba(float size)
{
	glBindTexture(GL_TEXTURE_2D, sobaTex);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.33);
	glVertex3f(-size / 2, -size / 2, size / 2);		//0
	glTexCoord2f(0.25, 0.33);
	glVertex3f(size / 2, -size / 2, size / 2);		//1
	glTexCoord2f(0.25, 0.66);
	glVertex3f(size / 2, size / 2, size / 2);		//2
	glTexCoord2f(0.0, 0.66);
	glVertex3f(-size / 2, size / 2, size / 2);		//3
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.25, 0.33);
	glVertex3f(size / 2, -size / 2, size / 2);		//1
	glTexCoord2f(0.5, 0.33);
	glVertex3f(size / 2, -size / 2, -size / 2);		//5
	glTexCoord2f(0.5, 0.66);
	glVertex3f(size / 2, size / 2, -size / 2);		//6
	glTexCoord2f(0.25, 0.66);
	glVertex3f(size / 2, size / 2, size / 2);		//2
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(0.5, 0.33);
	glVertex3f(size / 2, -size / 2, -size / 2);		//5
	glTexCoord2f(0.75, 0.33);
	glVertex3f(-size / 2, -size / 2, -size / 2);	//4
	glTexCoord2f(0.75, 0.66);
	glVertex3f(-size / 2, size / 2, -size / 2);		//7
	glTexCoord2f(0.5,0.66);
	glVertex3f(size / 2, size / 2, -size / 2);		//6
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.75, 0.33);
	glVertex3f(-size / 2, -size / 2, -size / 2);	//4
	glTexCoord2f(1.0, 0.33);
	glVertex3f(-size / 2, -size / 2, size / 2);		//0
	glTexCoord2f(1.0,0.66);
	glVertex3f(-size / 2, size / 2, size / 2);		//3
	glTexCoord2f(0.75, 0.66);
	glVertex3f(-size / 2, size / 2, -size / 2);		//7
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.25,2.0/3);
	glVertex3f(-size / 2, size / 2, size / 2);		//3
	glTexCoord2f(0.5, 2.0/3);
	glVertex3f(size / 2, size / 2, size / 2);		//2
	glTexCoord2f(0.5,1.0);
	glVertex3f(size / 2, size / 2, -size / 2);		//6
	glTexCoord2f(0.25, 1.0);
	glVertex3f(-size / 2, size / 2, -size / 2);		//7
	glEnd();
	
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.25, 0.0);
	glVertex3f(-size / 2, -size / 2, size / 2);		//0
	glTexCoord2f(0.5, 0.0);
	glVertex3f(size / 2, -size / 2, size / 2);		//1
	glTexCoord2f(0.5, 1.0 / 3);
	glVertex3f(size / 2, -size / 2, -size / 2);		//5
	glTexCoord2f(0.25, 1.0 / 3);
	glVertex3f(-size / 2, -size / 2, -size / 2);	//4
	glEnd();
}
