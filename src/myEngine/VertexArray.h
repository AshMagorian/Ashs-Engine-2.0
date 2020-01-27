#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Resource.h"
#include "VertexBuffer.h"

class VertexBuffer;
/**
*The vertex array is a resource which stores all of the data necessary to draw a model
*/
class VertexArray : public Resource
{
private:
	GLuint id;
	std::vector<std::shared_ptr<VertexBuffer>> buffers; ///< Stores the buffers (Which holds the model data)
	bool dirty;

	void SplitStringWhitespace(std::string& input, std::vector<std::string>& output);
	void SplitString(std::string& input, char splitter, std::vector<std::string>& output);

public:
	VertexArray();
	/**
	*\brief Reads the model file and stores the data
	*/
	VertexArray(std::string path);
	/**
	*\brief Stores a buffer into the vector. It's position in teh vector depends on it's attrbute which is passed through
	*/
	void SetBuffer(std::string attribute, std::shared_ptr<VertexBuffer> buffer);
	int GetVertexCount();
	GLuint GetId();

};
#endif