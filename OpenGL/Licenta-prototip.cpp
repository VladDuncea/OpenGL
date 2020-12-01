 /* FUNCTII DE DESENARE
 Elemente de noutate:
	- folosirea indecsilor: elemente asociate (matrice, buffer)
	- cele 4 functii de desenare (glDrawArrays, glDrawElements, glDrawElementsBaseVertex, glDrawArraysInstanced) */
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h

#include "loadShaders.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace std;
  
//////////////////////////////////////

GLuint
	VaoId,
	VboId,
	EboId,
	ColorBufferId,
	ProgramId,
	codColLocation,
	defaultTranslateLocation,
	defaultResizeLocation,
	viewLocation,
	projectionLocation,
	modifyLocation;
 

glm::mat4 defaultResize, defaultTranslate, view, projection;

int codCol, modify = 0;
float PI=3.141592, angle=0;
float tx=0; float ty=0; 
float width=200, height=200;
float i=0.0, alpha=0.0, beta=0.2;



void CreateVBO(void)
{
 
	// coordonatele varfurilor
	static const GLfloat vf_pos[] =
	{
		50,50,0,1.0f,
		100,50,0,1.0f,
		100,100,0,1.0f,
		50,100,0,1.0f,
	};  
	// culorile varfurilor
	static const GLfloat vf_col[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
	};

	// indici pentru trasarea unui triunghi
	static const GLuint vf_ind[] =
	{
		1,2,3,
	};
 

	// se creeaza un buffer nou pentru varfuri
	glGenBuffers(1, &VboId);
	// buffer pentru indici
	glGenBuffers(1, &EboId);
	// se creeaza / se leaga un VAO (Vertex Array Object)
	glGenVertexArrays(1, &VaoId);


	// legare VAO
	glBindVertexArray(VaoId);
 
	// buffer-ul este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);

	// buffer-ul va contine atat coordonatele varfurilor, cat si datele de culoare
	glBufferData(GL_ARRAY_BUFFER, sizeof(vf_col)+sizeof(vf_pos), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vf_pos), vf_pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vf_pos), sizeof(vf_col),vf_col);
 
	// buffer-ul pentru indici
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vf_ind), vf_ind, GL_STATIC_DRAW);
 
	// se activeaza lucrul cu atribute; atributul 0 = pozitie, atributul 1 = culoare, acestea sunt indicate corect in VBO
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vf_pos));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

 
}

void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EboId);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
	ProgramId=LoadShaders("Licenta_prototip.vert", "08_01_Shader.frag");
	glUseProgram(ProgramId);
}
 
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}
 
void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului

	float camX = 0, camY = 0;
	//Pozitionarea camerei
	glm::vec3 cameraPos = glm::vec3(camX, camY, 1.0f);
	glm::vec3 cameraTarget = glm::vec3(camX, camY, 0.0f);
	view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

	//Perspectiva
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 10.0f);
	glm::mat4 projection = glm::ortho();

	//matrice pentru mutare din [0,width] x [0,height] in [-width/2,width/2] x [-height/2,height/2]
	defaultTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-width / 2, -height / 2, 0.0));
	// matricea pentru scalare la patratul standard
	defaultResize = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / (width / 2), 1.f / (height / 2), 1.0));

	//Compilare shader
	CreateShaders();

	//Locatii variabile shader
	modifyLocation = glGetUniformLocation(ProgramId, "modify");
	defaultTranslateLocation = glGetUniformLocation(ProgramId, "defaultTranslate");
	defaultResizeLocation = glGetUniformLocation(ProgramId, "defaultResize");
	viewLocation = glGetUniformLocation(ProgramId, "view");
	projectionLocation = glGetUniformLocation(ProgramId, "projection");

	//Transmitere matrice unice
	// matricea de mutare default
	glUniformMatrix4fv(defaultTranslateLocation, 1, GL_FALSE, &defaultTranslate[0][0]);
	// matricea de redimensionare
	glUniformMatrix4fv(defaultResizeLocation, 1, GL_FALSE, &defaultResize[0][0]);
	// camera
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	// proiectia
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &view[0][0]);

	CreateVBO();
}

void RenderFunction(void)
{

	//Initializare modify - pt 0 nu face decat sa aduca la [-1, 1],
	modify = 0;
	glUniform1i(modifyLocation, modify);

	glClear(GL_COLOR_BUFFER_BIT);


	glDrawArrays(GL_POLYGON, 0, 4);
 
//  // Desenare cu glDrawArrays
//	  // Matricea de redimensionare
//	  myMatrix=resizeMatrix;  
//	  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
//	  
//
//  // Utilizarea indicilor
//	  // Matricea: redimensionare+translatie
//	  myMatrix=resizeMatrix*glm::translate(glm::mat4(1.0f), glm::vec3(-20.0, -20.0, 0.0));  
//	  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
//	  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
//	 
//// Utilizarea indicilor
//     // Matricea: redimensionare+ alta translatie (pe verticala)
//	  myMatrix=resizeMatrix*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 40.0, 0.0));  
//	  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
//	  glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL, 1);
//
//// Randare instantiata
//	// Matricea: redimensionare+ alta translatie (pe orizontala)
//	  myMatrix=resizeMatrix*glm::translate(glm::mat4(1.0f), glm::vec3(70.0, 0.0, 0.0));  
//	  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
//	  glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

	  ////Task1
	  //myMatrix = resizeMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(20.0, 20.0, 0.0));
	  //glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	  //glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, (void*)(3));

	glFlush();
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	float scale = 3;

	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowPosition (100,100); 
	glutInitWindowSize(scale * width, scale * height);
	glutCreateWindow("Proof of concept Duncea"); 
	glewInit(); 
	Initialize( );
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

