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
	void DrawPatch(double R,int n);
	void inverseTSC(double x,double y,double& phi,double& theta);
	void DrawEarth(double R,int tes);
	void DrawMoon(double R, int tes);
	void DrawSpace(double R, int tes);

	void DrawTeksture(float dSize, float nSteps);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT T[6];	//zemlja
	UINT S[6];	//svemir
	UINT M[6];	//mesec
public:
	float daljina;
	float alfa;
	float beta;
	bool svetlo;
	float rotacija;

};
