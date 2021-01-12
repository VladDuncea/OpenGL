#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <time.h>       /* time */
#include <vector>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "SOIL.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "loadShaders.h"
#include "objloader.hpp"  


 

using namespace std;
 
//CONSTANTE
const float PI = 3.141592;
const float ERR = 0.00001;

// QUATERNIONI
class Quaternion {


public:
	double w, x, y, z;

	Quaternion(double angle, double x, double y, double z)
	{
		this->w = cos(angle / 2);
		this->x = x * sin(angle / 2);
		this->y = y * sin(angle / 2);
		this->z = z * sin(angle / 2);
	}

	Quaternion()
	{
		this->w = 1;
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	//Ducere quaternion la un quaternion normalizat
	void normalize()
	{
		double magnitudine = w * w + x * x + y * y + z * z;

		//Calculam normalizarea doar in cazul in care nu suntem deja normalizati
		if (abs(magnitudine - 1) > ERR)
		{
			magnitudine = sqrt(magnitudine);
			w /= magnitudine;
			x /= magnitudine;
			y /= magnitudine;
			z /= magnitudine;
		}
	}

	glm::mat4 matrice()
	{
		glm::mat4 mq;

		mq[0][0] = 1 - 2 * y * y - 2 * z * z;
		mq[0][1] = 2 * x * y - 2 * w * z;
		mq[0][2] = 2 * x * z + 2 * w * y;
		mq[0][3] = 0;

		mq[1][0] = 2 * x * y + 2 * w * z;
		mq[1][1] = 1 - 2 * x * x - 2 * z * z;
		mq[1][2] = 2 * y * z + 2 * w * x;
		mq[1][3] = 0;

		mq[2][0] = 2 * x * z - 2 * w * y;
		mq[2][1] = 2 * y * z - 2 * w * x;
		mq[2][2] = 1 - 2 * x * x - 2 * y * y;
		mq[2][3] = 0;

		mq[3][0] = 0;
		mq[3][1] = 0;
		mq[3][2] = 0;
		mq[3][3] = 1;

		return mq;
	}

	friend Quaternion operator*(Quaternion q1, Quaternion q2);

};



//////////////////////////////////////
// identificatori 
GLuint
  VaoId,
  VboId,
  EboId,
  ColorBufferId,
  ProgramId,
  myMatrixLocation,
  matrUmbraLocation,
  viewLocation,
  projLocation,
  matrRotlLocation,
  codColLocation,
  culoareCustomLocation,
  culoareLocation,
  depthLocation;

 GLuint texture;

int codCol, culoareCustom = 0;


//Time keeping
long long old_t; 

// matrice utilizate
glm::mat4 myMatrix, matrRot; 

// elemente pentru matricea de vizualizare
float Obsx=0.0, Obsy=-600.0, Obsz=250.f;
float Refx=0.0f, Refy=600.0f, Refz=0.0f;
float Vx=0.0, Vy=0.0, Vz=1.0;
glm::mat4 view;

// elemente pentru matricea de proiectie
float width=800, height=600, xwmin=-800.f, xwmax=800, ywmin=-600, ywmax=600, znear=0.1, zfar=1, fov=45;
glm::mat4 projection;
 
// sursa de lumina
float xL = 100.f, yL = 100.f, zL = 300.f;

// matricea umbrei
float matrUmbra[4][4];

//Variabila pt afisare piramida/trunchi
bool copac = true, culoriRandom = false, carusel = false;

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; // 

const int nr_copaci = 80;
int poz[nr_copaci * 2];

enum {
	Menu_Brad, 
	Menu_Copac, 
	Menu_Carusel, 
};

int rendermode;

//Rotatii
float rot_speed = PI / 10;
Quaternion total_rot(0, 0, 0, 1);


void menu(int selection)
{
	switch (selection) {

	case Menu_Brad:
		copac = false;
		total_rot = Quaternion();
		carusel = false;
		break;
	case Menu_Copac:
		copac = true;
		total_rot = Quaternion();
		carusel = false;
		break;
	case Menu_Carusel:
		carusel = true;
		break;
	};

	rendermode = selection;
	glutPostRedisplay();
}



Quaternion operator*(Quaternion q1, Quaternion q2)
{
	Quaternion qrez;

	qrez.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	qrez.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	qrez.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	qrez.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;

	return qrez;
}


float srot = 100;
void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key) {
		case 'q' :
			Vx -= 0.1;
			break;
		case 'e' :
			Vx += 0.1;
			break;
		case 'w' :
			if (carusel)
			{
				Quaternion partial_rot = Quaternion(PI/ srot, 1, 0, 0);
				total_rot = partial_rot * total_rot;
			}
			else
			{
				Obsy += 10;
				Refy += 10;
			}
			break;
		case 's' :
			if (carusel)
			{
				Quaternion partial_rot = Quaternion(-PI / srot, 1, 0, 0);
				total_rot = partial_rot * total_rot;
			}
			else
			{
				Obsy -= 10;
				Refy -= 10;
			}
			break;
		case 'd':
			if (carusel)
			{
				Quaternion partial_rot = Quaternion(+PI / srot, 0, 1, 0);
				total_rot = partial_rot * total_rot;
			}
			else
			{
				Obsx += 10;
				Refx += 10;
			}
			break;
		case 'a':
			if (carusel)
			{
				Quaternion partial_rot = Quaternion(-PI / srot, 0, 1, 0);
				total_rot = partial_rot * total_rot;
			}
			else
			{
				Obsx -= 10;
				Refx -= 10;
			}
			break;
		case 'f':
			culoriRandom = !culoriRandom;
			break;
	}
if (key == 27)
exit(0);
}
void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT :
			Obsx-=10;
			break;
		case GLUT_KEY_RIGHT :
			Obsx+=10;
			break;
		case GLUT_KEY_UP :
			Obsz+=10;
			break;
		case GLUT_KEY_DOWN :
			Obsz-=10;
			break;
	}
}

void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[] = {

		// coordonate                   // culori			// normale				//Coord texturare
		//Piramida
		-50.0f,  -50.0f, 50.0f, 1.0f,   1.0f, 0.5f, 0.4f,   -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
		50.0f,  -50.0f,  50.0f, 1.0f,   1.0f, 0.5f, 0.4f,   1.0f, -1.0f, -1.0f,		0.0f, 0.0f,
		50.0f,  50.0f,  50.0f, 1.0f,    1.0f, 0.5f, 0.4f,   1.0f, 1.0f, -1.0f,		0.0f, 0.0f,
		-50.0f,  50.0f, 50.0f, 1.0f,    1.0f, 0.5f, 0.4f,   -1.0f, 1.0f, -1.0f,		0.0f, 0.0f,
		0.0f,   0.0f, 150.0f, 1.0f,     1.0f, 0.5f, 0.4f,   0.0f, 0.0f, 1.0f,		0.0f, 0.0f,

		//Suprafata de baza 
	   -1000.0f,  -1000.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		1000.0f,  -1000.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f,		3.0f, 0.0f,
		1000.0f,  1000.0f,  0.0f, 1.0f,  1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f,		3.0f, 3.0f,
	   -1000.0f,  1000.0f,  0.0f, 1.0f,  1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f,		0.0f, 3.0f,

		//Trunchi de piramida
		-50.0f, -50.0f,	50.0f,	1.0f,   1.0f, 0.5f, 0.4f,   -1.0f,	-1.0f,	-1.0f,	0.0f, 0.0f,
		50.0f,  -50.0f, 50.0f,	1.0f,   1.0f, 0.5f, 0.4f,   1.0f,	-1.0f,	-1.0f,	0.0f, 0.0f,
		50.0f,  50.0f,  50.0f,	1.0f,   1.0f, 0.5f, 0.4f,   1.0f,	1.0f,	-1.0f,	0.0f, 0.0f,
		-50.0f, 50.0f,	50.0f,	1.0f,   1.0f, 0.5f, 0.4f,   -1.0f,	1.0f,	-1.0f,	0.0f, 0.0f,
		-20.0f, -20.0f,	150.0f, 1.0f,   1.0f, 0.5f, 0.4f,   -1.0f,	-1.0f,	1.0f,	0.0f, 0.0f,
		20.0f,	-20.0f,	150.0f, 1.0f,   1.0f, 0.5f, 0.4f,	1.0f,	-1.0f,	1.0f,	0.0f, 0.0f,
		20.0f,	20.0f,	150.0f, 1.0f,   1.0f, 0.5f, 0.4f,	1.0f,	1.0f,	1.0f,	0.0f, 0.0f,
		-20.0f,	20.0f,	150.0f, 1.0f,   1.0f, 0.5f, 0.4f,	-1.0f,	1.0f,	1.0f,	0.0f, 0.0f,

		// con
        -40.0f, -69.28f,	0.0f,	1.0f,		0.1f, 1.0f, 0.2f,	-0.353f, -0.612f, 0.707f,		0.0f, 0.0f,
        40.0f,	-69.28f,	0.0f,	1.0f,		0.1f, 1.0f, 0.2f,	0.353f,	-0.612f, 0.707f,		0.0f, 0.0f,
        80.0f,	0.0f,		0.0f,	1.0f,		0.1f, 1.0f, 0.2f,	0.707f,	0.0f,	 0.707f,		0.0f, 0.0f,
        40.0f,	69.28f,		0.0f,	1.0f,		0.1f, 1.0f, 0.2f,	40.0f,	69.28f,	 80.0f,		0.0f, 0.0f,
        -40.0f, 69.28f,		0.0f,	1.0f,		0.1f, 1.0f, 0.2f,	-40.0f, 69.28f, 80.0f,		0.0f, 0.0f,
        -80.0f, 0.0f,		0.0f,	1.0f,		0.1f, 1.0f, 0.2f,	-80.0f, 0.0f,   80.0f,		0.0f, 0.0f,
        0.0f,	0.0f,		100.0f, 1.0f,		0.3f, 1.0f, 0.2f,	0.0f,	0.0f,   1.0f,		0.0f, 0.0f,

		// varfuri cub
        -50.0f,  -50.0f, 50.0f, 1.0f,		0.54f, 0.27f,0.07f,		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
        50.0f,  -50.0f,  50.0f, 1.0f,		0.54f, 0.27f,0.07f,		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,
        50.0f,  50.0f,  50.0f, 1.0f,		0.54f, 0.27f,0.07f,		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,
        -50.0f,  50.0f, 50.0f, 1.0f,		0.54f, 0.27f,0.07f,		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,
        -50.0f,  -50.0f, 150.0f, 1.0f,		0.54f, 0.27f,0.07f,		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,
        50.0f,  -50.0f,  150.0f, 1.0f,		0.54f, 0.27f,0.07f,		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,
        50.0f,  50.0f,  150.0f, 1.0f,		0.54f, 0.27f,0.07f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
        -50.0f,  50.0f, 150.0f, 1.0f,		0.54f, 0.27f,0.07f,		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,

	};

	// indicii pentru varfuri
	GLubyte Indices[] = {
	  //0
	  2, 1, 0,   2, 0, 3,   
	  2, 3, 4,   3, 0, 4,
	  0, 1, 4,   1, 2, 4,

	  //18
	  5,6,7,    7,8,5,

	  //24
	  9,10,11,	11,12,9,
	  9,10,13,	10,14,13,
	  10,11,14,	11,15,10,
	  11,12,16,	11,16,15,
	  12,9,13,	12,13,16,
	  13,14,15,	15,16,13,

	  // fetele conului (70)
		17, 18, 23,		18, 19, 23,
		19, 20, 23,		20, 21, 23,
		21, 22, 23,		22, 17, 23,

		//fetele cubului(78)
		25, 26, 24,		26, 24, 27,
		26, 27, 30,		30, 27, 31,
		31, 27, 28,		28, 27, 24,
		28, 24, 29,		29, 24, 25,
		25, 26, 29,		29, 26, 30,
		29, 30, 28,		28, 30, 31,
	};

	// se creeaza un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	// se creeaza un buffer nou (atribute)
	glGenBuffers(1, &VboId);
	// se creeaza un buffer nou (indici)
	glGenBuffers(1, &EboId);

	// legarea VAO 
	glBindVertexArray(VaoId);

	// legarea buffer-ului "Array"
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// legarea buffer-ului "Element" (indicii)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	// indicii sunt "copiati" in bufferul curent
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)0);

	// se activeaza lucrul cu atribute; atributul 1 = culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

	// se activeaza lucrul cu atribute; atributul 2 = normale
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(10 * sizeof(GLfloat)));

}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &EboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void LoadTexture(void)
{

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image = SOIL_load_image("grass.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

}
 
void CreateShaders(void)
{
	ProgramId=LoadShaders("Tema_5_Duncea_Vlad.vert", "Tema_5_Duncea_Vlad.frag");
	glUseProgram(ProgramId);
}
 
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}
 
void Initialize(void)
{
	myMatrix = glm::mat4(1.0f);
	matrRot=glm::rotate(glm::mat4(1.0f), PI/8, glm::vec3(0.0, 0.0, 1.0));
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // culoarea de fond a ecranului
	CreateShaders();
	//bool res = loadOBJ("cube.obj", vertices, uvs, normals);
	CreateVBO();
	glEnable(GL_DEPTH_TEST);

	//Locatii
	myMatrixLocation = glGetUniformLocation(ProgramId, "modify");
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.0f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
	viewLocation = glGetUniformLocation(ProgramId, "view");
	projLocation = glGetUniformLocation(ProgramId, "projection");
	culoareCustomLocation = glGetUniformLocation(ProgramId, "culoareCustom");
	culoareLocation = glGetUniformLocation(ProgramId, "culoare");

	//Texturi
	LoadTexture();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	srand(time(NULL));
	//Copaci
	int limita = 160;
	for (int i = 0; i < 2*nr_copaci; i+=2)
	{
		poz[i] = 0;
		while (poz[i] > -limita && poz[i] < limita)
		{
			poz[i] = rand() % 1800 - 900;
		}
		
		poz[i+1] = rand() % 1800 - 900;
	}
	
}

void desenareBrad(float x, float y)
{
	//Obiecte
	codCol = 0;
	glUniform1i(codColLocation, codCol);

	float s = 1.0, poz = 50;
	//Con1
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, poz)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, (void*)(60));

	//Con2
	s = 0.8;
	poz = 100;
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, poz)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, (void*)(60));

	//Con3
	s = 0.6;
	poz = 140;
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x,y, poz)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, (void*)(60));

	//Trunchi
	s = 0.5;
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, -25)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	//Obiecte
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	//Revenire la modificare 0
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.0f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	
}

void desenareCopac(float x, float y)
{
	//Obiecte
	glUniform1i(codColLocation, 0);

	float s = 1.0, poz = 50;

	//Fortare verde
	glUniform1i(culoareCustomLocation, 1);
	glUniform3f(culoareLocation, 0.1f, 0.8f, 0.2f);
	if (culoriRandom)
	{
		glUniform1i(glGetUniformLocation(ProgramId, "random"), 1);
	}

	//Coroana
	s = 0.8;
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 50)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x+20, y+20, 60)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x-20, y-20, 60)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	s = 0.6;
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 100)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x+20, y-20, 60)) * glm::scale(glm::vec3(s, s, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));


	glUniform1i(glGetUniformLocation(ProgramId, "random"), 0);
	//Revenire la culoarea default
	glUniform1i(culoareCustomLocation, 0);
	//Trunchi
	s = 1.5;
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, -80)) * glm::scale(glm::vec3(0.4, 0.4, s));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	//Revenire
	glUniform1i(culoareCustomLocation, 0);

	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.0f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
}

void desenareDrum()
{
	float s;
	//Drum
	glUniform1i(culoareCustomLocation, 1);
	glUniform3f(culoareLocation, 0.2f, 0.2f, 0.2f);
	s = 20;
	myMatrix = total_rot.matrice() * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10)) * glm::scale(glm::vec3(3, s, 0.1));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	//Linii stanga-dreapta
	glUniform3f(culoareLocation, 1.f, 1.f, 1.0f);
	myMatrix = total_rot.matrice() * glm::translate(glm::mat4(1.0f), glm::vec3(-135, 0, -5)) * glm::scale(glm::vec3(0.1, s, 0.1));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));
	myMatrix = total_rot.matrice() * glm::translate(glm::mat4(1.0f), glm::vec3(+135, 0, -5)) * glm::scale(glm::vec3(0.1, s, 0.1));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	//Linii mijloc
	for (int i = -10; i <= 10; i++)
	{
		myMatrix = total_rot.matrice() * glm::translate(glm::mat4(1.0f), glm::vec3(0, i*100, -5)) * glm::scale(glm::vec3(0.1, 0.6, 0.1));
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));
	}

	glUniform1i(culoareCustomLocation, 0);
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.0f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
}

void desenareMasina(float y, int parte_drum=1)
{
	int x = -50;
	if (parte_drum == 2)
		x = 50;
	glUniform1i(culoareCustomLocation, 1);
	glUniform3f(culoareLocation, 0.8f, 0.2f, 0.2f);

	

	myMatrix = total_rot.matrice() * glm::translate(glm::mat4(1.0f), glm::vec3(x, y, -5))* glm::scale(glm::vec3(0.5, 1, 0.3));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));

	myMatrix = total_rot.matrice() * glm::translate(glm::mat4(1.0f), glm::vec3(x, y, -5)) * glm::scale(glm::vec3(0.4, 0.4, 0.5));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(78));


	glUniform1i(culoareCustomLocation, 0);
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.0f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
}


float vitezaMasini = 200;
int limita = 1000;
float poz_1 = -(rand() % limita), poz_2 = -(rand() % limita);

void masini()
{
	//Time update
	long long t = glutGet(GLUT_ELAPSED_TIME);
	float dt = (t - old_t) / 1000.0;
	old_t = t;

	//Miscare masini
	poz_1 += vitezaMasini * dt;
	poz_2 += vitezaMasini * dt;

	if (carusel)
	{
		//Avansare unghi
		Quaternion avans_rot = Quaternion(rot_speed * dt, 0, 0, 1);
		total_rot = avans_rot * total_rot;
		total_rot.normalize();
	}

	if (poz_1 > limita)
		poz_1 = -limita;
	if (poz_2 > limita)
		poz_2 = -limita;

	desenareMasina(-poz_1, 1);
	desenareMasina(poz_2, 2);
}

void RenderFunction(void)
{
  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
	
	// reperul de vizualizare
	glm::vec3 Obs = glm::vec3 (Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert =  glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
		 
	projection = glm::infinitePerspective(fov, GLfloat(width)/GLfloat(height), znear);
	// matricea pentru umbra
	float D = -4.f;
	matrUmbra[0][0] = zL+D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
	matrUmbra[1][0] = 0; matrUmbra[1][1] = zL+D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
	matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
	matrUmbra[3][0] = -D*xL; matrUmbra[3][1] = -D*yL; matrUmbra[3][2] = -D*zL; matrUmbra[3][3] = zL;
 
 
  
 
	// variabile uniforme pentru shaderul de varfuri
	
	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE,  &matrUmbra[0][0]);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE,  &view[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE,  &projection[0][0]);
 
	// Variabile uniforme pentru iluminare
	GLint objectColorLoc = glGetUniformLocation(ProgramId, "objectColor");
	GLint lightColorLoc  = glGetUniformLocation(ProgramId, "lightColor");
	GLint lightPosLoc    = glGetUniformLocation(ProgramId, "lightPos");
	GLint viewPosLoc     = glGetUniformLocation(ProgramId, "viewPos");
	GLint codColLocation = glGetUniformLocation(ProgramId, "codCol");
	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.4f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, xL, yL, zL);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);

	

	//Obiecte
	codCol = 0;
	glUniform1i(codColLocation, codCol);

	//Desenam copaci daca nu suntem in modul carusel
	for (int i = 0; (i < 2*nr_copaci) && !carusel; i+=2)
	{
		if (copac)
		{
			desenareCopac(poz[i], poz[i + 1]);
		}
		else
		{
			desenareBrad(poz[i], poz[i + 1]);
		}
	}

	desenareDrum();

	masini();

	glUniform1i(glGetUniformLocation(ProgramId, "random"), 0);

	//Randare instantiata
	//glUniform1i(glGetUniformLocation(ProgramId, "instanced"), 1);
	//glDrawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, 0, 4);
	//glUniform1i(glGetUniformLocation(ProgramId, "instanced"), 0);

	// variabile uniforme pentru shaderul de fragmente
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	//Plan
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)(18));


	glutSwapBuffers(); 
	glFlush ( );
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
 
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE|GLUT_MULTISAMPLE);
	glutInitWindowPosition (100,100); 
	glutInitWindowSize(1200,900); 
	glutCreateWindow("Tema 5 Duncea Vlad"); 
	glewInit(); 
	Initialize( );
	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
 
	glutCreateMenu(menu);
	glutAddMenuEntry("Brad", Menu_Brad);
	glutAddMenuEntry("Copac", Menu_Copac);
	glutAddMenuEntry("Carusel", Menu_Carusel);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutCloseFunc(Cleanup);
	glutMainLoop();
 
}


