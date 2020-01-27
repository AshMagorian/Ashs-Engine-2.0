#include <myEngine/myEngine.h>
#include "Camera_FirstPerson.h"

#include <glm/ext.hpp>
#include <iostream>
/**
*\brief Sets the transform and input variables
*/
void Camera_FirstPerson::onBegin()
{
	m_Transform = getEntity()->GetTransform();
	m_Input = getApplication()->GetInput();
}
/**
*\brief Calculates the forward vector and then updates the position by checking if the wasd keys are pressed
*/
void Camera_FirstPerson::onTick()
{
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	  // Calculate the new Front vector
	m_forwardVector.x = cos(glm::radians(-m_yaw+90.0f)) * cos(glm::radians(-m_pitch));
	m_forwardVector.y = sin(glm::radians(-m_pitch));
	m_forwardVector.z = sin(glm::radians(-m_yaw+90.0f)) * cos(glm::radians(-m_pitch));
	m_forwardVector = glm::normalize(m_forwardVector);
	// Also re-calculate the Right and Up vector
	m_rightVector = glm::normalize(glm::cross(m_forwardVector, m_upVector));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_upVector = glm::normalize(glm::cross(m_rightVector, m_forwardVector));

	glm::vec3 flatForward = glm::normalize(glm::vec3(m_forwardVector.x, 0.0f, m_forwardVector.z));
	glm::vec3 flatRight = glm::normalize(glm::vec3(m_rightVector.x, 0.0f, m_rightVector.z));

	glm::vec3 totalMovement = glm::vec3(0.0f, 0.0f, 0.0f);
	bool movedFwd = false;
	bool movedHor = false;
	if (m_Input->IsKey('w'))
	{
		movedFwd = !movedFwd;
		totalMovement -= flatForward;
	}
	if (m_Input->IsKey('s'))
	{
		movedFwd = !movedFwd;
		totalMovement +=  flatForward;
	}
	if (m_Input->IsKey('a')) // rotate left
	{
		movedHor = !movedHor;
		totalMovement +=  flatRight;
	}
	if (m_Input->IsKey('d')) // rotate right
	{
		movedHor = !movedHor;
		totalMovement -= flatRight;
	} 
	if (movedFwd == true || movedHor == true)
	{
		m_Transform->SetPos(m_Transform->GetPos() + (m_movementSpeed * getApplication()->GetDeltaTime() * glm::normalize(totalMovement)));
	}
	m_yaw += m_Input->GetDeltaMouseX() * getApplication()->GetDeltaTime() * m_rotationSpeed;
	m_pitch += m_Input->GetDeltaMouseY() * getApplication()->GetDeltaTime() * m_rotationSpeed;
	if (m_pitch > 89.0f) { m_pitch = 89.0f; }
	if (m_pitch < -89.0f) { m_pitch = -89.0f; }
	
	m_Transform->SetRotation(glm::vec3(m_pitch,m_yaw ,0.0f ));
}