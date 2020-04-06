#ifndef WEATHERPARTICLES_H
#define WEATHERPARTICLES_H

#include "Component.h"

struct Particle;

class ShaderProgram;
class VertexArray;
class Texture;

class WeatherParticles : public Component
{
private:
	std::vector<Particle> m_particlesContainer;

	float m_windStrength = 5.0f;
	bool m_isWind = false;

	float m_pi = 3.14159f;
	float m_delta = 0.0f;
	int m_particlesCount = 0;

	float m_particleLife = 3.0f;
	int m_maxParticles = 7000;
	float m_spreadAngle = 10.0f;
	float m_startHeight;
	float m_spawnRadius = 15.0f;
	glm::vec3 m_startVelocity = glm::vec3(0.0f, -30.0f, 0.0f);
	glm::vec3 m_force = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 m_cameraPosition;

	glm::vec4 m_startColour = glm::vec4(1.0f, 1.0f, 1.0f, 0.11f);
	glm::vec4 m_endColour = glm::vec4(1.0f, 1.0f, 1.0f, 0.11f);

	std::vector<float> m_positionData;
	std::vector<float> m_velocityLifeData;

	glm::vec3 GetRandomPositionOnPlane(glm::vec3 _avgDirection);
	glm::vec3 GetRandomSpreadDirection();
	void CreateNewParticle(Particle& _p);

	std::shared_ptr<ShaderProgram> m_shaderProgram;
	std::shared_ptr<VertexArray> m_particlesVA;
public:
	void onInit();
	void onTick();
	void onDisplay();


	void MakeTextureParticles(std::shared_ptr<Texture> _tex);
	void MakeMaskedParticles(std::shared_ptr<Texture> _tex);

	void SetWind(bool _x) { m_isWind = _x; }
};
#endif