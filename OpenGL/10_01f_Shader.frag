 
// Shader-ul de fragment / Fragment shader  
 
 #version 400

//in vec4 ex_Color;
in vec3 FragPos;  
in vec3 Normal; 
in vec3 inLightPos;
in vec3 inViewPos;


out vec4 out_Color;

uniform vec3 objectColor;
uniform vec3 lightColor;
 




void main(void)
  {
  	// Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
     vec3 lightDir = normalize(inLightPos - FragPos);
    // vec3 dir=vec3(0.0,-150.0,200.0); // sursa directionala
    // vec3 lightDir=normalize(dir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(inViewPos - FragPos);//vector catre observator normalizat (V)
    vec3 reflectDir = reflect(-lightDir, norm); // reflexia razei de lumina (R)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 100);
    vec3 specular = specularStrength * spec * lightColor;  
    vec3 emission=vec3(0.0, 0.0, 0.0);
   // vec3 emission=vec3(1.0,0.8,1.0);
    vec3 result = emission+(ambient + diffuse + specular) * objectColor;
	out_Color = vec4(result, 1.0f);
  }
 