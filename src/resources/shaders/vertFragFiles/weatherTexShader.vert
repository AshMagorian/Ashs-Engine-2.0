#version 430 core

uniform mat4 in_View;
uniform mat4 in_Projection;

uniform vec4 in_StartColor;
uniform vec4 in_EndColor;
uniform float in_TotalLife;
uniform vec3 in_PlayerPos;

layout(location = 0) in vec3 in_Position;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 4) in vec4 in_Particle_Position;
layout(location = 5) in vec4 in_Particle_Velocity;

out vec2 ex_TexCoord;

void main()
{
	vec3 CamVector = vec3 (in_Particle_Position.x, in_Particle_Position.y, in_Particle_Position.z) - in_PlayerPos;
	vec3 CameraRight_worldspace = normalize(cross(CamVector, vec3(0.0, 1.0, 0.0)));
	vec3 CameraUp_worldspace = vec3(0.0, 1.0, 0.0);
	
	vec3 newPosition = 
	vec3 (in_Particle_Position.x, in_Particle_Position.y, in_Particle_Position.z)
	+ (CameraRight_worldspace * in_Position.x * in_Particle_Position.w)
	+ (CameraUp_worldspace * in_Position.y *  in_Particle_Position.w);

	gl_Position = in_Projection * in_View * vec4(newPosition, 1.0);

	ex_TexCoord = in_TexCoord;
}