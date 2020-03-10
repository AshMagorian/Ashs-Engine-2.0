#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	VBcomponents = 0;
	dirty = false;

	glGenBuffers(1, &id);

	if (!id)
	{
		throw std::exception();
	}
}

void VertexBuffer::ParticleBufferInit(int _maxParticles)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::add(glm::vec2 value)
{
	if (VBcomponents != 2 && VBcomponents != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	VBcomponents = 2;
	dirty = true;
}

void VertexBuffer::add(glm::vec3 value)
{
	if (VBcomponents != 3 && VBcomponents != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);
	VBcomponents = 3;
	dirty = true;
}

void VertexBuffer::add(glm::vec4 value)
{
	if (VBcomponents != 4 && VBcomponents != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);
	data.push_back(value.w);
	VBcomponents = 4;
	dirty = true;
}

GLuint VertexBuffer::GetId()
{
	if (dirty)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.at(0), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		dirty = false;
	}

	return id;
}

GLuint VertexBuffer::GetParticleBufferId(int _maxParticles, int _particlesCount,  std::vector<float> _data)
{
	if (_particlesCount > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning
		glBufferSubData(GL_ARRAY_BUFFER, 0, _particlesCount * sizeof(GLfloat) * 4, &_data.at(0));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		VBcomponents = 4;
	}
	return id;
}