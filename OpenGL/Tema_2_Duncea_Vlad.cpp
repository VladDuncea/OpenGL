///* Teama 2 - Duncea Vlad Alexandru Grupa 344, Am folosit shaderul .frag din curs
//* Shaderul .vert este modificat si a fost incarcat cu tema
//*/
//
//#include <windows.h> 
//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>
//#include <GL/glew.h> 
//#include <GL/freeglut.h>
//#include "loadShaders.h"
//#include <vector>
//
//
//#include "glm/glm/glm.hpp"  
//#include "glm/glm/gtc/matrix_transform.hpp"
//#include "glm/glm/gtx/transform.hpp"
//#include "glm/glm/gtc/type_ptr.hpp"
//
////////////////////////////////////////
//
//using namespace std;
//
//GLuint
//VaoId,
//VboId,
//ColorBufferId,
//ProgramId,
//translMatrixLocation,
//resizeMatrixLocation,
//matrScaleLocation,
//modifyLocation,
//matrTranslLocation,
//matrTranslD1Location,
//matrTranslD2Location;
//
//int modify;
//float PI = 3.141592;
//
////Coordonate conform cerintei
//int width = 400, height = 500;
//
//
//glm::mat4 matrTransl, matrTranslD1, matrTranslD2, resizeMatrix, translMatrix, matrScale, matrRot, matrTransl1, matrTransl2;
//glm::vec3 pctTrans,pctTransD;
//
//float centru_greutate(vector<float> points) 
//{
//	float s = 0;
//	int n = points.size();
//
//	for (int i = 0; i < n; i++)
//		s += points[i];
//
//	return s / n;
//}
//
//void CreateVBO(void)
//{
//	// varfurile 
//	GLfloat Vertices[] = {
//		//Varfuri pt background
//		0, 0, 0.0f, 1.0f,
//		2*width, 0, 0.0f, 1.0f,
//		2*width, 2*height, 0.0f, 1.0f,
//		0, 2*height, 0.0f, 1.0f,
//
//		//Varfuri dreptunghi
//		200.0f,200.0f, 0.0f, 1.0f,
//		300.0f,200.0f, 0.0f, 1.0f,
//		300.0f,350.0f, 0.0f, 1.0f,
//		200.0f,350.0f, 0.0f, 1.0f,
//
//		//Varfuri P1
//		400.0f, 500.0f, 0.0f, 1.0f,
//		500.0f, 400.0f, 0.0f, 1.0f,
//		600.0f, 400.0f, 0.0f, 1.0f,
//		500.0f, 600.0f, 0.0f, 1.0f,
//		400.0f, 700.0f, 0.0f, 1.0f,
//
//		//Varfuri P2 ( construit din triunghiuri)
//		300.0f, 600.0f, 0.0f, 1.0f,
//		300.0f, 800.0f, 0.0f, 1.0f,
//		200.0f, 700.0f, 0.0f, 1.0f,
//
//		300.0f, 800.0f, 0.0f, 1.0f,
//		100.0f, 800.0f, 0.0f, 1.0f,
//		200.0f, 700.0f, 0.0f, 1.0f,
//
//		300.0f, 600.0f, 0.0f, 1.0f,
//		200.0f, 700.0f, 0.0f, 1.0f,
//		100.0f, 600.0f, 0.0f, 1.0f,
//	};
//
//	//Calcul centru P1 P2
//
//	pctTrans[0] = (centru_greutate({400,500,600,500,400}) + centru_greutate({300,300,200,300,100,200,300,200,100}) )/2;
//	pctTrans[1] = (centru_greutate({ 500,400,400,600,700 }) + centru_greutate({ 600,800,700,800,800,700,600,700,600 }))/2;
//	pctTrans[2] = 0;
//
//	//matricele de translatie
//	matrTransl1 = glm::translate(glm::mat4(1.0f), -pctTrans);
//	matrTransl2 = glm::translate(glm::mat4(1.0f), pctTrans);
//
//	//Calcul centru D
//	pctTransD[0] = centru_greutate({200,300,300,200 });
//	pctTransD[1] = centru_greutate({ 200,200,350,350 });
//	pctTransD[2] = 0;
//
//	//matricea de translatie pt D
//	matrTranslD1 = glm::translate(glm::mat4(1.0f), -pctTransD);
//	matrTranslD2 = glm::translate(glm::mat4(1.0f), pctTransD);
//
//	// culorile varfurilor din colturi
//	GLfloat Colors[] = {
//		//Culori background
//		0.0f, 0.5f, 0.5f, 1.0f,
//		0.0f, 0.4f, 0.5f, 1.0f,
//		0.8f, 0.8f, 0.8f, 1.0f,
//		0.8f, 0.8f, 0.8f, 1.0f,
//
//		//Culori dreptungi
//		0.7f, 0.0f, 0.0f, 1.0f,
//		0.7f, 0.0f, 0.0f, 1.0f,
//		0.7f, 0.0f, 0.0f, 1.0f,
//		0.7f, 0.0f, 0.0f, 1.0f,
//
//		//Culori P1
//		0.0f, 0.4f, 0.0f, 1.0f,
//		0.0f, 0.4f, 0.0f, 1.0f,
//		0.0f, 0.4f, 0.0f, 1.0f,
//		0.0f, 0.4f, 0.0f, 1.0f,
//		0.0f, 0.4f, 0.0f, 1.0f,
//
//		//Culori P2
//		0.0f, 0.0f, 0.5f, 1.0f,
//		0.0f, 0.0f, 0.5f, 1.0f,
//		0.0f, 0.0f, 0.5f, 1.0f,
//		0.0f, 0.0f, 0.5f, 1.0f,
//		0.0f, 0.0f, 0.5f, 1.0f,
//		0.0f, 0.0f, 0.5f, 1.0f,
//		0.0f, 0.0f, 0.5f, 1.0f,
//		0.0f, 0.0f, 0.5f, 1.0f,
//		0.0f, 0.0f, 0.5f, 1.0f,
//	};
//
//
//	// se creeaza un buffer nou
//	glGenBuffers(1, &VboId);
//	// este setat ca buffer curent
//	glBindBuffer(GL_ARRAY_BUFFER, VboId);
//	// punctele sunt "copiate" in bufferul curent
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
//
//	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
//	glGenVertexArrays(1, &VaoId);
//	glBindVertexArray(VaoId);
//	// se activeaza lucrul cu atribute; atributul 0 = pozitie
//	glEnableVertexAttribArray(0);
//	// 
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
//
//	// un nou buffer, pentru culoare
//	glGenBuffers(1, &ColorBufferId);
//	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
//	// atributul 1 =  culoare
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
//
//
//}
//
////Din curs
//void DestroyVBO(void)
//{
//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glDeleteBuffers(1, &ColorBufferId);
//	glDeleteBuffers(1, &VboId);
//
//	glBindVertexArray(0);
//	glDeleteVertexArrays(1, &VaoId);
//}
//
////Din curs - modificat fisierul .vert
//void DefaultShader(void)
//{
//	ProgramId = LoadShaders("Tema_2_Duncea_Vlad_1.vert", "04_01_Shader.frag");
//	glUseProgram(ProgramId);
//}
//
////Din curs
//void DestroyShaders(void)
//{
//	glDeleteProgram(ProgramId);
//}
//
////Modificata
//void Initialize(void)
//{
//	//matrice pentru mutare din [0,800] x [0,1000] in [-400,400] x [-500,500]
//	translMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-width, -height, 1.0));
//	// matricea pentru scalare la patratul standard
//	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / width, 1.f / height, 1.0));
//
//	// matricea pentru rotatie (cu  90 grade)
//	matrRot = glm::rotate(glm::mat4(1.0f), PI / 2.0f, glm::vec3(0.0, 0.0, 1.0));
//
//	//Matrice scalare D
//	//Scalare *2 pe Ox, *0.5 pe Oy
//	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 0.5, 1.0));
//
//	//Avand un fundal complet colorat nu are mare importanta aceasta linie
//	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
//
//	//Compilare shader
//	DefaultShader();
//}
//
//void RenderFunction(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	CreateVBO();
//
//	//Initializare modify - pt 0 nu face decat sa aduca la -1-1,
//	modifyLocation = glGetUniformLocation(ProgramId, "modify");
//	modify = 0;
//	glUniform1i(modifyLocation, modify);
//
//	// matricea de mutare default
//	translMatrixLocation = glGetUniformLocation(ProgramId, "translMatrix");
//	glUniformMatrix4fv(translMatrixLocation, 1, GL_FALSE, &translMatrix[0][0]);
//	// matricea de redimensionare
//	resizeMatrixLocation = glGetUniformLocation(ProgramId, "resizeMatrix");
//	glUniformMatrix4fv(resizeMatrixLocation, 1, GL_TRUE, &resizeMatrix[0][0]);
//
//	// desenare background
//	glDrawArrays(GL_POLYGON, 0, 4);
//
//	//Desenare dreptunghi D
//	glDrawArrays(GL_POLYGON, 4, 4);
//
//	//Deseneaza P1
//	glDrawArrays(GL_POLYGON, 8, 5);
//
//	//Deseneaza P2
//	glDrawArrays(GL_TRIANGLES, 13, 9);
//
//	//Desen P1,P2 rotit cu 90 fata de centrul lor
//	modify = 1;
//	glUniform1i(modifyLocation, modify);
//
//	//Calcul aici(in program)
//	matrTransl = matrTransl2 * matrRot * matrTransl1;
//	matrTranslLocation = glGetUniformLocation(ProgramId, "matrTransl");
//	glUniformMatrix4fv(matrTranslLocation, 1, GL_FALSE, &matrTransl[0][0]);
//
//	glDrawArrays(GL_POLYGON, 8, 5);
//	glDrawArrays(GL_TRIANGLES, 13, 9);
//
//	//Translatare si Scalare D in shader
//	modify = 2;
//	glUniform1i(modifyLocation, modify);
//
//	matrScaleLocation = glGetUniformLocation(ProgramId, "matrScale");
//	matrTranslD1Location = glGetUniformLocation(ProgramId, "matrTranslD1");
//	matrTranslD2Location = glGetUniformLocation(ProgramId, "matrTranslD2");
//	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
//	glUniformMatrix4fv(matrTranslD1Location, 1, GL_FALSE, &matrTranslD1[0][0]);
//	glUniformMatrix4fv(matrTranslD2Location, 1, GL_FALSE, &matrTranslD2[0][0]);
//
//	glDrawArrays(GL_POLYGON, 4, 4);
//
//
//	glFlush();
//}
//
//void Cleanup(void)
//{
//	DestroyShaders();
//	DestroyVBO();
//}
//
//int main(int argc, char* argv[])
//{
//
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(1.5*width, 1.5*height);
//	glutCreateWindow("Tema 2");
//	glewInit();
//	Initialize();
//	glutDisplayFunc(RenderFunction);
//	glutCloseFunc(Cleanup);
//	glutMainLoop();
//}
//
