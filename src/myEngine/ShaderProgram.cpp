#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Exception.h"

#include <glm/ext.hpp>
#include <iostream>


ShaderProgram::ShaderProgram()
{
	id = glCreateProgram();
}

/**
*The file passed through contains the paths for both shader files. This is because to make 
*Shaderprogram a resource it needed to take only one paramater as opposed to two
*/
ShaderProgram::ShaderProgram(std::string _path)
{
	std::string vertPath;
	std::string fragPath;

	std::ifstream file(_path);
	
	if (!file.is_open())
	{
		throw Exception("Shader file not found, '" + _path + "' cannot be loaded");
	}
	else
	{
		std::string line;
		std::getline(file, line);
		vertPath += line;
		std::getline(file, line);
		fragPath += line;
	}
	file.close();

	id = glCreateProgram();

	GLuint vertexShaderId = AttachVetexShader(vertPath);
	GLuint fragmentShaderId = AttachFragmentShader(fragPath);

	glBindAttribLocation(id, 0, "in_Position");
	glBindAttribLocation(id, 1, "in_Color");
	glBindAttribLocation(id, 2, "in_TexCoord");
	glBindAttribLocation(id, 3, "in_Normal");
	glBindAttribLocation(id, 4, "in_Particles_Position");
	glBindAttribLocation(id, 5, "in_Particles_Color");

	// Perform the link and check for faliure
	glLinkProgram(id);
	GLint success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw Exception("Shader cannot be created");
	}
	glUseProgram(id);


	glDetachShader(id, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(id, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
}

GLuint ShaderProgram::AttachVetexShader(std::string _path)
{
	std::string vertShader;
	std::ifstream file(_path);
	if (!file.is_open())
	{
		throw Exception("Vertex shader not found, '" + _path + "' cannot be loaded");
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			vertShader += line + "\n";
		}
	}
	file.close();

	const char *vertex = vertShader.c_str();
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertex, NULL);
	glCompileShader(vertexShaderId);
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorlog(maxLength);
		glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorlog[0]);
		//std::cout << &errorlog.at(0) << std::endl;
		throw Exception("Vertex shader compile error: " + (std::string)&errorlog.at(0));
	}
	glAttachShader(id, vertexShaderId);
	return vertexShaderId;
}
GLuint ShaderProgram::AttachFragmentShader(std::string _path)
{
	std::string fragShader;
	std::ifstream file(_path);

	if (!file.is_open())
	{
		throw Exception("Fragment shader not found, '" + _path + "' cannot be loaded");
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			fragShader += line + "\n";
		}
	}
	file.close();

	const char *fragment = fragShader.c_str();
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragment, NULL);
	glCompileShader(fragmentShaderId);
	GLint success = 0;
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorlog(maxLength);
		glGetShaderInfoLog(fragmentShaderId, maxLength, &maxLength, &errorlog[0]);
		//std::cout << &errorlog.at(0) << std::endl;
		throw Exception("Fragment shader compile error: " + (std::string)&errorlog.at(0));
	}
	glAttachShader(id, fragmentShaderId);
	return fragmentShaderId;
}

void ShaderProgram::MakeTransformFeedbackShader(std::string _vert, std::string _frag, std::string _varyings[])
{
	GLuint vertexShaderId = AttachVetexShader(_vert);
	GLuint fragmentShaderId = AttachFragmentShader(_frag);

}

/**
*\brief Draws the passed vertex array to the screen
*/
void ShaderProgram::Draw(std::shared_ptr<VertexArray> vertexArray)
{
	// Instruct OpenGL to use our shader program and our VAO
	glUseProgram(id);
	glBindVertexArray(vertexArray->GetId());

	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
	
		if (samplers.at(i).texture)
		{
			glBindTexture(GL_TEXTURE_2D, samplers.at(i).texture->getId());
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexCount());
	
	//Unbinds the textures for all samplers
	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Reset the state
	glBindVertexArray(0);
	glUseProgram(0);
}

void ShaderProgram::DrawParticles(std::shared_ptr<VertexArray> vertexArray, int _maxParticles, int _particlesCount, std::vector<float> _positionData, std::vector<float> _colourData)
{
	// Instruct OpenGL to use our shader program and our VAO
	glUseProgram(id);

	glDisable(GL_CULL_FACE);

	glBindVertexArray(vertexArray->GetParticlesId(_maxParticles, _particlesCount, _positionData, _colourData));

	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		if (samplers.at(i).texture)
		{
			glBindTexture(GL_TEXTURE_2D, samplers.at(i).texture->getId());
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _particlesCount);

	//Unbinds the textures for all samplers
	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glEnable(GL_CULL_FACE);

	// Reset the state
	glBindVertexArray(0);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec2 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform2f(uniformId, value.x, value.y);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec3 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform3f(uniformId, value.x, value.y, value.z);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, glm::vec4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform4f(uniformId, value.x, value.y, value.z, value.w);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, float value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform1f(uniformId, value);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, int value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform1i(uniformId, value);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, glm::mat4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(value));
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, std::shared_ptr<Texture> texture)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}
	//Tries to find the correct sampler in the vector
	for (size_t i = 0; i < samplers.size(); i++)
	{
		if (samplers.at(i).id == uniformId)
		{
			samplers.at(i).texture = texture;

			glUseProgram(id);
			glUniform1i(uniformId, i);
			glUseProgram(0);
			return;
		}
	}
	//If the sampler isn't found then a new sampler is made and pushed back
	Sampler s;
	s.id = uniformId;
	s.texture = texture;
	samplers.push_back(s);

	glUseProgram(id);
	glUniform1i(uniformId, samplers.size() - 1);
	glUseProgram(0);
}