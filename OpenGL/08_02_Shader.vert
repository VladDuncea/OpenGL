// Shader-ul de varfuri  
 
 #version 400

// pozitia este atribut standard
layout(location=0) in vec4 in_Position;

// culoarea este atribut instantiat
layout(location=1) in vec4 in_Color;
 
// matricea de transformare este atribut instantiat
layout (location = 3) in mat4 modelMatrix;

out vec4 gl_Position; 
out vec4 ex_Color;
 

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
 
void main(void)
  {
    gl_Position = projectionMatrix*viewMatrix*modelMatrix*in_Position;
	ex_Color=in_Color;
   } 
 