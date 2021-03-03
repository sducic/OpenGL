#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
const double M_PI = 3.141592653589793238463;
CGLRenderer::CGLRenderer(void)
{
	alfa =20;
	beta = -7;
	daljina = -5;
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
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	trava = LoadTexture("trava1.png");
	kuca = LoadTexture("kuca.jpg");
	krov = LoadTexture("krov.jpg");
	drvo = LoadTexture("bark.jpg");
	lisce = LoadTexture("lisce.jpg");

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
	//------------------
//	gluLookAt(5.0,5.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);
	glTranslatef(0,0,daljina);
	glRotatef(alfa,1.0,0.0,0.0);
	glRotatef(beta,0.0,1.0,0.0);



	SetLightModel();

	DrawQuad(2.0);

	DrawKuca(0.4);

	glPushMatrix();
	glTranslatef(0.0,1.0,0.0);
	DrawPiramida();
	glPopMatrix();

	DrawSuma();

	//glTranslated(2.0, 1.5, 0);
	//SetLampLight();
	//glutSolidSphere(0.2, 100, 100);
	//------------------
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
	glDeleteTextures(1, &trava);
	glDeleteTextures(1, &kuca);
	glDeleteTextures(1, &krov);
	glDeleteTextures(1, &drvo);
	glDeleteTextures(1, &lisce);

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

void CGLRenderer::DrawGrid(float size, float steps)
{
	float Zmin = -size / 2;
	float korak = size / steps;
	
	glNormal3f(0.0,1.0,0.0);

	float texX, texZ;
	texX = 0;
	texZ = 0;

	for (int i = 0; i < steps; i++)
	{
		float Xmin = -size / 2;
		glBegin(GL_QUAD_STRIP);
		for (int  j=0;j<steps;j++)
		{
			glTexCoord2f(texX, texZ + korak);
			glVertex3f(Xmin,0.0,Zmin);

			glTexCoord2f(texX, texZ);
			glVertex3f(Xmin, 0.0, Zmin+korak);

			Xmin += korak;
			texX += korak;
		}
		glEnd();
		Zmin += korak;

		texX = 0;
		texZ -= korak;


	}
}

void CGLRenderer::DrawQuad(float size)
{
	glBindTexture(GL_TEXTURE_2D, trava);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-size, 0.0, -size);
	glTexCoord2f(4.0, 0.0);
	glVertex3f(size, 0.0, -size);
	glTexCoord2f(4.0, 4.0);
	glVertex3f(size, 0.0, size);
	glTexCoord2f(0.0, 4.0);
	glVertex3f(-size, 0.0, size);

	glEnd();
}

void CGLRenderer::DrawQuadKuca(float size)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,kuca);
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-size, 0.0, -size);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(size, 0.0, -size);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(size, 0.0, size);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-size, 0.0, size);

	glEnd();
}

void CGLRenderer::DrawKuca(float size)
{
	glPushMatrix();
	glTranslatef(0,size,-size);
	glRotatef(90,1.0,0.0,0.0);
	DrawQuadKuca(size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, size, size);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawQuadKuca(size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(size, size,0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawQuadKuca(size);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-size, size, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	DrawQuadKuca(size);
	glPopMatrix();


	
}

void CGLRenderer::DrawValjak(float r, float h, int n)
{
	int i = 0;
	//telo
	glBegin(GL_QUAD_STRIP);
	for (float angle = 0; angle <= 360; angle += (360 / n))
	{
		//dole
		glNormal3f(r * cos(toRad(angle)), 0.0, r*sin(toRad(angle)));
		glTexCoord2f((float)i / n, 0.0);
		glVertex3f(r * cos(toRad(angle)), -h / 2, r * sin(toRad(angle)));

		//gore
		glNormal3f(r*cos(toRad(angle)), 0.0, r*sin(toRad(angle)));
		glTexCoord2f((float)i / n, 1.0);
		glVertex3f(r *cos(toRad(angle)), h / 2, r*sin(toRad(angle)));

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
		glTexCoord2f((float)i / n, 1.0);
		glVertex3f(r * cos(toRad(angle)), h / 2, r * sin(toRad(angle)));

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
		glTexCoord2f((float)i / n, 1.0);
		glVertex3f(r * cos(toRad(angle)), -h / 2, r *sin(toRad(angle)));
	}
	glEnd();
}


void CGLRenderer::DrawKupa()
{
	float h = 0.4;//visina
	float r =0.3;//precnik

	float ivica = sqrt(pow(h, 2) + pow((r / 2), 2)); //ivica kupe
	float sin_ugao = h / ivica; // sin ugla ivice
	float ugao_ivice = asin(sin_ugao) * 180 / M_PI; // ugao ivice

													//telo kupe
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, h / 2, 0.0);
	for (int angle = 0; angle <= 360; angle += 5) {
		glNormal3f(cos(toRad(angle - ugao_ivice)), 0.0, sin(toRad(angle - ugao_ivice)));
		glTexCoord2d(angle, 1.0);
		glVertex3f(r * cos(toRad(angle)), -h / 2, r * sin(toRad(angle)));
	}
	glEnd();

	// osnova cilinda : dole
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, -h / 2, 0.0);
	for (int angle = 0; angle <= 360; angle += 5) {
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(angle, 1.0);
		glVertex3f(r * cos(toRad(angle)), -h / 2, r * sin(toRad(angle)));
	}
	glEnd();

}

void CGLRenderer::DrawDrvo()
{
	//glColor3f(0.5f, 0.35f, 0.05f);
	glBindTexture(GL_TEXTURE_2D, drvo);
	glTranslatef(0,0.2,0);
	DrawValjak(0.1,0.4, 300);

	glTranslatef(0, 0.4, 0);
	glBindTexture(GL_TEXTURE_2D, lisce);
	DrawCone(0.3,0.5,10,3,3);

}

void CGLRenderer::DrawSuma()
{
	glPushMatrix();
	glTranslatef(0.5, 0,- 0.8);
	DrawDrvo();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0, 1.0);
	DrawDrvo();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.8, 0, 0.5);
	DrawDrvo();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 0, -1.2);
	DrawDrvo();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0, 0, -1.6);
	DrawDrvo();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, 0, 1.6);
	DrawDrvo();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2, 0, -1.3);
	DrawDrvo();
	glPopMatrix();

}

void CGLRenderer::DrawPiramida()
{
	// podaci o visini i sirini osnove
	float a = 1.0;
	float h = 0.4;
	glBindTexture(GL_TEXTURE_2D, krov);
	//osnova
	glBegin(GL_QUADS);

	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -h / 2, a / 2);//0
	glTexCoord2f(1.0, 0.0);
	glVertex3f(a / 2, -h / 2, a / 2);//1
	glTexCoord2f(1.0, 1.0);
	glVertex3f(a / 2, -h / 2, -a / 2);//2
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-a / 2, -h / 2, -a / 2);//3

	glEnd();

	// gornji deo piramide
	glBegin(GL_TRIANGLES);

	// za uglove normale
	float nal = h;
	float nas = a / 2;
	float angle = (atan(nas / nal)) * 180 / M_PI;//toDeg

												 // napred
	glNormal3f(cos(toRad(angle)), sin(toRad(angle)), 0.0);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, h / 2, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -h / 2, a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(a / 2, -h / 2, a / 2);

	// desno
	glNormal3f(cos(toRad(angle)), sin(toRad(angle)), 0.0);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, h / 2, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(a / 2, -h / 2, a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(a / 2, -h / 2, -a / 2);

	// nazad
	glNormal3f(cos(toRad(angle)), sin(toRad(angle)), 0.0);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, h / 2, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(a / 2, -h / 2, -a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-a / 2, -h / 2, -a / 2);

	// levo
	glNormal3f(cos(toRad(angle)), sin(toRad(angle)), 0.0);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(0.0, h / 2, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-a / 2, -h / 2, -a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-a / 2, -h / 2, a / 2);

	glEnd();
}
float CGLRenderer::toRad(float angle)
{
	return (angle * M_PI) / 180;
}

void CGLRenderer::SetLampLight()
{
	float amb[] = { 1.0, 1.0, 0.0, 1 };
	float dif[] = { 1,1,1,1 };
	float specular[] = { 1,1,1,1 };
	float pos[] = { 0,3,0 };
	float direction[] = { 1,0,0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);		//konstantno slabljenje
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);		//linearno slabljenje
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);	//opadanje
	glEnable(GL_LIGHT1);
}

void CGLRenderer::SetLightModel()
{
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	float light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	float light_position[] = { 7.0, 10.0, 7.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	if (svetlo)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
}

void CGLRenderer::DrawCone(double dSide, double dHeight, int nSteps, int repX, int repY) {
	double pi = 3.1415926535897932384626433832795;
	double alpha = 2.0 * pi / (double)nSteps;
	double radius = dSide / 2.0 * sin(alpha / 2.0);
	double beta = atan(radius / dHeight);
	double stepTexX = repX / (double)nSteps;
	double texX = 0.0;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= nSteps; i++) {
		glNormal3d(0.0, -1.0, 0.0);
		glTexCoord2d(0.5, 0.5);
		glVertex3d(0.0, -dHeight / 2.0, 0.0);
		glTexCoord2d(cos(i*alpha) / 2.0 + 0.5, sin(i*alpha) / 2.0 + 0.5);
		glVertex3d(dSide*cos(i*alpha), -dHeight / 2.0, dSide*sin(i*alpha));
	}
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= nSteps; i++) {
		glNormal3d(cos(-i*alpha)*cos(beta), sin(beta), sin(-i*alpha)*cos(beta));//glTexCoord2d(texX, 0.0);
		glTexCoord2d(repX / 2.0, repY / 2.0);
		glVertex3d(0.0, dHeight / 2.0, 0.0);//glTexCoord2d(texX, repY);
		glTexCoord2d((cos(i*alpha) / 2.0 + 0.5)*repX, (sin(i*alpha) / 2.0 + 0.5)*repY);
		glVertex3d(dSide*cos(-i*alpha), -dHeight / 2.0, dSide*sin(-i*alpha));
		texX += stepTexX;
	}
	glEnd();
}