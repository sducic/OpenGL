#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	daljina = -5;
	alfa = 15;
	beta = 20;
	svetlo = false;
	ugao = 0;
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
	glClearColor(0.0,0.0,0.0,1.0);
	glEnable(GL_DEPTH_TEST);

	mleko = LoadTexture("Mleko.bmp");

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
	//--------------------------------------------
	glTranslatef(0,0,daljina);
	glRotatef(alfa,1,0,0);
	glRotatef(beta,0,1,0);

	glDisable(GL_LIGHTING);
	if (svetlo)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	GLfloat pozicija[] = {0.0,0.0,1.0,0.0};

	glBindTexture(GL_TEXTURE_2D,mleko);
	
	glPushMatrix();
	
	glLightfv(GL_LIGHT0, GL_POSITION, pozicija);
	
	glRotatef(180,1,0,0);
	glRotatef(ugao, 0, 1, 0);
	DrawTetrapak(1.4,2.8,0.8);
	
	glPopMatrix();

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
	// -----------------------------
	glDeleteTextures(1,&mleko);

	//------------------------------
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

void CGLRenderer::DrawTetrapak(float a, float b, float c)
{
	double vert[72];
	double norm[72];
	double text[48];
	

	vert[0] = -a / 2;	vert[1] = -b / 2;	vert[2] = c / 2;		//0
	vert[3] = a / 2;	vert[4] = -b / 2;	vert[5] = c / 2;		//1
	vert[6] = a / 2;	vert[7] = b / 2;	vert[8] = c / 2;		//2
	vert[9] = -a / 2;	vert[10] = b / 2;	vert[11] = c / 2;		//3	
	norm[0] = 0.0;		norm[1] = 0.0;		norm[2] = 1.0;
	norm[3] = 0.0;		norm[4] = 0.0;		norm[5] = 1.0;
	norm[6] = 0.0;		norm[7] = 0.0;		norm[8] = 1.0;
	norm[9] = 0.0;		norm[10] = 0.0;		norm[11] = 1.0;

	vert[12] = a / 2;	vert[13] = -b / 2;	vert[14] = c / 2;		//1
	vert[15] = a / 2;	vert[16] = -b / 2;	vert[17] = -c / 2;		//5
	vert[18] = a / 2;	vert[19] = b / 2;	vert[20] = -c / 2;		//6
	vert[21] = a / 2;	vert[22] = b / 2;	vert[23] = c / 2;		//2
	norm[12] = 1.0;		norm[13] = 0.0;		norm[14] = 0.0;
	norm[15] = 1.0;		norm[16] = 0.0;		norm[17] = 0.0;
	norm[18] = 1.0;		norm[19] = 0.0;		norm[20] = 0.0;
	norm[21] = 1.0;		norm[22] = 0.0;		norm[23] = 0.0;

	vert[24] = -a / 2;	vert[25] = b / 2;	vert[26] = -c / 2;		//7
	vert[27] = a / 2;	vert[28] = b / 2;	vert[29] = -c / 2;		//6
	vert[30] = a / 2;	vert[31] = -b / 2;	vert[32] = -c / 2;		//5
	vert[33] = -a / 2;	vert[34] = -b / 2;	vert[35] = -c / 2;		//4
	norm[24] = 0.0;		norm[25] = 0.0;		norm[26] = -1.0;
	norm[27] = 0.0;		norm[28] = 0.0;		norm[29] = -1.0;
	norm[30] = 0.0;		norm[31] = 0.0;		norm[32] = -1.0;
	norm[33] = 0.0;		norm[34] = 0.0;		norm[35] = -1.0;

	vert[36] = -a / 2;	vert[37] = -b / 2;	vert[38] = c / 2;		//0
	vert[39] = -a / 2;	vert[40] = b / 2;	vert[41] = c / 2;		//3
	vert[42] = -a / 2;	vert[43] = b / 2;	vert[44] = -c / 2;		//7
	vert[45] = -a / 2;	vert[46] = -b / 2;	vert[47] = -c / 2;		//4
	norm[36] = -1.0;	norm[37] = 0.0;		norm[38] = 0.0;
	norm[39] = -1.0;	norm[40] = 0.0;		norm[41] = 0.0;
	norm[42] = -1.0;	norm[43] = 0.0;		norm[44] = 0.0;
	norm[45] = -1.0;	norm[46] = 0.0;		norm[47] = 0.0;

	vert[48] = -a / 2;	vert[49] = b / 2;	vert[50] = -c / 2;		//7
	vert[51] = -a / 2;	vert[52] = b / 2;	vert[53] = c / 2;		//3
	vert[54] = a / 2;	vert[55] = b / 2;	vert[56] = c / 2;		//2
	vert[57] = a / 2;	vert[58] = b / 2;	vert[59] = -c / 2;		//6
	norm[48] = 0.0;		norm[49] = 1.0;		norm[50] = 0.0;
	norm[51] = 0.0;		norm[52] = 1.0;		norm[53] = 0.0;
	norm[54] = 0.0;		norm[55] = 1.0;		norm[56] = 0.0;
	norm[57] = 0.0;		norm[58] = 1.0;		norm[59] = 0.0;

	vert[60] = -a / 2;	vert[61] = -b / 2;	vert[62] = c / 2;		//0
	vert[63] = -a / 2;	vert[64] = -b / 2;	vert[65] = -c / 2;		//4
	vert[66] = a / 2;	vert[67] = -b / 2;	vert[68] = -c / 2;		//5
	vert[69] = a / 2;	vert[70] = -b / 2;	vert[71] = c / 2;		//1
	norm[60] = 0.0;		norm[61] = -1.0;	norm[62] = 0.0;
	norm[63] = 0.0;		norm[64] = -1.0;	norm[65] = 0.0;
	norm[66] = 0.0;		norm[67] = -1.0;	norm[68] = 0.0;
	norm[69] = 0.0;		norm[70] = -1.0;	norm[71] = 0.0;

	text[0] = 0.5;	text[1] = 0.25;
	text[2] = 1.0;	text[3] = 0.25;
	text[4] = 1.0;	text[5] = 1.0;
	text[6] = 0.5;	text[7] = 1.0;

	text[8] = 0.5;	text[9] = 0.25;
	text[10] = 1.0;	text[11] = 0.25;
	text[12] = 1.0;	text[13] = 1.0;
	text[14] = 0.5;	text[15] = 1.0;

	text[16] = 1.0;	text[17] = 1.0;
	text[18] = 0.5;	text[19] = 1.0;
	text[20] = 0.5;	text[21] = 0.25;
	text[22] = 1.0;	text[23] = 0.25;

	text[24] = 1.0;	text[25] = 0.25;
	text[26] = 1.0;	text[27] = 1.0;
	text[28] = 0.5;	text[29] = 1.0;
	text[30] = 0.5;	text[31] = 0.25;
	

	text[32] = 0.0;	text[33] = 0.0;
	text[34] = 0.0;	text[35] = 0.25;
	text[36] = 0.5;	text[37] = 0.25;
	text[38] = 0.5;	text[39] = 0.0;

	text[40] = 0.5;	text[41] = 0.0;
	text[42] = 0.5;	text[43] = 0.25;
	text[44] = 1.0;	text[45] = 0.25;
	text[46] = 1.0;	text[47] =0.0;

	glVertexPointer(3,GL_DOUBLE,0,vert);
	glNormalPointer(GL_DOUBLE,0,norm);
	glTexCoordPointer(2, GL_DOUBLE, 0, text);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS,0,24);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}
