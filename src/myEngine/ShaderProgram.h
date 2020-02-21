#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>

#include "Resource.h"

class VertexArray;
class Texture;

struct Sampler
{
	GLint id;
	std::shared_ptr<Texture> texture;
};
/**
*The Shaderprogram is what calls the draw function to render objects onto the scene. They are composed
*of a vertex shader and a fragment shader
*/
class ShaderProgram : public Resource
{
private:
	GLuint id;
	std::vector<Sampler> samplers;

public:
	/**
	*\brief Reads the shader files
	*/
	ShaderProgram(std::string _path);
	/**
	*\brief draws the vertex array to the screen
	*/
	void Draw(std::shared_ptr<VertexArray> vertexArray);
	void DrawParticles(std::shared_ptr<VertexArray> vertexArray, int _maxParticles, int _particlesCount, std::vector<float> _positionData, std::vector<float> _colourData);
	/**
	*\brief Sets the uniform value to be passed into the shader files
	*/
	void SetUniform(std::string uniform, glm::vec2 value);
	void SetUniform(std::string uniform, glm::vec3 value);
	void SetUniform(std::string uniform, glm::vec4 value);
	void SetUniform(std::string uniform, float value);
	void SetUniform(std::string uniform, int value);
	void SetUniform(std::string uniform, glm::mat4 value);
	void SetUniform(std::string uniform, std::shared_ptr<Texture> texture);

	GLuint GetId() { return id; }

};


#endif