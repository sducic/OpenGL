#pragma once
#include"DImage.h"
class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	UINT LoadTexture(char* fileName);

	UINT e1, e2, e3, e4;
	float alfa, beta;
	bool svetlo;
	float rotacija;
	float metal;

	void Zarubljena(float r1,float r2,float h,int n);

	void DrawFPyramid(float a,float b,float h);
	void DrawStranicu(float a, float b, float h);

	void DrawStranicu1(float a, float b, float h);			//prepravka zbog teksture najviseg dela

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
