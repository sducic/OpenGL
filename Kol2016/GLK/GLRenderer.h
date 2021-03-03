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

	float alfa, beta, rotacija;
	bool svetlo;

	void DrawCube(double a);
	void DrawTube(double r1,double r2,double h,int n);
	void DrawCone(double r,double h,int n);
	void DrawPaddle(double length,double width);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT brick, env;
};
