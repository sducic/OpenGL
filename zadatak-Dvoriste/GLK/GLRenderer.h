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
	void DrawGrid(float size,float steps);
	void DrawQuad(float size);
	void DrawQuadKuca(float size);
	void DrawKuca(float size);
	void DrawValjak(float r, float h, int n);
	void DrawKupa();
	void DrawDrvo();
	void DrawSuma();

	void DrawCone(double dSide, double dHeight, int nSteps, int repX, int repY);

	void DrawPiramida();
	float toRad(float angle);

	void SetLampLight();
	void SetLightModel();

	float alfa, beta, daljina;
	bool svetlo;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT trava, kuca, krov,drvo,lisce;

};
