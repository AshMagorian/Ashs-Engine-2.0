#version 430 core

in vec4 ex_Particle_Color;

void main()
{
	if(ex_Particle_Color.a < 0.1) {discard;}
	gl_FragColor = ex_Particle_Color;
}