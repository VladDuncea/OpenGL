/* Functii de desenare in Open GL. Randare instantiata

*/
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
// identificatori 
GLuint
  VaoId,
  VBPos,
  VBCol,
  VBModelMat,
  EboId,
  ColorBufferId,
  ProgramId,
  myMatrixLocation,
  viewLocation,
  projLocation,
  matrRotlLocation,
  codColLocation,
  depthLocation;

 GLuint texture;
int color_loc;    
int codCol;
float PI=3.141592;
#define INSTANCE_COUNT 40


// matrice utilizate
glm::mat4 myMatrix, matrRot; 

// elemente pentru matricea de vizualizare
float Obsx=0.0, Obsy=0.0, Obsz=-800.f;
float Refx=0.0f, Refy=0.0f;
float Vx=0.0;
glm::mat4 view;

// elemente pentru matricea de proiectie
float width=800, height=600, znear=1, zfar=-5, fovdeg=90;
glm::mat4 projection;
 
void displayMatrix ( )
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
		cout <<  myMatrix[ii][jj] << "  " ;
		cout << endl;
	};

};
void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key) {
		case 'l' :
			Vx += 0.1;
			break;
		case 'r' :
			Vx -= 0.1;
			break;
		case '+' :
			//znear += 10;
			//zfar += 10;
			Obsz+=10;
			break;
		case '-' :
			//znear -= 10;
			//zfar -= 10;
			Obsz-=10;
			break;

	}
if (key == 27)
exit(0);
}
void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT :
			Obsx-=20;
			break;
		case GLUT_KEY_RIGHT :
			Obsx+=20;
			break;
		case GLUT_KEY_UP :
			Obsy+=20;
			break;
		case GLUT_KEY_DOWN :
			Obsy-=20;
			break;
	}
}
void CreateVBO(void)
{
  // Varfurile 
  GLfloat Vertices[] = {
	
	 // punctele din planul z=-50   coordonate                   		
     -50.0f,  -50.0f, -50.0f, 1.0f, 	
     50.0f,  -50.0f,  -50.0f, 1.0f,  
	 50.0f,  50.0f,  -50.0f, 1.0f,  
	 -50.0f,  50.0f, -50.0f, 1.0f, 
	 // punctele din planul z=+50  coordonate                   		
     -50.0f,  -50.0f, 50.0f, 1.0f, 	
     50.0f,  -50.0f,  50.0f, 1.0f,   	
	 50.0f,  50.0f,  50.0f, 1.0f,   
	 -50.0f,  50.0f, 50.0f, 1.0f,  
 
  };
 // Culorile instantelor
   glm::vec4 Colors[INSTANCE_COUNT];

    for (int n = 0; n < INSTANCE_COUNT; n++)
    {
        float a = float(n) / 4.0f;
        float b = float(n) / 5.0f;
        float c = float(n) / 6.0f;

        Colors[n][0] = 0.35f + 0.3f * (sinf(a + 2.0f) + 1.0f);
        Colors[n][1] = 0.25f + 0.25f * (sinf(b + 3.0f) + 1.0f);
        Colors[n][2] = 0.25f + 0.35f * (sinf(c + 4.0f) + 1.0f);
        Colors[n][3] = 1.0f;
    }
	
	// Matricele instantelor
	glm:: mat4 MatModel[INSTANCE_COUNT];

	for (int n = 0; n < INSTANCE_COUNT; n++)
		{
			MatModel[n]= glm::translate(glm::mat4(1.0f), glm::vec3(80*n*sin(10.f*n*180/PI), 80*n*cos(10.f*n*180/PI), 0.0))*glm::rotate(glm::mat4(1.0f), n*PI/8, glm::vec3(n, 2*n*n, n/3));
		}
 
  // indicii pentru varfuri
  GLubyte Indices[ ]={

	1, 2, 0,   0, 2, 3,  //  Fata "de jos"
	2, 3, 6,   6, 3, 7,  
	7, 3, 4,   4, 3, 0,
	4, 0, 5,   5, 0, 1,
	1, 2, 5,   5, 2, 6,
	5, 6, 4,   4, 6, 7, //  Fata "de sus"
	0, 1, 2, 3,  // Contur fata de jos
	4, 5, 6, 7,  // Contur fata de sus
	0, 4,
	1, 5,
	2, 6, 
	3, 7

  };

  // se creeaza un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
  glGenVertexArrays(1, &VaoId);
  // se creeaza cate un buffer nou (atribute)
  glGenBuffers(1, &VBPos);
  glGenBuffers(1, &VBCol);
  glGenBuffers(1, &VBModelMat);
  // se creeaza un buffer nou (indici)
   glGenBuffers(1, &EboId);

  // legarea VAO 
  glBindVertexArray(VaoId);

  // 0: Pozitie
  glBindBuffer(GL_ARRAY_BUFFER, VBPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),(GLvoid*)0);

  // 1=color_loc: Culoare
  glBindBuffer(GL_ARRAY_BUFFER, VBCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4),(GLvoid*)0);
  glEnableVertexAttribArray(color_loc);
  glVertexAttribDivisor(color_loc, 1);

  // 3..6 (3+i): Matrice de pozitie
  glBindBuffer(GL_ARRAY_BUFFER, VBModelMat);
  glBufferData(GL_ARRAY_BUFFER, sizeof(MatModel), MatModel, GL_DYNAMIC_DRAW);
  // Pentru fiecare coloana
    for (int i = 0; i < 4; i++)
    {
        // Set up the vertex attribute
        glVertexAttribPointer(3 + i,              // Location
                              4, GL_FLOAT, GL_FALSE,       // vec4
                              sizeof(glm::mat4),                // Stride
                              (void *)(sizeof(glm::vec4) * i)); // Start offset
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);
    }

  // Indicii 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}
void DestroyVBO(void)
{
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
 
  glDeleteBuffers(1, &VBPos);
  glDeleteBuffers(1, &VBCol);
  glDeleteBuffers(1, &VBModelMat);
  glDeleteBuffers(1, &EboId);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VaoId);
   
}
void CreateShaders(void)
{
  ProgramId=LoadShaders("08_02_Shader.vert", "08_02_Shader.frag");
  glUseProgram(ProgramId);
} 
void DestroyShaders(void)
{
  glDeleteProgram(ProgramId);
}
void Initialize(void)
{

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
	CreateShaders();
	
}
void RenderFunction(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   // viewMatrix
	glm::vec3 Obs = glm::vec3 (Obsx, Obsy, Obsz); 
	Refx=Obsx; Refy=Obsy;
	glm::vec3 PctRef = glm::vec3(Refx, Refy, 800.0f); 
	glm::vec3 Vert =  glm::vec3(Vx, 1.0f, 0.0f);
  	view = glm::lookAt(Obs, PctRef, Vert);
	// projectionMatrix
    projection = glm::perspective(fovdeg*PI/180, GLfloat(width)/GLfloat(height), znear, zfar);
   
  CreateVBO();
  
 // variabile uniforme pentru shaderul de varfuri
  viewLocation = glGetUniformLocation(ProgramId, "viewMatrix");
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE,  &view[0][0]);
  projLocation = glGetUniformLocation(ProgramId, "projectionMatrix");
  glUniformMatrix4fv(projLocation, 1, GL_FALSE,  &projection[0][0]);
 
  GLint depthNearLoc = glGetUniformLocation(ProgramId, "gl_DepthRange.near");
  GLint depthFarLoc = glGetUniformLocation(ProgramId, "gl_DepthRange.far");
  GLint depthDiffLoc = glGetUniformLocation(ProgramId, "gl_DepthRange.diff");

  glUniform1f (depthNearLoc, -1.0);
  glUniform1f (depthFarLoc , 2.0);
  glUniform1f (depthDiffLoc, 1.0);

  codColLocation=glGetUniformLocation(ProgramId, "codCol");
  color_loc  = glGetAttribLocation(ProgramId, "in_Color");

  // Fetele
  codCol=0;
  glUniform1i(codColLocation, codCol);
  glDrawElementsInstanced(GL_TRIANGLES, 36,  GL_UNSIGNED_BYTE, 0, INSTANCE_COUNT);
  // Muchiile
  codCol=1;
  glUniform1i(codColLocation, codCol);
  glLineWidth(2.5);
  glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(36), INSTANCE_COUNT);
  glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(40), INSTANCE_COUNT);
  glDrawElementsInstanced(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(44), INSTANCE_COUNT);
  
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
  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
  glutInitWindowPosition (100,100); 
  glutInitWindowSize(1200,900); 
  glutCreateWindow("<<Instanced rendering>>"); 
  glewInit(); 
  Initialize( );
  glutDisplayFunc(RenderFunction);
  glutIdleFunc(RenderFunction);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
  glutCloseFunc(Cleanup);
  glutMainLoop();
 
}

