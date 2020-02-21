uniform mat4 in_View;
uniform mat4 in_Projection;
uniform mat4 in_Model;

attribute vec3 in_Position;
attribute vec4 in_Particle_Position;
attribute vec4 in_Particle_Color;

varying vec4 ex_Particle_Color;

void main()
{
	vec3 CameraRight_worldspace = vec3(in_View[0][0], in_View[1][0], in_View[2][0]);
	vec3 CameraUp_worldspace = vec3(in_View[0][1], in_View[1][1], in_View[2][1]);
	vec3 newPosition = 
	vec3 (in_Particle_Position.x, in_Particle_Position.y, in_Particle_Position.z) 
	+ CameraRight_worldspace * in_Position.x * in_Particle_Position.w
	+ CameraUp_worldspace * in_Position.y * in_Particle_Position.w;

	gl_Position = in_Projection * in_View * in_Model * vec4(newPosition, 1.0);
	ex_Particle_Color = in_Particle_Color;
}