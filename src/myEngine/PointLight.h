#ifndef POINT_LIGHT
#define POINT_LIGHT

#include "Lights.h"
#include "Component.h"
#include "Entity.h"
#include "Application.h"

class PointLight : public Component
{
	friend class Lights;
private:
	glm::vec3 m_position;

	float m_constant;
	float m_linear;
	float m_quadratic;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

public:
	void onInit()
	{
		m_position = getEntity()->GetTransform()->GetPos();
		m_constant = 1.0f;
		m_linear = 0.22f;
		m_quadratic = 0.20f;
		m_ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		m_diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
		m_specular = glm::vec3(0.7f, 0.7f, 0.7f);

		getApplication()->GetLightManager()->AddPointLight(getEntity());
	}

	void onTick()
	{
		m_position = getEntity()->GetTransform()->GetPos();
	}
	void SetAttenuation(float _const, float _lin, float _quad) { m_constant = _const; m_linear = _lin; m_quadratic = _quad; }
	void SetLighting(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) { m_ambient = _ambient; m_diffuse = _diffuse; m_specular = _specular; }
	
	bool Clone(std::shared_ptr<Entity> _entity)
	{
		std::shared_ptr<PointLight> tmp = _entity->addComponent<PointLight>();

		tmp->SetAttenuation(m_constant, m_linear, m_quadratic);
		tmp->SetLighting(m_ambient, m_diffuse, m_specular);
		return true;
	}
};
#endif