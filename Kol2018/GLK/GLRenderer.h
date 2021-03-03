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
	void DrawSpace(float a);
	void DrawTriangle(float d1,float d2);
	void DrawShip();

	void DrawKvadrat(float size);

	float alfa, beta;
	float daljina;
	bool svetlo;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	UINT Ship;
	UINT Space[6];


};
