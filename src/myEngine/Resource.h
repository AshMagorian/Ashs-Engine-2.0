#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>

/**
*An abstract class for resources loaded in by the user (Models, textures, shader programs and sound)
*/
class Resource
{
private:
	std::string m_path; ///< The directory path for the resource
public:
	/**
	*\brief This only exists as there needs to be a virtual functino to make a class abstract
	*/
	virtual ~Resource() {}
	/**
	*\brief Returns the path of the resource
	*/
	std::string GetPath() { return m_path; }
	/**
	*\brief Sets the path of the resource
	*/
	void SetPath(std::string _path) { m_path = _path; }

};
#endif