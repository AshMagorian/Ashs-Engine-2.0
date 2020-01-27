#ifndef MATERIAL_H
#define MATERIAL_H
#include "Texture.h"
#include "Resource.h"
class Material : public Resource
{
private:
	std::shared_ptr<Texture> m_diffuse;
	std::shared_ptr<Texture> m_specular;
	float m_shininess;

public:
	Material() {}
	
	void SetDiffuse(std::shared_ptr<Texture> _tex) { m_diffuse = _tex; }
	void SetSpecular(std::shared_ptr<Texture> _tex) { m_specular = _tex; }
	void SetShininess(float _value) { m_shininess = _value; }

	std::shared_ptr<Texture> GetDiffuse() { return m_diffuse; }
	std::shared_ptr<Texture> GetSpecular() { return m_specular; }
	float GetShininess() { return m_shininess; }
};
#endif