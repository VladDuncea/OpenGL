// Shader-ul de varfuri  
 
 #version 400


in vec4 in_Position;
in vec4 in_Color;

out vec4 gl_Position; 
out vec4 ex_Color;
uniform mat4 defaultTranslate;
uniform mat4 defaultResize;
uniform mat4 matrTransl;
uniform mat4 matrScale;
uniform mat4 matrRot;
//matrices for last rotation
uniform mat4 matrRotFinal;
uniform mat4 matrRotTransl1;
uniform mat4 matrRotTransl2;
uniform int modify;
uniform int rotateFinal;


void main(void)
{

    
    if(modify == 1)
    {
        //simple translate
        gl_Position = defaultTranslate*matrTransl*in_Position;
    }
    else if(modify == 2)
    {
        //scale and translate
        gl_Position = matrTransl*matrScale*defaultTranslate*in_Position;
    }
    else if(modify == 3)
    {
        //scale, rotate, translate(same point of reference)
        gl_Position = matrTransl*matrRot*matrScale*defaultTranslate*in_Position;
    }
    else
    {
        //Default, translate and resize for normal use
        gl_Position = defaultTranslate*in_Position;
    }

    if(rotateFinal == 1)
    {
        //translate, rotate, translate back
        gl_Position = matrRotTransl2*matrRotFinal*matrRotTransl1*gl_Position;
    }

    gl_Position = defaultResize*gl_Position;

    ex_Color = in_Color;
} 
 