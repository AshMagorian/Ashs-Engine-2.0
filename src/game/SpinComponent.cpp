#include "SpinComponent.h"

void SpinComponent::onBegin()
{
	m_transform = getEntity()->GetTransform();
}

void SpinComponent::onTick()
{
	
	m_transform->SetRotation(glm::vec3(m_transform->GetRotation().x, m_transform->GetRotation().y + (m_spinSpeed * getApplication()->GetDeltaTime()), m_transform->GetRotation().z));
	//m_transform->SetPos(glm::vec3(m_transform->GetPos().x, m_transform->GetPos().y, m_transform->GetPos().z + (m_spinSpeed * getApplication()->GetDeltaTime())));
} 