#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "Component.h"
#include <glm/glm.hpp>
/**
*\BoxCollider is a component which gives an entity a simple aabb collider which instantiates collision
*with other entities with the same component
*/
class BoxCollider : public Component
{
private:
	/**
	*\brief Constructor
	*/
	void CollideBox();

	glm::vec3 m_size; ///< A vec3 containing the width, height and depth of the box
	glm::vec3 m_offset; ///< The offset the position of the relative Entity
	glm::vec3 m_lastPosition; ///< The previous position of the collider box
	/**
	*\brief Checks if the box is colliding with the box defined in the parameter
	*/
	bool IsColliding(glm::vec3 _position, glm::vec3 _size);
	/**
	*\brief Moves the collided entity so that the collision boxes don't overlap
	*/
	glm::vec3 GetCollisionResponse(glm::vec3 _position, glm::vec3 _size);

public:
	/**
	*\brief Setup initial values of the data
	*/
	void onInit();
	/**
	*\brief Checks for collision every frame
	*/
	void onTick();
	/**
	*\brief Sets the offset value
	*/
	void SetOffset(glm::vec3 _offset) {m_offset = _offset;}
	/**
	*\brief Sets the size value
	*/
	void SetSize(glm::vec3 _size) {m_size = _size;}

	bool Clone(std::shared_ptr<Entity> _entity);
};
#endif