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
uniform int modify;


void main(void)
{

    
    if(modify == 1)
    {
        //simple translate
        gl_Position = defaultResize*defaultTranslate*matrTransl*in_Position;
    }
    else if(modify == 2)
    {
        gl_Position = defaultResize*matrTransl*matrScale*defaultTranslate*in_Position;
    }
    else
    {
        //Default, translate and resize for normal use
        gl_Position = defaultResize*defaultTranslate*in_Position;
    }

    ex_Color = in_Color;
} 
 