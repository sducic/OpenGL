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
	void DrawKocka(double size);
	void Abazur(double r1,double r2,double h,int step);
	void Telo(double r1,double r2,double r3,double h,int n);

	void DrawValjak(float r, float h, int n);

	void DrawSoba(float size);

	float daljina, alfa, beta;
	bool svetlo;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT kocka,sobaTex,lampaTex,metal;

};
