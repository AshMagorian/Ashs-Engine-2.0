 #include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include "Entity.h"
#include "Exception.h"
Camera::Camera()
{
	m_viewAngle = 45.0f;
}

Camera::~Camera()
{

}
/**
*\brief Throws an exception if the camera wasn't initialised. Calculates the new matrix values
*/
void Camera::UpdateMatrix(int _w, int _h)
{
	if (!m_currentCamera)
	{
		throw Exception("Camera not found");
	}
	m_projectionMatrix = glm::perspective(glm::radians(m_viewAngle),
		(float) _w / (float) _h, 0.1f, 100.f);
	m_viewMatrix = glm::inverse(m_currentCamera->GetTransform()->GetModelMatrix());

}