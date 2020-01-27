#include "Component.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
/**
*This camera class stores the vital data of the main camera of the scene. It stores a pointer to a camera defined
*in the game itself. 
*/
class Camera 
{
private:

	float m_viewAngle; ///< The viewing angle of the camera

	glm::mat4 m_viewMatrix; ///< The view matrix of the main camera
	glm::mat4 m_projectionMatrix; ///< The projection matrix of the main camera

	std::shared_ptr<Entity> m_currentCamera; ///< A pointer to the main camera set from the main game

public:
	Camera();
	~Camera();
	/**
	*\brief Updates the 2 matrix values
	*/
	void UpdateMatrix(int _w, int _h);
	/**
	*\brief Sets the new view angle
	*/
	void SetViewAngle(float _angle) { m_viewAngle = _angle; }
	/**
	*\brief Sets which camera to render from
	*/
	void SetCurrentCamera(std::shared_ptr<Entity> _cam) { m_currentCamera = _cam; }
	/**
	*\brief Gets the main camera
	*/
	std::shared_ptr<Entity> GetCurrentCamera() { return m_currentCamera; }
	/**
	*\brief Returns the view matrix
	*/
	glm::mat4 GetViewMatrix() { return m_viewMatrix; }
	/**
	*\brief Returns the projection matrix
	*/
	glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }
};