#version 430 core

uniform mat4 in_View;
uniform mat4 in_Projection;
uniform mat4 in_Model;

//uniform vec3 in_CentrePos;
//uniform vec2 in_Size;

attribute vec3 in_Position;
attribute vec2 in_TexCoord;

varying vec2 ex_TexCoord;

void main()
{
	vec3 CameraRight_worldspace = vec3(in_View[0][0], in_View[1][0], in_View[2][0]);
	vec3 CameraUp_worldspace = vec3(in_View[0][1], in_View[1][1], in_View[2][1]);

	//vec3 newPosition = 
	//vec3 (in_CentrePos.x, in_CentrePos.y, in_CentrePos.z) 
	//+ CameraRight_worldspace * in_Position.x * in_Size.x
	//+ CameraUp_worldspace * in_Position.y * in_Size.y;

	vec3 newPosition =  
	+ CameraRight_worldspace * in_Position.x 
	+ CameraUp_worldspace * in_Position.y;
	
	gl_Position = in_Projection * in_View * in_Model * vec4(newPosition, 1.0);
	ex_TexCoord = in_TexCoord;
}