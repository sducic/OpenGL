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

	float alfa, beta;
	bool svetlo;

	void DrawBody(double r, double h1, double h2,int n);

	void DrawFigura(double r1, double r2, double h,int n);

	void DrawWing();
	void DrawWings();

	void DrawAxes();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT slika;
};
