// Shader-ul de varfuri  
 
 #version 400


in vec4 in_Position;
in vec4 in_Color;

out vec4 gl_Position; 
out vec4 ex_Color;
uniform mat4 translMatrix;
uniform mat4 resizeMatrix;
uniform mat4 matrScale;
uniform mat4 matrTransl;
uniform mat4 matrTranslD1;
uniform mat4 matrTranslD2;
uniform int modify;


void main(void)
{

    if(modify == 1)
        gl_Position =resizeMatrix*translMatrix*matrTransl*in_Position;
    else if(modify == 2)
        gl_Position =resizeMatrix*translMatrix*matrTranslD2*matrScale*matrTranslD1*in_Position;
    else
        gl_Position =resizeMatrix*translMatrix*in_Position;

    ex_Color = in_Color;
} 
 