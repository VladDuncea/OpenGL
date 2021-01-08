
// Shader-ul de fragment / Fragment shader  
 
 #version 400

in vec4 ex_Color;
in vec4 ex_Pos;
in vec2 tex_Coord;

uniform int codCol;
uniform int random;

out vec4 out_Color;
 
uniform sampler2D myTexture;


//Am folosit functia clamp (1)
//Am folosit functia exp (2)
//Am folosit functia sin/fract (3)

float rand (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}


void main(void)
{
	if ( codCol==0 )
	{
		//out_Color = mix(texture(myTexture, tex_Coord), ex_Color, 0.2);
		out_Color = ex_Color;
	}
		
	if ( codCol==1 )
		out_Color=vec4 (0.0, 0.0, 0.0, 0.0);
	if( codCol == 2)
		out_Color = clamp(mix(texture(myTexture, tex_Coord), ex_Color, 0.2),0.1,0.5);
	
	if(random == 1)
	{
		out_Color = ex_Color;
		out_Color[0] = exp(out_Color[0]);
		out_Color[1] = rand(vec2(ex_Pos[0],ex_Pos[1]));
		out_Color[2] = rand(vec2(ex_Pos[1],ex_Pos[2]));
	}
		

}
 