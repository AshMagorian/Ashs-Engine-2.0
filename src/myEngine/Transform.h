#include "Component.h"
#include "glm/glm.hpp"

/**
*The transform component is included in every entity. About 90% of entities will need to access transform and there
*is negligible computational cost in the remaining 10% of entities accessing it. It stores all of the necessary data
*to create the model matrix
*/
class Transform : public Component
{
private:

	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f); ///< The position of the entity
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);///< The rotation of the entity
	glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f); ///< The scale of the entity

public:

	Transform() {}
	~Transform() {}
	/**
	*\brief Returns the position
	*/
	glm::vec3 GetPos() { return m_position; }
	/**
	*\brief Returns the rotation (which is capped at 360 degrees)
	*/
	glm::vec3 GetRotation() 
	{
		if (m_rotation.x < 360.0f && m_rotation.x > -360.0f && m_rotation.y < 360.0f && m_rotation.y > -360.0f && m_rotation.z < 360.0f && m_rotation.z > -360.0f)
		{
			return m_rotation;
		}
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
	/**
	*\brief Returns the scale
	*/
	glm::vec3 GetScale() { return m_scale; }
	/**
	*\brief Sets the position
	*/
	void SetPos(glm::vec3 _pos) { m_position = _pos; }
	/**
	*\brief Sets the rotation
	*/
	void SetRotation(glm::vec3 _rot) { m_rotation = _rot; }
	/**
	*\brief Sets the scale
	*/
	void SetScale(glm::vec3 _sca) { m_scale = _sca; }
	/**
	*\brief Calculates and returns the model matrix
	*/
	glm::mat4 GetModelMatrix();

	bool Clone(std::shared_ptr<Entity> _entity);

protected:
	glm::mat4 m_model; ///< The model matrix
};