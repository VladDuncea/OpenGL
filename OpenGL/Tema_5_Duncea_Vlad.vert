// Shader-ul de varfuri  
 
 #version 400


layout(location=0) in vec4 in_Position;
layout(location=1) in vec3 in_Color;
layout(location=2) in vec3 in_Normal;
layout(location=3) in vec2 texCoord;

out vec4 gl_Position; 
out vec4 ex_Color;
out vec4 ex_Pos;
out vec2 tex_Coord;

uniform mat4 matrUmbra;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modify;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 culoare;
uniform int codCol;
uniform int culoareCustom;
uniform int instanced;

void main(void)
{
    vec4 position = in_Position;
    //Mutam instantele dupa indice
    if(instanced == 1)
    {
        position[0] += (gl_InstanceID * 100.0);
        position[1] += (gl_InstanceID * 100.0);
    }

    // aplicare transformari si determinare pozitii
    //  gl_Position = projection*view*in_Position;
    if ( codCol==0 || codCol == 2)
    {
        gl_Position = projection*view*modify*position;
	   
	    ex_Pos = gl_Position;
        vec3 Normal=mat3(projection*view)*in_Normal; 
        vec3 inLightPos= vec3(projection*view* vec4(lightPos, 1.0f));
        vec3 inViewPos=vec3(projection*view*vec4(viewPos, 1.0f));
        vec3 FragPos = vec3(gl_Position);


        // Ambient
        float ambientStrength = 0.3f;
        vec3 ambient = ambientStrength * lightColor;
  	
        // Diffuse 
        vec3 norm = normalize(Normal);
        //vec3 lightDir = normalize(inLightPos - FragPos);
        vec3 lightDir = normalize(-inLightPos); // pentru sursa directionala
        float diff = max(dot(norm, lightDir), 0.0);
        //vec3 diffuse = diff * lightColor;
        vec3 diffuse = min(pow(diff,1) * lightColor, 0.4); // varianta de atenuare
    
            // Specular
        float specularStrength = 0.1f;
        vec3 viewDir = normalize(inViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
        vec3 specular = specularStrength * spec * lightColor;  
        
        vec3 color = in_Color;
        if(culoareCustom == 1)
        {
             color = culoare;
        }
           

        vec3 result = (ambient + diffuse + specular) * color;
	    ex_Color = vec4(result, 1.0f);
    }
    if ( codCol==1 )
    {
        gl_Position = projection*view*matrUmbra*modify*position;
	    ex_Pos = gl_Position;
    }
	    
    
    tex_Coord = vec2(texCoord.x, 1-texCoord.y);
 
} 
 