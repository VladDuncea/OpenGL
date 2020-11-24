/* Teama 2 - Duncea Vlad Alexandru Grupa 344, Am folosit shaderul .frag din curs
* Shaderul .vert este modificat si a fost incarcat cu tema
*/

#include <windows.h> 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h> 
#include <GL/freeglut.h>
#include "loadShaders.h"
#include <vector>


#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

//////////////////////////////////////

using namespace std;

GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId,
defaultTranslateLocation,
defaultResizeLocation,
matrScaleLocation,
matrRotateLocation,
modifyLocation,
matrRotateFinalLocation,
matrRotTransl1Location,
matrRotTransl2Location,
rotateFinalLocation,
matrTranslLocation;

int modify, rotateFinal;
float PI = 3.141592;

//Time keeping
long long old_t; 

//Coordonate conform cerintei
const int width = 800, height = 1200;
const int w = width, h = height;
const int lineHeight = 100, lineWidth = 20;
const float lineSpace = 30;
const float speed = 200;
const int nrLines = h / lineHeight;
float linePozVect[h / lineHeight];

//Masina 1
int offsetXMasina1 = 150;
float timpMasina1 = 0,duratieMasina1 = 0.1, frecventaMasina1 = 2;
float animatieMasina1 = 0;

//Masina 2
float offsetYMasina2Init = -height-200, offsetXMasina2Init = 150;
float offsetXMasina2 = offsetXMasina2Init, offsetYMasina2 = offsetYMasina2Init;


//Animatie
int etapaCurenta = 1;
//ET1
float stopEtapa1 = -350,initialSpeedEtapa1=440, speedEtapa1 = initialSpeedEtapa1;
float accelYEtapa1 = -90;
//ET2
int nrFlash = 0, maxFlash = 2;
float timpFlash = 0,duratieFlashFaruri = 0.2, frecventaFlashFaruri = 0.2;
bool flash = false;
//ET3
int countEtapa3 = 0, duratieEtapa3 = 2, maxEtapa3 = 1;
float timpEtapa3 = 0;
//ET4
float stopXEtapa4 = -150, stopYEtapa4 = 100;
float speedXEtapa4 = 100, speedYEtapa4 = 0, maxSpeedYEtapa4 = 200, accelYEtapa4 = 50;
bool stanga = true;


glm::mat4 defaultResize, defaultTranslate, matrTransl, matrScale, matrRot, matrRotFinal;
glm::mat4 matrRotTransl1,matrRotTransl2;
glm::vec3 pctTrans, pctTransD;

#pragma region Semnalizator

class Semnalizator
{
private:
	static bool _semnal, _semnalAprins;
	static float _timpSemnal;
	static const float _duratieSemnal, _frecventaSemnal;
public:

	static void advance(float dt)
	{
		if (!_semnal)
			return;

		_timpSemnal += dt;

		if (_semnalAprins && _timpSemnal > _duratieSemnal)
		{
			_semnalAprins = false;
			_timpSemnal = 0;
		}
		else if (!_semnalAprins && _timpSemnal > _frecventaSemnal)
		{
			_semnalAprins = true;
			_timpSemnal = 0;
		}
	}

	static void semnal(bool val)
	{
		_semnal = val;
	}

	static bool semnalizeaza()
	{
		return _semnal && _semnalAprins;
	}

};
bool Semnalizator::_semnal = false;
bool Semnalizator::_semnalAprins = true;
float Semnalizator::_timpSemnal = 0;
const float Semnalizator::_duratieSemnal = 0.6f;
const float Semnalizator::_frecventaSemnal = 0.3f;

#pragma endregion

float centru_greutate(vector<float> points)
{
	float s = 0;
	int n = points.size();

	for (int i = 0; i < n; i++)
		s += points[i];

	return s / n;
}

void AdvanceLoop(void)
{
	//Time update
	long long t = glutGet(GLUT_ELAPSED_TIME);
	float dt = (t - old_t) / 1000.0;
	old_t = t;


	// Middle lines -------------------
	float maxHeight = 0;
	//find highest line
	for (int i = 0; i < nrLines; i++)
	{
		if (linePozVect[i] > maxHeight)
			maxHeight = linePozVect[i];
	}

	//Move middle lines
	for (int i = 0; i < nrLines; i++)
	{
		//Check if line needs to be moved up
		if (linePozVect[i] < -height / 2 - lineHeight)
		{
			linePozVect[i] = maxHeight + lineHeight + lineSpace;
			maxHeight = linePozVect[i];
		}

		//Move line
		linePozVect[i] -= speed*dt;
	}
	// End Middle Lines ----------------


	//Car2 ---------------------------

	//Miscare initiala
	if (etapaCurenta == 1)
	{
		if (speedEtapa1 > 0)
		{
			offsetYMasina2 += speedEtapa1 * dt;
			speedEtapa1 += accelYEtapa1 * dt;
		}
		else
		{
			etapaCurenta = 2;
			speedEtapa1 = initialSpeedEtapa1;
		}
	}
	//Flashuri
	else if (etapaCurenta == 2)
	{
		if (nrFlash >= maxFlash)
		{
			etapaCurenta = 3;
			nrFlash = 0;
			timpFlash = 0;
		}
		else
		{
			timpFlash += dt;
			if (!flash && timpFlash > frecventaFlashFaruri)
			{
				timpFlash = 0;
				flash = true;
			}
			else if (flash && timpFlash > duratieFlashFaruri)
			{
				timpFlash = 0;
				nrFlash++;
				flash = false;
			}
		}
	}
	//Delay intre flashuri si transfer la etapa 4
	else if (etapaCurenta == 3)
	{
		timpEtapa3 += dt;

		if (timpEtapa3 > duratieEtapa3)
		{
			timpEtapa3 = 0;
			countEtapa3 += 1;

			if (countEtapa3 > maxEtapa3)
			{
				etapaCurenta = 4;
				countEtapa3 = 0;
				Semnalizator::semnal(true);
			}
			else
			{
				etapaCurenta = 2;
			}
		}

	}
	//Depasire 
	else if (etapaCurenta == 4)
	{
		offsetYMasina2 += speedYEtapa4*dt;
		if (speedYEtapa4 < maxSpeedYEtapa4)
		{
			speedYEtapa4 += accelYEtapa4 * dt;
		}
		
		if (stanga)
		{
			offsetXMasina2 -= speedXEtapa4 * dt;
			
			if (offsetXMasina2 < stopXEtapa4)
			{
				offsetXMasina2 = stopXEtapa4;
				stanga = false;
				Semnalizator::semnal(false);
			}
		}
		else
		{
			if (offsetYMasina2 > stopYEtapa4)
			{
				Semnalizator::semnal(true);
				if (offsetXMasina2 < offsetXMasina2Init)
				{
					offsetXMasina2 += speedXEtapa4 * dt;
					
				}
				else
				{
					Semnalizator::semnal(false);
				}
					
			}
		}

		if (offsetYMasina2 > height)
		{
			etapaCurenta = 5;
			stanga = true;
			speedYEtapa4 = 0;
			Semnalizator::semnal(false);
		}

		//Procesare semnalizator
		Semnalizator::advance(dt);
	}
	else if (etapaCurenta == 5)
	{
		//resetare valori si reincepere animatie
		offsetYMasina2 = offsetYMasina2Init;
		offsetXMasina2 = offsetXMasina2Init;
		etapaCurenta = 1;
	}
	//End Car2 -------------------------
	
	glutPostRedisplay();
}

void CreateVBO(void)
{
	//Calcul linii drum
	float l = lineWidth;
	float s = (width - l)/2.0;
	float l2 = l;
	float h2 = lineHeight;

	// varfurile 
	GLfloat Vertices[] = {
		//Varfuri pt drum 0
		0.0f,	0.0f,	0.0f,	1.0f,
		w,		0.0f,	0.0f,	1.0f,
		w,		h,		0.0f,	1.0f,
		0.0f,	h,		0.0f,	1.0f,

		//Varfuri linie continua 4
		s,		0.0f,	0.0f,	1.0f,
		s+l,	0.0f,	0.0f,	1.0f,
		s+l,	h,		0.0f,	1.0f,
		s,		h,		0.0f,	1.0f,

		//Varfuri linie punctata 8
		(w-l2)/2,	(h - h2)/2.0,	0.0f,	1.0f,
		(w+l2)/2,	(h - h2)/2.0,	0.0f,	1.0f,
		(w+l2)/2,	(h + h2)/2.0,	0.0f,	1.0f,
		(w-l2)/2,	(h + h2)/2.0,	0.0f,	1.0f,

		//Patrat pentru masina 1 12
		(w-10)/2,	(h-10)/2,	0.0f,	1.0f,
		(w+10)/2,	(h-10)/2,	0.0f,	1.0f,
		(w+10)/2,	(h+10)/2,	0.0f,	1.0f,
		(w-10)/2,	(h+10)/2,	0.0f,	1.0f,

		//Patrat pentru masina 2 16
		(w - 10) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 10) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 10) / 2,	(h + 10) / 2,	0.0f,	1.0f,
		(w - 10) / 2,	(h + 10) / 2,	0.0f,	1.0f,

		//Trunchi lumina 20
		(w-10)/2,	(h)/2,	1.0f,	1.0f,
		(w+10)/2,	(h)/2,	1.0f,	1.0f,
		(w+150)/2,	(h+20)/2,	1.0f,	1.0f,
		(w-150)/2,	(h+20)/2,	1.0f,	1.0f,

		//Trunchi flash lumina 24
		(w - 10) / 2,	(h) / 2,	1.0f,	1.0f,
		(w + 10) / 2,	(h) / 2,	1.0f,	1.0f,
		(w + 300) / 2,	(h + 20) / 2,	1.0f,	1.0f,
		(w - 300) / 2,	(h + 20) / 2,	1.0f,	1.0f,

		//Semnal 28
		(w - 10) / 2,	(h - 20) / 2,	0.0f,	1.0f,
		(w + 10) / 2,	(h - 20) / 2,	0.0f,	1.0f,
		(w + 10) / 2,	(h + 20) / 2,	0.0f,	1.0f,
		(w - 10) / 2,	(h + 20) / 2,	0.0f,	1.0f,

		//Stop 32
		(w - 20) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 20) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 20) / 2,	(h + 10) / 2,	0.0f,	1.0f,
		(w - 20) / 2,	(h + 10) / 2,	0.0f,	1.0f,

		//Trunchi lumina stop 36
		(w - 2000) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 2000) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 20) / 2,	(h) / 2,	0.0f,	1.0f,
		(w - 20) / 2,	(h) / 2,	0.0f,	1.0f,

		//Trunchi lumina frana 40
		(w - 10000) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 10000) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 20) / 2,	(h) / 2,	0.0f,	1.0f,
		(w - 20) / 2,	(h) / 2,	0.0f,	1.0f,

		//Trunchi lumina semnal 44
		(w - 20) / 2,	(h - 2000) / 2,	0.0f,	1.0f,
		(w) / 2,		(h - 10) / 2,	0.0f,	1.0f,
		(w) / 2,		(h + 10) / 2,	0.0f,	1.0f,
		(w - 20) / 2,	(h + 2000) / 2,	0.0f,	1.0f,

		//Far 48
		(w - 20) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 20) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 20) / 2,	(h + 10) / 2,	0.0f,	1.0f,
		(w - 20) / 2,	(h + 10) / 2,	0.0f,	1.0f,
	};

	GLfloat Colors[] = {
		//Culori drum
		0.2f, 0.2f, 0.2f, 1.0f,
		0.2f, 0.2f, 0.2f, 1.0f,
		0.2f, 0.2f, 0.2f, 1.0f,
		0.2f, 0.2f, 0.2f, 1.0f,

		//Culori linie continua (totul alb)
		0.8f, 0.8f, 0.8f, 1.0f,
		0.8f, 0.8f, 0.8f, 1.0f,
		0.8f, 0.8f, 0.8f, 1.0f,
		0.8f, 0.8f, 0.8f, 1.0f,

		//Culori linie punctata (totul alb)
		0.8f, 0.8f, 0.8f, 1.0f,
		0.8f, 0.8f, 0.8f, 1.0f,
		0.8f, 0.8f, 0.8f, 1.0f,
		0.8f, 0.8f, 0.8f, 1.0f,

		//Culoare masina 1
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,

		//Culoare masina 2
		0.8f, 0.7f, 0.0f, 1.0f,
		0.8f, 0.7f, 0.0f, 1.0f,
		0.8f, 0.7f, 0.0f, 1.0f,
		0.8f, 0.7f, 0.0f, 1.0f,

		//Culoare lumina
		1.0f, 1.0f, 1.0f, 0.1f,
		1.0f, 1.0f, 1.0f, 0.1f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,

		//Culoare flash lumina
		1.0f, 1.0f, 1.0f, 0.3f,
		1.0f, 1.0f, 1.0f, 0.3f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,

		//Culoare semnal
		1.0f, 0.7f, 0.0f, 1.0f,
		1.0f, 0.7f, 0.0f, 1.0f,
		1.0f, 0.7f, 0.0f, 1.0f,
		1.0f, 0.7f, 0.0f, 1.0f,

		//Culoare stop
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		//Culoare lumina stop
		1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.03f,
		1.0f, 0.0f, 0.0f, 0.03f,

		//Culoare lumina frana
		1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.1f,
		1.0f, 0.0f, 0.0f, 0.1f,

		//Culoare lumina semnalizator
		1.0f, 0.7f, 0.0f, 0.0f,
		1.0f, 0.7f, 0.0f, 0.05f,
		1.0f, 0.7f, 0.0f, 0.05f,
		1.0f, 0.7f, 0.0f, 0.0f,

		//Culoare far
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};


	// se creeaza un buffer nou
	glGenBuffers(1, &VboId);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	// 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);


}

//Din curs
void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

//Din curs - modificat fisierul .vert
void DefaultShader(void)
{
	ProgramId = LoadShaders("Tema_3_Duncea_Vlad.vert", "04_01_Shader.frag");
	glUseProgram(ProgramId);
}

//Din curs
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

//Modificata
void Initialize(void)
{
	// Enable blending
	// Folosim blending pentru transparente
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//matrice pentru mutare din [0,width] x [0,height] in [-width/2,width/2] x [-height/2,height/2]
	defaultTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-width/2, -height/2, 0.0));
	// matricea pentru scalare la patratul standard
	defaultResize = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / (width/2), 1.f / (height/2), 1.0));


	//Compilare shader
	DefaultShader();

	//Locatii
	modifyLocation = glGetUniformLocation(ProgramId, "modify");
	defaultTranslateLocation = glGetUniformLocation(ProgramId, "defaultTranslate");
	defaultResizeLocation = glGetUniformLocation(ProgramId, "defaultResize");
	matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
	matrScaleLocation = glGetUniformLocation(ProgramId, "matrScale");
	matrRotateLocation = glGetUniformLocation(ProgramId, "matrRot");

	//Final rotations
	matrRotateFinalLocation = glGetUniformLocation(ProgramId, "matrRotFinal");
	matrRotTransl1Location = glGetUniformLocation(ProgramId, "matrRotTransl1");
	matrRotTransl2Location = glGetUniformLocation(ProgramId, "matrRotTransl2");
	rotateFinalLocation = glGetUniformLocation(ProgramId, "rotateFinal");
	

	//Init middle lines
	for (int i = 0; i < nrLines; i++)
	{
		linePozVect[i] = height - i * (lineHeight + lineSpace);
	}

	//Initial time
	old_t = glutGet(GLUT_ELAPSED_TIME);

	CreateVBO();
}

//Functii desenare
void drawCar1(void)
{
	modify = 2;
	glUniform1i(modifyLocation, modify);

	//Centru
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1, 0, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(10 - animatieMasina1, 10 - animatieMasina1, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 12, 4);

	//Bot
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1, 70, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8 - animatieMasina1, 12 - animatieMasina1, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 12, 4);

	//Spate
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1, -70, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 12, 4);


	//Far
	//far dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1 + 25, 125, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 48, 4);

	//far stanga
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1 - 25, 125, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 48, 4);

	
	//Stop
	//stop dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1 +30, -125, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 32, 4);

	//stop stanga
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1 -30, -125, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 32, 4);
}

void drawCar1Lights(void)
{
	//Desenare lumina masina 1
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1, 130, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8 - animatieMasina1, 60, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 20, 4);

	//lumina stop dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1 + 30, -130, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 200, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 36, 4);

	//lumina stop dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina1 - 30, -130, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 36, 4);
}

void drawCar2(void)
{
	//Desenare masina 2 ------------------------
	float pozYMasina2 = -350;

	//Actualizare matrice rotatie finala
	matrRotTransl1 = glm::translate(glm::mat4(1.0f), glm::vec3(-offsetXMasina2, -offsetYMasina2, 0.0));
	glUniformMatrix4fv(matrRotTransl1Location, 1, GL_FALSE, &matrRotTransl1[0][0]);
	matrRotTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, offsetYMasina2, 0.0));
	glUniformMatrix4fv(matrRotTransl2Location, 1, GL_FALSE, &matrRotTransl2[0][0]);

	// matricea pentru rotatie (cu  180 grade)
	float unghi = 0.2 * (1 - abs(offsetXMasina2 / offsetXMasina2Init));
	if (!stanga)
		unghi *= -1;
	matrRotFinal = glm::rotate(glm::mat4(1.0f), unghi, glm::vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(matrRotateFinalLocation, 1, GL_FALSE, &matrRotFinal[0][0]);

	rotateFinal = 1;
	glUniform1i(rotateFinalLocation, rotateFinal);

	modify = 2;
	glUniform1i(modifyLocation, modify);

	//Centru
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, offsetYMasina2, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 16, 4);

	//Bot
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, 70 + offsetYMasina2, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8, 12, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 16, 4);

	//Spate
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, -70 + offsetYMasina2, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8, 12, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 16, 4);

	//Semnal
	if (Semnalizator::semnalizeaza())
	{
		if (stanga)
		{
			//semnal fata
			matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 - 35, 120 + offsetYMasina2, 0.0));
			glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
			matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1.0));
			glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
			glDrawArrays(GL_POLYGON, 28, 4);

			//semnal spate
			matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 - 35, -120 + offsetYMasina2, 0.0));
			glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
			glDrawArrays(GL_POLYGON, 28, 4);
		}
		else
		{
			//semnal fata
			matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 + 35, 120 + offsetYMasina2, 0.0));
			glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
			matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1.0));
			glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
			glDrawArrays(GL_POLYGON, 28, 4);

			//semnal spate
			matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 + 35, -120 + offsetYMasina2, 0.0));
			glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
			glDrawArrays(GL_POLYGON, 28, 4);
		}
	}

	//Far
	//far dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 + 25, offsetYMasina2 + 125, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 48, 4);

	//far stanga
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 - 25, offsetYMasina2 + 125, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 48, 4);

	//Stop masina
	//stop dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 + 30, offsetYMasina2 - 125, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 32, 4);

	//stop stanga
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 - 30, offsetYMasina2 - 125, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 32, 4);

	rotateFinal = 0;
	glUniform1i(rotateFinalLocation, rotateFinal);
}

void drawCar2Lights(void)
{
	rotateFinal = 1;
	glUniform1i(rotateFinalLocation, rotateFinal);

	if (!flash)
	{
		//Desenare lumina masina 2
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, 130 + offsetYMasina2, 0.0));
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8, 60, 1.0));
		glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
		glDrawArrays(GL_POLYGON, 20, 4);
	}
	else
	{
		//Desenare flash lumina masina 2
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, 130 + offsetYMasina2, 0.0));
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8, 100, 1.0));
		glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
		glDrawArrays(GL_POLYGON, 24, 4);
	}


	//Semnal
	if (Semnalizator::semnalizeaza())
	{
		if (stanga)
		{
			//Desenare semnal sus
			matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 - 30, 125 + offsetYMasina2, 0.0));
			glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
			matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(60, 1, 1.0));
			glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
			glDrawArrays(GL_POLYGON, 44, 4);

			//Desenare semnal sus
			matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 - 30, -125 + offsetYMasina2, 0.0));
			glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
			glDrawArrays(GL_POLYGON, 44, 4);
		}
		else
		{
			modify = 3;
			glUniform1i(modifyLocation, modify);

			// matricea pentru rotatie (cu  180 grade)
			matrRot = glm::rotate(glm::mat4(1.0f), PI, glm::vec3(0.0, 0.0, 1.0));
			glUniformMatrix4fv(matrRotateLocation, 1, GL_FALSE, &matrRot[0][0]);

			//Desenare semnal sus
			matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 + 30, 125 + offsetYMasina2, 0.0));
			glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
			matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(60, 1, 1.0));
			glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
			glDrawArrays(GL_POLYGON, 44, 4);

			//Desenare semnal sus
			matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 + 30, -125 + offsetYMasina2, 0.0));
			glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
			glDrawArrays(GL_POLYGON, 44, 4);

			modify = 2;
			glUniform1i(modifyLocation, modify);
		}
	}


	int pozStop = 36;
	if (etapaCurenta == 1)
		pozStop += 4;

	//lumina stop dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 + 30, offsetYMasina2 - 130, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 200, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, pozStop, 4);

	//lumina stop dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2 - 30, offsetYMasina2 - 130, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, pozStop, 4);

	rotateFinal = 0;
	glUniform1i(rotateFinalLocation, rotateFinal);
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	

	//Initializare modify - pt 0 nu face decat sa aduca la [-1, 1],
	modify = 0;
	glUniform1i(modifyLocation, modify);

	//Nu aplica o rotatie la final
	rotateFinal = 0;
	glUniform1i(rotateFinalLocation, rotateFinal);

	// matricea de mutare default
	glUniformMatrix4fv(defaultTranslateLocation, 1, GL_FALSE, &defaultTranslate[0][0]);
	// matricea de redimensionare
	glUniformMatrix4fv(defaultResizeLocation, 1, GL_TRUE, &defaultResize[0][0]);

	// desenare drum
	glDrawArrays(GL_POLYGON, 0, 4);


	//Deseneaza linia punctata -------------------------------
	modify = 1;
	glUniform1i(modifyLocation, modify);
	float n = height / (lineHeight + lineSpace);
	for (int i = 0; i < nrLines; i++)
	{
		//matrice pentru mutarea liniei punctate
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(0, linePozVect[i], 0.0));
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);

		glDrawArrays(GL_POLYGON, 8, 4);
	}

	//Deseneaza liniile de pe margini ----------------------------
	int offsetLinieMargine = 300;

	//Linia stanga
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(-offsetLinieMargine, 0, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 4, 4);

	//Linia dreapta
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetLinieMargine, 0, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 4, 4);


	//Desenare masini
	drawCar1();
	drawCar2();

	//Desenare lumini masini
	drawCar1Lights();
	drawCar2Lights();

	//Folosim doua buffere
	glutSwapBuffers();
	glFlush();
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	float scale = 1;

	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(scale * width, scale * height);
	glutCreateWindow("Tema 3");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutIdleFunc(AdvanceLoop);
	glutMainLoop();
}

