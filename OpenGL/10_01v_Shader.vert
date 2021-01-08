// Shader-ul de varfuri  
 
 #version 400


layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
 

out vec4 gl_Position; 
out vec4 ex_Color;


uniform mat4 myMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
 

void main(void)
  {
    // aplicare transformari si determinare pozitii
    gl_Position = projection*view*myMatrix*vec4(in_Position, 1.0);
    vec3 Normal=mat3(projection*view*myMatrix)*in_Normal; 
    vec3 inLightPos= vec3(projection*view*myMatrix* vec4(lightPos, 1.0f));
    vec3 inViewPos=vec3(projection*view*myMatrix* vec4(viewPos, 1.0f));
    vec3 FragPos = vec3(gl_Position);


      	// Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(inLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(inViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
	ex_Color = vec4(result, 1.0f);
 
   } 
 