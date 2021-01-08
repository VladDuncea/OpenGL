//
//#include <windows.h>  // biblioteci care urmeaza sa fie incluse
//#include <stdlib.h> // necesare pentru citirea shader-elor
//#include <stdio.h>
//#include <math.h>
//#include <iostream>
//#include <GL/glew.h> // glew apare inainte de freeglut
//#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
//
//#include "loadShaders.h"
//
//#include "glm/glm/glm.hpp"  
//#include "glm/glm/gtc/matrix_transform.hpp"
//#include "glm/glm/gtx/transform.hpp"
//#include "glm/glm/gtc/type_ptr.hpp"
// 
//
//using namespace std;
//  
////////////////////////////////////////
//// identificatori 
//GLuint
//  VaoId,
//  VboId,
//  ColorBufferId,
//  ProgramId,
//  myMatrixLocation,
//  viewLocation,
//  projLocation,
//  codColLocation,
//  depthLocation;
//
// GLuint texture;
//
//int codCol;
//float PI=3.141592;
//
//// matrice utilizate
//glm::mat4 myMatrix; 
//
//// elemente pentru matricea de vizualizare
//float Obsx=0.0, Obsy=-600.0, Obsz=0.f;
//float Refx=0.0f, Refy=1000.0f, Refz=0.0f;
//float Vx=0.0, Vy=0.0, Vz=1.0;
//glm::mat4 view;
//
//// elemente pentru matricea de proiectie
//float width=800, height=600, xwmin=-800.f, xwmax=800, ywmin=-600, ywmax=600, znear=0.1, zfar=1, fov=45;
//glm::mat4 projection;
// 
//
//enum {
//    Il_Frag, Il_Frag_Av, Il_Vert, Il_Vert_Av
//};
//
//int rendermode;
//
//void menu(int selection)
//{
//    rendermode = selection;
//    glutPostRedisplay();
//}
//int l1, l2;
//
//void displayMatrix ( )
//{
//	for (int ii = 0; ii < 4; ii++)
//	{
//		for (int jj = 0; jj < 4; jj++)
//		cout <<  myMatrix[ii][jj] << "  " ;
//		cout << endl;
//	};
//
//};
//
//void processNormalKeys(unsigned char key, int x, int y)
//{
//
//	switch (key) {
//		case 'l' :
//			Vx -= 0.1;
//			break;
//		case 'r' :
//			Vx += 0.1;
//			break;
//		case '+' :
//			Obsy+=10;
//			break;
//		case '-' :
//			Obsy-=10;
//			break;
//
//	}
//if (key == 27)
//exit(0);
//}
//void processSpecialKeys(int key, int xx, int yy) {
//
//	switch (key) {
//		case GLUT_KEY_LEFT :
//			Obsx-=20;
//			break;
//		case GLUT_KEY_RIGHT :
//			Obsx+=20;
//			break;
//		case GLUT_KEY_UP :
//			Obsz+=20;
//			break;
//		case GLUT_KEY_DOWN :
//			Obsz-=20;
//			break;
//	}
//}
//
//void CreateVBO(void)
//{
//  // varfurile 
//  GLfloat Vertices[] = {
//	
//	  // inspre Oz'
//	    -50.f, -50.f, -50.f,  0.0f,  0.0f, -1.0f,
//         50.f, -50.f, -50.f,  0.0f,  0.0f, -1.0f,
//         50.f,  50.f, -50.f,  0.0f,  0.0f, -1.0f,
//         50.f,  50.f, -50.f,  0.0f,  0.0f, -1.0f,
//        -50.f,  50.f, -50.f,  0.0f,  0.0f, -1.0f,
//        -50.f, -50.f, -50.f,  0.0f,  0.0f, -1.0f,
//
//		// inspre Oz
//        -50.f, -50.f,  50.f,  0.0f,  0.0f,  1.0f,
//         50.f, -50.f,  50.f,  0.0f,  0.0f,  1.0f,
//         50.f,  50.f,  50.f,  0.0f,  0.0f,  1.0f,
//         50.f,  50.f,  50.f,  0.0f,  0.0f,  1.0f,
//        -50.f,  50.f,  50.f,  0.0f,  0.0f,  1.0f,
//        -50.f, -50.f,  50.f,  0.0f,  0.0f,  1.0f,
//
//		// inspre Ox'
//        -50.f,  50.f,  50.f, -1.0f,  0.0f,  0.0f,
//        -50.f,  50.f, -50.f, -1.0f,  0.0f,  0.0f,
//        -50.f, -50.f, -50.f, -1.0f,  0.0f,  0.0f,
//        -50.f, -50.f, -50.f, -1.0f,  0.0f,  0.0f,
//        -50.f, -50.f,  50.f, -1.0f,  0.0f,  0.0f,
//        -50.f,  50.f,  50.f, -1.0f,  0.0f,  0.0f,
//
//		// inspre Ox
//         50.f,  50.f,  50.f,  1.0f,  0.0f,  0.0f,
//         50.f,  50.f, -50.f,  1.0f,  0.0f,  0.0f,
//         50.f, -50.f, -50.f,  1.0f,  0.0f,  0.0f,
//         50.f, -50.f, -50.f,  1.0f,  0.0f,  0.0f,
//         50.f, -50.f,  50.f,  1.0f,  0.0f,  0.0f,
//         50.f,  50.f,  50.f,  1.0f,  0.0f,  0.0f,
//
//		 // inspre Oy'
//        -50.f, -50.f, -50.f,  0.0f, -1.0f,  0.0f,
//         50.f, -50.f, -50.f,  0.0f, -1.0f,  0.0f,
//         50.f, -50.f,  50.f,  0.0f, -1.0f,  0.0f,
//         50.f, -50.f,  50.f,  0.0f, -1.0f,  0.0f,
//        -50.f, -50.f,  50.f,  0.0f, -1.0f,  0.0f,
//        -50.f, -50.f, -50.f,  0.0f, -1.0f,  0.0f,
//
//		// inspre Oy
//        -50.f,  50.f, -50.f,  0.0f,  1.0f,  0.0f,
//         50.f,  50.f, -50.f,  0.0f,  1.0f,  0.0f,
//         50.f,  50.f,  50.f,  0.0f,  1.0f,  0.0f,
//         50.f,  50.f,  50.f,  0.0f,  1.0f,  0.0f,
//        -50.f,  50.f,  50.f,  0.0f,  1.0f,  0.0f,
//        -50.f,  50.f, -50.f,  0.0f,  1.0f,  0.0f,
//
//        // Fiecare varf cu normala lui
//
//        // inspre Oz'
//          - 50.f, -50.f, -50.f,  -1.0f,  -1.0f, -1.0f,
//           50.f, -50.f, -50.f,  1.0f,  -1.0f, -1.0f,
//           50.f,  50.f, -50.f,  1.0f,  1.0f, -1.0f,
//           50.f,  50.f, -50.f,  1.0f,  1.0f, -1.0f,
//          -50.f,  50.f, -50.f,  -1.0f,  1.0f, -1.0f,
//          -50.f, -50.f, -50.f,  -1.0f,  -1.0f, -1.0f,
//
//      // inspre Oz
//      -50.f, -50.f,  50.f,  -1.0f,  -1.0f,  1.0f,
//       50.f, -50.f,  50.f,  1.0f,  -1.0f,  1.0f,
//       50.f,  50.f,  50.f,  1.0f,  1.0f,  1.0f,
//       50.f,  50.f,  50.f,  1.0f,  1.0f,  1.0f,
//      -50.f,  50.f,  50.f,  -1.0f,  1.0f,  1.0f,
//      -50.f, -50.f,  50.f,  -1.0f,  -1.0f,  1.0f,
//
//      // inspre Ox'
//      -50.f,  50.f,  50.f, -1.0f,  1.0f,  1.0f,
//      -50.f,  50.f, -50.f, -1.0f,  1.0f,  -1.0f,
//      -50.f, -50.f, -50.f, -1.0f,  -1.0f,  -1.0f,
//      -50.f, -50.f, -50.f, -1.0f,  -1.0f,  -1.0f,
//      -50.f, -50.f,  50.f, -1.0f,  -1.0f,  1.0f,
//      -50.f,  50.f,  50.f, -1.0f,  1.0f,  1.0f,
//
//      // inspre Ox
//       50.f,  50.f,  50.f,  1.0f,  1.0f,  1.0f,
//       50.f,  50.f, -50.f,  1.0f,  1.0f,  -1.0f,
//       50.f, -50.f, -50.f,  1.0f,  -1.0f,  -1.0f,
//       50.f, -50.f, -50.f,  1.0f,  -1.0f,  -1.0f,
//       50.f, -50.f,  50.f,  1.0f,  -1.0f,  1.0f,
//       50.f,  50.f,  50.f,  1.0f,  1.0f,  1.0f,
//
//      // inspre Oy'
//     -50.f, -50.f, -50.f,  -1.0f, -1.0f,  -1.0f,
//      50.f, -50.f, -50.f,  1.0f, -1.0f,  -1.0f,
//      50.f, -50.f,  50.f,  1.0f, -1.0f,  1.0f,
//      50.f, -50.f,  50.f,  1.0f, -1.0f,  1.0f,
//     -50.f, -50.f,  50.f,  -1.0f, -1.0f,  1.0f,
//     -50.f, -50.f, -50.f,  -1.0f, -1.0f,  -1.0f,
//
//      // inspre Oy
//      -50.f,  50.f, -50.f,  -1.0f,  1.0f,  -1.0f,
//       50.f,  50.f, -50.f,  1.0f,  1.0f,  -1.0f,
//       50.f,  50.f,  50.f,  1.0f,  1.0f,  1.0f,
//       50.f,  50.f,  50.f,  1.0f,  1.0f,  1.0f,
//      -50.f,  50.f,  50.f,  -1.0f,  1.0f,  1.0f,
//      -50.f,  50.f, -50.f,  -1.0f,  1.0f,  -1.0f
//  };
//
// 
//  // se creeaza un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
//  glGenVertexArrays(1, &VaoId);
//  // se creeaza un buffer nou (atribute)
//  glGenBuffers(1, &VboId);
// 
//
//  // legarea VAO 
//  glBindVertexArray(VaoId);
//
//  // legarea buffer-ului "Array"
//  glBindBuffer(GL_ARRAY_BUFFER, VboId);
//  // punctele sunt "copiate" in bufferul curent
//  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
//
//  
//  // se activeaza lucrul cu atribute; atributul 0 = pozitie
//  glEnableVertexAttribArray(0);
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),(GLvoid*)0);
//
//  // se activeaza lucrul cu atribute; atributul 1 = normale
//  glEnableVertexAttribArray(1);
//  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),(GLvoid*)(3 * sizeof(GLfloat)));
// 
//}
//void DestroyVBO(void)
//{
//   glDisableVertexAttribArray(1);
//  glDisableVertexAttribArray(0);
//
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
// 
//  glDeleteBuffers(1, &VboId);
// 
//
//  glBindVertexArray(0);
//  glDeleteVertexArrays(1, &VaoId);
//   
//}
// 
//void CreateShadersVertex(void)
//{
//  ProgramId=LoadShaders("10_01v_Shader.vert", "10_01v_Shader.frag");
//  glUseProgram(ProgramId);
//}
//void CreateShadersFragment(void)
//{
//    ProgramId = LoadShaders("10_01f_Shader.vert", "10_01f_Shader.frag");
//    glUseProgram(ProgramId);
//}
//void DestroyShaders(void)
//{
//  glDeleteProgram(ProgramId);
//}
// 
//void Initialize(void)
//{
//    myMatrix = glm::mat4(1.0f);
//    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
//
//    CreateVBO();
//	
//}
//void RenderFunction(void)
//{
//  
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   glEnable(GL_DEPTH_TEST);
//    
//    // reperul de vizualizare
//	glm::vec3 Obs = glm::vec3 (Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
//	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
//	glm::vec3 Vert =  glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
//  	view = glm::lookAt(Obs, PctRef, Vert);
//	 	 
//    projection = glm::infinitePerspective(fov, GLfloat(width)/GLfloat(height), znear);
//    myMatrix = glm::mat4(1.0f);
// 
//  
// 
//  switch (rendermode) {
//
//  case Il_Frag:
//      CreateShadersFragment();
//      l1 = 0; l2 = 36;
//      break;
//  case Il_Frag_Av:
//      CreateShadersFragment();
//      l1 = 36; l2 = 36;
//      break;
//  case Il_Vert:
//      CreateShadersVertex();
//      l1 = 0; l2 = 36;
//      break;
//  case Il_Vert_Av:
//      CreateShadersVertex();
//      l1 = 36; l2 = 36;
//      break;
//  };
//
// // variabile uniforme pentru transformari
//  myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
//  glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
//  viewLocation = glGetUniformLocation(ProgramId, "view");
//  glUniformMatrix4fv(viewLocation, 1, GL_FALSE,  &view[0][0]);
//  projLocation = glGetUniformLocation(ProgramId, "projection");
//  glUniformMatrix4fv(projLocation, 1, GL_FALSE,  &projection[0][0]);
// 
//  // Variabile uniforme pentru iluminare
//  GLint objectColorLoc = glGetUniformLocation(ProgramId, "objectColor");
//  GLint lightColorLoc  = glGetUniformLocation(ProgramId, "lightColor");
//  GLint lightPosLoc    = glGetUniformLocation(ProgramId, "lightPos");
//  GLint viewPosLoc     = glGetUniformLocation(ProgramId, "viewPos");
//  glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.4f);
//  glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
//  glUniform3f(lightPosLoc, 0.f, -150.f, 200.f);
//  glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
//
//  // desenare
//  glDrawArrays(GL_TRIANGLES, l1, l2);
//
//  // Eliberare memorie si realocare resurse
//  DestroyShaders ( );
//  // 
// 
//  glutSwapBuffers(); 
//  glFlush ( );
//}
//void Cleanup(void)
//{
//  DestroyShaders();
//  DestroyVBO();
//}
//
//int main(int argc, char* argv[])
//{
// 
//  glutInit(&argc, argv);
//  glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
//  glutInitWindowPosition (100,100); 
//  glutInitWindowSize(1200,900); 
//  glutCreateWindow("Implementarea modelului de iluminare"); 
//  glewInit(); 
//  Initialize( );
//  glutIdleFunc(RenderFunction);
//  glutDisplayFunc(RenderFunction);
//  glutKeyboardFunc(processNormalKeys);
//  glutSpecialFunc(processSpecialKeys);
//  glutCreateMenu(menu);
//  glutAddMenuEntry("Fragment", Il_Frag);
//  glutAddMenuEntry("Fragment+Mediere_Normale", Il_Frag_Av);
//  glutAddMenuEntry("Varfuri", Il_Vert);
//  glutAddMenuEntry("Varfuri+Mediere_Normale", Il_Vert_Av);
//  glutAttachMenu(GLUT_RIGHT_BUTTON);
//
//  glutCloseFunc(Cleanup);
//  glutMainLoop();
// 
//}
//
