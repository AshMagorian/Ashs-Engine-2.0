varying vec4 ex_Particle_Color;

void main()
{
	if(ex_Particle_Color.a < 0.1) {discard;}
	gl_FragColor = ex_Particle_Color;
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}