#include "SpinComponent.h"

void SpinComponent::onBegin()
{
	m_transform = getEntity()->GetTransform();
}

void SpinComponent::onTick()
{
	m_transform->SetRotation(glm::vec3(0.0f, m_transform->GetRotation().y +(m_spinSpeed * getApplication()->GetDeltaTime()) , 0.0f));
} 