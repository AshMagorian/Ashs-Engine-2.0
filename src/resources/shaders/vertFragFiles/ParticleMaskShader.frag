#version 430 core

uniform sampler2D in_Texture;

in vec2 ex_TexCoord;
in vec4 ex_Particle_Color;

void main()
{
	vec4 tex = texture2D(in_Texture, ex_TexCoord);
    	if(tex.a < 0.1) {discard;}
		else {tex = ex_Particle_Color;}
	gl_FragColor = tex;
}