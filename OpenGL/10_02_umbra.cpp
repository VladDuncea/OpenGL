
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
  depthLocation;

 GLuint texture;

int codCol;
float PI=3.141592;

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
float xL = 0.f, yL = 0.f, zL = 400.f;

// matricea umbrei
float matrUmbra[4][4];


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
			Vx -= 0.1;
			break;
		case 'r' :
			Vx += 0.1;
			break;
		case '+' :
			Obsy+=10;
			break;
		case '-' :
			Obsy-=10;
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

        // coordonate                   // culori			// normale
        -50.0f,  -50.0f, 50.0f, 1.0f,  1.0f, 0.5f, 0.4f,  -1.0f, -1.0f, -1.0f,
        50.0f,  -50.0f,  50.0f, 1.0f,  1.0f, 0.5f, 0.4f,  1.0f, -1.0f, -1.0f,
        50.0f,  50.0f,  50.0f, 1.0f,   1.0f, 0.5f, 0.4f,  1.0f, 1.0f, -1.0f,
        -50.0f,  50.0f, 50.0f, 1.0f,   1.0f, 0.5f, 0.4f,  -1.0f, 1.0f, -1.0f,
        0.0f,   0.0f, 150.0f, 1.0f,  1.0f, 0.5f, 0.4f,  -1.0f, -1.0f, 1.0f,
        // 
       -1000.0f,  -1000.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f,
        1000.0f,  -1000.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f,
        1000.0f,  1000.0f,  0.0f, 1.0f,  1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f,
       -1000.0f,  1000.0f,  0.0f, 1.0f,  1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f,

    };

    // indicii pentru varfuri
    GLubyte Indices[] = {

      2, 1, 0,   2, 0, 3,   
      2, 3, 4,   3, 0, 4,
      0, 1, 4,   1, 2, 4,

      5,6,7,    7,8,5,

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
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);

    // se activeaza lucrul cu atribute; atributul 1 = culoare
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

    // se activeaza lucrul cu atribute; atributul 2 = normale
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));

}
void DestroyVBO(void)
{
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &VboId);
    glDeleteBuffers(1, &EboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
}

 
void CreateShaders(void)
{
  ProgramId=LoadShaders("10_02_Shader.vert", "10_02_Shader.frag");
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
	
}
void RenderFunction(void)
{
  
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
    
    // reperul de vizualizare
	glm::vec3 Obs = glm::vec3 (Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
    glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert =  glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
  	view = glm::lookAt(Obs, PctRef, Vert);
	 	 
    projection = glm::infinitePerspective(fov, GLfloat(width)/GLfloat(height), znear);
    myMatrix = glm::mat4(1.0f);
    // matricea pentru umbra
    float D = -5.f;
    matrUmbra[0][0] = zL+D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
    matrUmbra[1][0] = 0; matrUmbra[1][1] = zL+D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
    matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
    matrUmbra[3][0] = -D*xL; matrUmbra[3][1] = -D*yL; matrUmbra[3][2] = -D*zL; matrUmbra[3][3] = zL;
 
 
  CreateVBO();
 
 // variabile uniforme pentru shaderul de varfuri
  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
  matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
  glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE,  &matrUmbra[0][0]);
  viewLocation = glGetUniformLocation(ProgramId, "view");
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE,  &view[0][0]);
  projLocation = glGetUniformLocation(ProgramId, "projection");
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

  // desenare
  codCol = 0;
  glUniform1i(codColLocation, codCol);
  glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, 0);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)(18));

  codCol = 1;
  glUniform1i(codColLocation, codCol);
  glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, 0);


  // Eliberare memorie si realocare resurse
  DestroyVBO ( );
  DestroyShaders ( );

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
  glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
  glutInitWindowPosition (100,100); 
  glutInitWindowSize(1200,900); 
  glutCreateWindow("Implementarea modelului de iluminare"); 
  glewInit(); 
  Initialize( );
  glutIdleFunc(RenderFunction);
  glutDisplayFunc(RenderFunction);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
 

  glutCloseFunc(Cleanup);
  glutMainLoop();
 
}

