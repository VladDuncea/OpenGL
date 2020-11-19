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
modifyLocation,
matrTranslLocation;

int modify;
float PI = 3.141592;

//Time keeping
long long old_t; 

//Coordonate conform cerintei
const int width = 800, height = 1200;
const int w = width, h = height;
const int lineHeight = 100;
const float lineSpace = 30;
const float speed = 200;
const int nrLines = h / lineHeight;
float linePozVect[h / lineHeight];

//Masina 1
float timpMasina1 = 0,duratieMasina1 = 0.1, frecventaMasina1 = 2;
float animatieMasina1 = 0;

//Masina 2
float offsetYMasina2Init = -height, offsetXMasina2Init = 150;
float offsetXMasina2 = offsetXMasina2Init, offsetYMasina2 = offsetYMasina2Init;


//Animatie
int etapaCurenta = 1;
//ET1
float stopEtapa1 = -350;
float speedEtapa1 = 100;
//ET2
int nrFlash = 0, maxFlash = 2;
float timpFlash = 0,duratieFlashFaruri = 0.5, frecventaFlashFaruri = 0.5;
bool flash = false;
//ET3
int countEtapa3 = 0, duratieEtapa3 = 2, maxEtapa3 = 1;
float timpEtapa3 = 0;
//ET4
float stopXEtapa4 = -150, stopYEtapa4 = 200;
float speedXEtapa4 = 100, speedYEtapa4 = 0, maxSpeedYEtapa4 = 200, accelYEtapa4 = 20;
bool stanga = true;


glm::mat4 defaultResize, defaultTranslate, matrTransl, matrScale;
glm::vec3 pctTrans, pctTransD;

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

	//Cars ---------------------------

	////Car1 -------
	//timpMasina1 += dt;
	//if (animatieMasina1 == 0 && timpMasina1 > frecventaMasina1)
	//{
	//	timpMasina1 = 0;
	//	animatieMasina1 = 0.1;
	//}
	//else if (animatieMasina1 != 0 && timpMasina1 > duratieMasina1)
	//{
	//	timpMasina1 = 0;
	//	animatieMasina1 = 0;
	//	frecventaMasina1 = (rand() % 3 + 1);
	//}
	////End Car1 ---

	//Car2 ---------------------------

	//Miscare initiala
	if (etapaCurenta == 1)
	{
		if (offsetYMasina2 < stopEtapa1)
		{
			offsetYMasina2 += speedEtapa1 * dt;
		}
		else
		{
			etapaCurenta = 2;
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
			}
		}
		else
		{
			if (offsetYMasina2 > stopYEtapa4)
			{
				offsetXMasina2 += speedXEtapa4 * dt;
			}
		}

		if (offsetYMasina2 > height*0.75)
		{
			etapaCurenta = 5;
			stanga = true;
			speedYEtapa4 = 0; 
		}
	}
	else if (etapaCurenta == 5)
	{
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
	float l = 20.0;
	float s = (width - l)/2.0;
	float l2 = l;
	float h2 = lineHeight;

	// varfurile 
	GLfloat Vertices[] = {
		//Varfuri pt drum
		0.0f,	0.0f,	0.0f,	1.0f,
		w,		0.0f,	0.0f,	1.0f,
		w,		h,		0.0f,	1.0f,
		0.0f,	h,		0.0f,	1.0f,

		//Varfuri linie continua
		s,		0.0f,	0.0f,	1.0f,
		s+l,	0.0f,	0.0f,	1.0f,
		s+l,	h,		0.0f,	1.0f,
		s,		h,		0.0f,	1.0f,

		//Varfuri linie punctata
		(w-l2)/2,	(h - h2)/2.0,	0.0f,	1.0f,
		(w+l2)/2,	(h - h2)/2.0,	0.0f,	1.0f,
		(w+l2)/2,	(h + h2)/2.0,	0.0f,	1.0f,
		(w-l2)/2,	(h + h2)/2.0,	0.0f,	1.0f,

		//Patrat pentru masina 1
		(w-10)/2,	(h-10)/2,	0.0f,	1.0f,
		(w+10)/2,	(h-10)/2,	0.0f,	1.0f,
		(w+10)/2,	(h+10)/2,	0.0f,	1.0f,
		(w-10)/2,	(h+10)/2,	0.0f,	1.0f,

		//Patrat pentru masina 2
		(w - 10) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 10) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 10) / 2,	(h + 10) / 2,	0.0f,	1.0f,
		(w - 10) / 2,	(h + 10) / 2,	0.0f,	1.0f,

		//Trunchi lumina
		(w-10)/2,	(h-10)/2,	0.0f,	1.0f,
		(w+10)/2,	(h-10)/2,	0.0f,	1.0f,
		(w+100)/2,	(h+10)/2,	0.0f,	1.0f,
		(w-100)/2,	(h+10)/2,	0.0f,	1.0f,

		//Trunchi flash lumina
		(w - 10) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 10) / 2,	(h - 10) / 2,	0.0f,	1.0f,
		(w + 180) / 2,	(h + 10) / 2,	0.0f,	1.0f,
		(w - 180) / 2,	(h + 10) / 2,	0.0f,	1.0f,
	};

	// culorile varfurilor din colturi
	GLfloat Colors[] = {
		//Culori drum
		0.23f, 0.24f, 0.27f, 1.0f,
		0.23f, 0.24f, 0.27f, 1.0f,
		0.43f, 0.44f, 0.47f, 1.0f,
		0.43f, 0.44f, 0.47f, 1.0f,

		//Culori linie continua (totul alb)
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		//Culori linie punctata (totul alb)
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		//Culoare masina 1
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		//Culoare masina 2
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		//Culoare lumina
		1.0f, 1.0f, 1.0f, 0.4f,
		1.0f, 1.0f, 1.0f, 0.4f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,

		//Culoare flash lumina
		1.0f, 1.0f, 1.0f, 0.6f,
		1.0f, 1.0f, 1.0f, 0.6f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
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

	// matricea pentru rotatie (cu  90 grade)
	//matrRot = glm::rotate(glm::mat4(1.0f), PI / 2.0f, glm::vec3(0.0, 0.0, 1.0));

	//Matrice scalare D
	//Scalare *2 pe Ox, *0.5 pe Oy
	//matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 0.5, 1.0));

	//Avand un fundal complet colorat nu are mare importanta aceasta linie
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului

	//Compilare shader
	DefaultShader();

	//Locatii
	modifyLocation = glGetUniformLocation(ProgramId, "modify");
	defaultTranslateLocation = glGetUniformLocation(ProgramId, "defaultTranslate");
	defaultResizeLocation = glGetUniformLocation(ProgramId, "defaultResize");
	matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
	matrScaleLocation = glGetUniformLocation(ProgramId, "matrScale");
	

	//Init middle lines
	for (int i = 0; i < nrLines; i++)
	{
		linePozVect[i] = height - i * (lineHeight + lineSpace);
	}

	//Initial time
	old_t = glutGet(GLUT_ELAPSED_TIME);
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	CreateVBO();

	//Initializare modify - pt 0 nu face decat sa aduca la [-1, 1],
	modify = 0;
	glUniform1i(modifyLocation, modify);

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


	//Desenare masina 1 ------------------------
	int offsetMasina1 = 150;

	modify = 2;
	glUniform1i(modifyLocation, modify);

	//Centru
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetMasina1, 0, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(10 - animatieMasina1, 10 - animatieMasina1, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 12, 4);

	//Desenare lumina masina 1
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetMasina1, 350, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8 - animatieMasina1, 45, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 20, 4);

	//Bot
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetMasina1, 70, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8 - animatieMasina1, 12 - animatieMasina1, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 12, 4);

	//Spate
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetMasina1, -70, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 12, 4);


	//Desenare masina 2 ------------------------
	int offsetMasina2 = 150;
	float pozYMasina2 = -350;

	modify = 2;
	glUniform1i(modifyLocation, modify);

	//Centru
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, offsetYMasina2, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 16, 4);

	if (!flash)
	{
		//Desenare lumina masina 2
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, 350 + offsetYMasina2, 0.0));
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8, 45, 1.0));
		glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
		glDrawArrays(GL_POLYGON, 20, 4);
	}
	else
	{
		//Desenare flash lumina masina 2
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, 450 + offsetYMasina2, 0.0));
		glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
		matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8, 65, 1.0));
		glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
		glDrawArrays(GL_POLYGON, 24, 4);
	}
	
	//Bot
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, 70 + offsetYMasina2, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(8, 12, 1.0));
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	glDrawArrays(GL_POLYGON, 16, 4);

	//Spate
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(offsetXMasina2, -70 + offsetYMasina2, 0.0));
	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
	glDrawArrays(GL_POLYGON, 16, 4);

	

	////Calcul aici(in program)
	//matrTransl = matrTransl2 * matrRot * matrTransl1;
	//matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
	//glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);

	//glDrawArrays(GL_POLYGON, 8, 5);
	//glDrawArrays(GL_TRIANGLES, 13, 9);

	////Translatare si Scalare D in shader
	//modify = 2;
	//glUniform1i(modifyLocation, modify);

	//matrScaleLocation = glGetUniformLocation(ProgramId, "matrScale");
	//matrTranslD1Location = glGetUniformLocation(ProgramId, "matrTranslD1");
	//matrTranslD2Location = glGetUniformLocation(ProgramId, "matrTranslD2");
	//glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	//glUniformMatrix4fv(matrTranslD1Location, 1, GL_FALSE, &matrTranslD1[0][0]);
	//glUniformMatrix4fv(matrTranslD2Location, 1, GL_FALSE, &matrTranslD2[0][0]);

	//glDrawArrays(GL_POLYGON, 4, 4);

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
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(scale * width, scale * height);
	glutCreateWindow("Tema 2");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutIdleFunc(AdvanceLoop);
	glutMainLoop();
}

