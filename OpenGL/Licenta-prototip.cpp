// /* FUNCTII DE DESENARE
// Elemente de noutate:
//	- folosirea indecsilor: elemente asociate (matrice, buffer)
//	- cele 4 functii de desenare (glDrawArrays, glDrawElements, glDrawElementsBaseVertex, glDrawArraysInstanced) */
//#include <windows.h>  // biblioteci care urmeaza sa fie incluse
//#include <stdlib.h> // necesare pentru citirea shader-elor
//#include <stdio.h>
//#include <math.h>
//#include <iostream>
//#include <GL/glew.h> // glew apare inainte de freeglut
//#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
//
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <list>
//#include <queue>
//
//#include "loadShaders.h"
//
//#include "glm/glm/glm.hpp"  
//#include "glm/glm/gtc/matrix_transform.hpp"
//#include "glm/glm/gtx/transform.hpp"
//#include "glm/glm/gtc/type_ptr.hpp"
//
//using namespace std;
//  
////////////////////////////////////////
//
//GLuint
//	VaoId,
//	VboId,
//	EboId,
//	ColorBufferId,
//	ProgramId,
//	codColLocation,
//	defaultTranslateLocation,
//	defaultResizeLocation,
//	viewLocation,
//	projectionLocation,
//	modifyLocation,
//	pointSizeLocation;
// 
//
//glm::mat4 defaultResize, defaultTranslate, view, projection;
//
//int codCol, modify = 0, pointSize;
//float PI=3.141592, angle=0;
//float tx=0; float ty=0; 
//float width=200, height=200, depth = 200;
//float i=0.0, alpha=0.0, beta=0.2;
//
//int nr_triunghiuri, nr_puncte;
//
//class state
//{
//public:
//	int cost, level, last_node;
//	vector<bool> used;
//	state* father;
//	vector<vector<int>> matrix;
//
//	state(vector<bool>& used, int cost = 0, state* father = NULL)
//	{
//		this->cost = cost;
//		this->father = father;
//		this->used = used;
//		last_node = 0;
//		level = 1;
//	}
//
//};
//
//struct customCmp
//{
//	bool operator()(const state* a, const state* b)
//	{
//		return a->cost > b->cost;
//	}
//};
//
//void print_sol(state* st)
//{
//	if (st == NULL)
//		return;
//
//	print_sol(st->father);
//	std::cout << st->last_node + 1 << " ";
//}
//
//int reduction(vector<vector<int>>& matrix)
//{
//	int n = matrix.size(), red = 0;
//	for (int i = 0; i < n; i++)
//	{
//		int min = -1;
//		for (int j = 0; j < n; j++)
//		{
//			if (matrix[i][j] != -1 && (matrix[i][j] < min || min == -1))
//			{
//				min = matrix[i][j];
//			}
//		}
//
//		//nothing to do
//		if (min <= 0)
//			continue;
//
//		red += min;
//		for (int j = 0; j < n; j++)
//		{
//			if (matrix[i][j] != -1)
//				matrix[i][j] -= min;
//		}
//	}
//
//	for (int i = 0; i < n; i++)
//	{
//		int min = -1;
//		for (int j = 0; j < n; j++)
//		{
//			if (matrix[j][i] != -1 && (matrix[j][i] < min || min == -1))
//			{
//				min = matrix[j][i];
//			}
//		}
//
//		//nothing to do
//		if (min <= 0)
//			continue;
//
//		red += min;
//		for (int j = 0; j < n; j++)
//		{
//			if (matrix[j][i] != -1)
//				matrix[j][i] -= min;
//		}
//	}
//
//	return red;
//}
//
//void print_matrix(vector<vector<int>> matrix)
//{
//	for (int i = 0; i < matrix.size(); i++)
//	{
//		for (int j = 0; j < matrix[i].size(); j++)
//		{
//			cout << matrix[i][j] << " ";
//		}
//		cout << endl;
//	}
//	cout << flush;
//}
//
//void problema4(const GLfloat vf_pos[])
//{
//	priority_queue<state*, vector<state*>, customCmp> min_heap;
//	vector<vector<int>> matrix;
//	ifstream fin("date.in");
//	int n = nr_puncte, red;
//	matrix.resize(n);
//
//	//read adj matrix
//	for (int i = 0; i < n; i++)
//	{
//		int point, nr_neigh;
//		fin >> point;
//		matrix[point].resize(n,-1);
//
//		fin >> nr_neigh;
//
//		for (int j = 0; j < nr_neigh; j++)
//		{
//			int x;
//			fin >> x;
//			matrix[i][x] = x;
//		}
//	}
//
//	print_matrix(matrix);
//
//	//reduction
//	red = reduction(matrix);
//
//	vector<bool> used(n, 0);
//	used[0] = 1;
//	//add initial state to min_heap
//	state* initial_state = new state(used);
//	initial_state->matrix = matrix;
//	initial_state->cost = red;
//
//	min_heap.push(initial_state);
//
//	//to be used after finding solutions
//	int best_sol = -1;
//	state* best_state = NULL;
//
//	//Extract states one by one
//	while (!min_heap.empty())
//	{
//		//take best state out
//		state* selected_state = min_heap.top();
//		min_heap.pop();
//
//		//poped bad node
//		if (selected_state->cost > best_sol && best_sol != -1)
//		{
//			delete(selected_state);
//			continue;
//		}
//
//		//compute all of its children that can yield solutions and add them to min_heap
//		for (int i = 1; i < n; i++)
//		{
//			//jump over used nodes
//			if (selected_state->used[i])
//				continue;
//
//			state* child = new state(selected_state->used, selected_state->cost, selected_state);
//			child->level = selected_state->level + 1;
//			child->matrix = selected_state->matrix;
//			child->last_node = i;
//
//			//last level node, we can compute real cost and add as solution
//			if (selected_state->level == n - 1)
//			{
//				child->cost += child->matrix[selected_state->last_node][i] + child->matrix[i][0];
//
//				if (child->cost < best_sol || best_sol == -1)
//				{
//					best_sol = child->cost;
//					best_state = child;
//				}
//			}
//			else
//			{
//				//line i is -1
//				for (int j = 0; j < n; j++)
//					child->matrix[selected_state->last_node][j] = -1;
//
//				//column j is -1
//				for (int j = 0; j < n; j++)
//					child->matrix[j][i] = -1;
//
//				//dont go back
//				child->matrix[i][0] = -1;
//
//				//reduce matrix
//				child->cost += reduction(child->matrix);
//
//				//not last level node, we compute cost an insert it in min_heap
//				child->cost += selected_state->matrix[selected_state->last_node][i];
//
//				//check if child can make a better solution
//				if (child->cost >= best_sol && best_sol != -1)
//				{
//					delete(child);
//					continue;
//				}
//
//				child->used[i] = true;
//				min_heap.push(child);
//			}
//		}
//
//	}
//
//	//print solution
//	cout << "Cost minim:" << best_sol << endl;
//	print_sol(best_state);
//	cout << "1" << endl;
//
//}
//
//void CreateVBO(void)
//{
// 
//	// coordonatele varfurilor
//	static const GLfloat vf_pos[] =
//	{
//		50.0f,		150.0f,		100.0f,		1.0f,
//		50.0f,		100.0f,		100.0f,		1.0f,
//		50.0f,		50.0f,		100.0f,		1.0f,
//
//		100.0f,		150.0f,		120.0f,		1.0f,
//		100.0f,		100.0f,		120.0f,		1.0f,
//		100.0f,		50.0f,		100.0f,		1.0f,
//								
//		150.0f,		150.0f,		100.0f,		1.0f,
//		150.0f,		100.0f,		100.0f,		1.0f,
//		150.0f,		50.0f,		100.0f,		1.0f,
//	};
//	nr_puncte = 9;
//	problema4(vf_pos);
//
//	// culorile varfurilor
//	static const GLfloat vf_col[] =
//	{
//		1.0f, 0.0f, 0.0f, 1.0f,
//		1.0f, 0.0f, 0.0f, 1.0f,
//		1.0f, 0.0f, 0.0f, 1.0f,
//
//		0.0f, 1.0f, 0.0f, 1.0f,
//		0.0f, 1.0f, 0.0f, 1.0f,
//		0.0f, 1.0f, 0.0f, 1.0f,
//			  
//		1.0f, 1.0f, 0.0f, 1.0f,
//		1.0f, 1.0f, 0.0f, 1.0f,
//		1.0f, 1.0f, 0.0f, 1.0f,
//		
//	};
//
//	nr_triunghiuri = 8;
//
//	// indici pentru trasarea unui triunghi
//	static const GLuint vf_ind[] =
//	{
//		0,1,3,
//		1,4,3,
//		1,2,5,
//		1,5,4,
//
//		3,4,6,
//		4,7,6,
//		4,5,7,
//		5,8,7
//	};
// 
//
//	// se creeaza un buffer nou pentru varfuri
//	glGenBuffers(1, &VboId);
//	// buffer pentru indici
//	glGenBuffers(1, &EboId);
//	// se creeaza / se leaga un VAO (Vertex Array Object)
//	glGenVertexArrays(1, &VaoId);
//
//
//	// legare VAO
//	glBindVertexArray(VaoId);
// 
//	// buffer-ul este setat ca buffer curent
//	glBindBuffer(GL_ARRAY_BUFFER, VboId);
//
//	// buffer-ul va contine atat coordonatele varfurilor, cat si datele de culoare
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vf_col)+sizeof(vf_pos), NULL, GL_STATIC_DRAW);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vf_pos), vf_pos);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vf_pos), sizeof(vf_col),vf_col);
// 
//	// buffer-ul pentru indici
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vf_ind), vf_ind, GL_STATIC_DRAW);
// 
//	// se activeaza lucrul cu atribute; atributul 0 = pozitie, atributul 1 = culoare, acestea sunt indicate corect in VBO
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vf_pos));
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//
// 
//}
//
//void DestroyVBO(void)
//{
//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(0);
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glDeleteBuffers(1, &EboId);
//	glDeleteBuffers(1, &ColorBufferId);
//	glDeleteBuffers(1, &VboId);
//
//	glBindVertexArray(0);
//	glDeleteVertexArrays(1, &VaoId);
//}
//
//void CreateShaders(void)
//{
//	ProgramId=LoadShaders("Licenta_prototip.vert", "08_02_Shader.frag");
//	glUseProgram(ProgramId);
//}
// 
//void DestroyShaders(void)
//{
//	glDeleteProgram(ProgramId);
//}
// 
//void Initialize(void)
//{
//	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
//
//	// Enable blending
//	// Folosim blending pentru transparente
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	float camLookX = -0.2, camLookY = 0.5, camHeight = 0.8;
//	float camPozX = 0.5, camPozY = -0.8;
//
//	//Pozitionarea camerei
//	glm::vec3 cameraPos = glm::vec3(camPozX, camPozY, camHeight);
//	glm::vec3 cameraTarget = glm::vec3(camLookX, camLookY, -0.2f);
//	view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 0.0f, 1.0f));
//
//	//Perspectiva
//	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 10.0f);
//	//glm::mat4 projection = glm::ortho(-2.0,2.0,-2.0,2.0,0.1,2.0);
//
//	//matrice pentru mutare din [0,width] x [0,height] in [-width/2,width/2] x [-height/2,height/2]
//	defaultTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-width / 2, -height / 2, -depth/2));
//	// matricea pentru scalare la patratul standard
//	defaultResize = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / (width / 2), 1.f / (height / 2), 1.f/(depth/2)));
//
//	//Compilare shader
//	CreateShaders();
//
//	//Locatii variabile shader
//	modifyLocation = glGetUniformLocation(ProgramId, "modify");
//	defaultTranslateLocation = glGetUniformLocation(ProgramId, "defaultTranslate");
//	defaultResizeLocation = glGetUniformLocation(ProgramId, "defaultResize");
//	viewLocation = glGetUniformLocation(ProgramId, "view");
//	projectionLocation = glGetUniformLocation(ProgramId, "projection");
//
//	codColLocation = glGetUniformLocation(ProgramId, "codCol");
//	pointSizeLocation = glGetUniformLocation(ProgramId, "pointSize");
//
//	//Transmitere matrice unice
//	// matricea de mutare default
//	glUniformMatrix4fv(defaultTranslateLocation, 1, GL_FALSE, &defaultTranslate[0][0]);
//	// matricea de redimensionare
//	glUniformMatrix4fv(defaultResizeLocation, 1, GL_FALSE, &defaultResize[0][0]);
//	// camera
//	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
//	// proiectia
//	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
//
//	CreateVBO();
//}
//
//void RenderFunction(void)
//{
//
//	//Initializare modify - pt 0 nu face decat sa aduca la [-1, 1],
//	modify = 0;
//	glUniform1i(modifyLocation, modify);
//
//	pointSize = 1;
//	glUniform1i(pointSizeLocation, pointSize);
//
//	//Culoarea default
//	codCol = 0;
//	glUniform1i(codColLocation, codCol);
//
//	//Clear
//	glClear(GL_COLOR_BUFFER_BIT);
//
//
//	//Draw mesh
//	glDrawElements(GL_TRIANGLES, nr_triunghiuri * 3, GL_UNSIGNED_INT, (void*)(0));
//
//	//Culoarea neagra pentru linii
//	codCol = 1;
//	glUniform1i(codColLocation, codCol);
//
//	//Draw mesh edges
//	for (int i = 0; i < nr_triunghiuri;i++)
//	{
//		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (GLvoid*)(3* i * sizeof(unsigned int)));
//	}
//
//	glFlush();
//
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
//	float scale = 6;
//
//	glutInit(&argc, argv);
//	glutSetOption(GLUT_MULTISAMPLE, 4);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_MULTISAMPLE);
//	glutInitWindowPosition (100,100); 
//	glutInitWindowSize(scale * width, scale * height);
//	glutCreateWindow("Proof of concept Duncea"); 
//	glewInit(); 
//	Initialize( );
//	glutDisplayFunc(RenderFunction);
//	glutCloseFunc(Cleanup);
//	glutMainLoop();
//}
//
