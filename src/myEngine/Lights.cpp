#include "Lights.h"

#include "myEngine.h"

Lights::Lights()
{
	m_directionalLight->ambient = glm::vec3(0.15f, 0.15f, 0.15f);
	m_directionalLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	m_directionalLight->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	m_directionalLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
}
void Lights::UpdateLightShaderValues()
{
	for (std::list<std::shared_ptr<ShaderProgram>>::iterator i = m_shaderPrograms.begin(); i != m_shaderPrograms.end(); ++i)
	{
		(*i)->SetUniform("in_ViewPos", m_application.lock()->GetCamera()->GetCurrentCamera()->GetTransform()->GetPos());

		(*i)->SetUniform("in_DirLight.direction",m_directionalLight->direction );
		(*i)->SetUniform("in_DirLight.ambient",m_directionalLight->ambient );
		(*i)->SetUniform("in_DirLight.diffuse",m_directionalLight->diffuse );
		(*i)->SetUniform("in_DirLight.specular",m_directionalLight->specular );
	}
}