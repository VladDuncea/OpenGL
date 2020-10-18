// Codul sursa este adaptat dupa OpenGLBook.com

#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "loadShaders.h"


//////////////////////////////////////

GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId;


float tX(int x)
{
    return (x - 400) / 400.0;
}

float tY(int x)
{
    return (x - 350) / 350.0;
}

void CreateVBO(void)
{
    // varfurile 
    GLfloat Vertices[] = {
        tX(20), tY(130), 0.0f, 1.0f,
        tX(140), tY(130), 0.0f, 1.0f,
        tX(140), tY(310), 0.0f, 1.0f,
        tX(20),  tY(310), 0.0f, 1.0f,

        tX(0), tY(0), 0.0f, 1.0f,
        tX(100), tY(10), 0.0f, 1.0f,
        tX(200), tY(120), 0.0f, 1.0f,
        tX(155), tY(290), 0.0f, 1.0f,

        tX(250), tY(300), 0.0f, 1.0f,
        tX(50), tY(300), 0.0f, 1.0f,
        tX(200), tY(350), 0.0f, 1.0f,
        tX(250), tY(500), 0.0f, 1.0f,
        tX(350), tY(200), 0.0f, 1.0f,

        tX(650), tY(300), 0.0f, 1.0f,
        tX(450), tY(300), 0.0f, 1.0f,
        tX(600), tY(350), 0.0f, 1.0f,
        tX(650), tY(500), 0.0f, 1.0f,
        tX(750), tY(200), 0.0f, 1.0f,

    };

    // culorile, ca atribute ale varfurilor
    GLfloat Colors[] = {
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.25f, 1.0f,
        1.0f, 0.0f, 0.25f, 1.0f,
        1.0f, 0.0f, 0.25f, 1.0f,
        1.0f, 0.0f, 0.25f, 1.0f,
        1.0f, 0.0f, 0.25f, 1.0f,

        1.0f, 0.75f, 0.25f, 1.0f,
        1.0f, 0.75f, 0.25f, 1.0f,
        1.0f, 0.75f, 0.25f, 1.0f,
        1.0f, 0.75f, 0.25f, 1.0f,
        1.0f, 0.75f, 0.25f, 1.0f,
    };


    // se creeaza un buffer nou
    glGenBuffers(1, &VboId);
    // este setat ca buffer curent
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // varfurile sunt "copiate" in bufferul curent
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

void CreateShaders(void)
{
    ProgramId = LoadShaders("02_01_Shader.vert", "02_01_Shader.frag");
    glUseProgram(ProgramId);
}
void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
    CreateVBO();
    CreateShaders();
}
void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_POLYGON, 0, 4);

    glDrawArrays(GL_POLYGON, 4, 4);

    glDrawArrays(GL_TRIANGLE_FAN, 8, 5);

    glDrawArrays(GL_TRIANGLE_STRIP, 13, 5);

    glFlush();
}
void Cleanup(void)
{
    DestroyShaders();
    DestroyVBO();
}

int main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100); // pozitia initiala a ferestrei
    glutInitWindowSize(600, 400); //dimensiunile ferestrei
    glutCreateWindow("Transfer 01_04 la OpenGL nou"); // titlul ferestrei
    glewInit(); // nu uitati de initializare glew; trebuie initializat inainte de a a initializa desenarea
    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();


}

