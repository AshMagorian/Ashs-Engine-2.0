#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Component.h"

class ShaderProgram;
class VertexArray;

struct Particle
{
	glm::vec3 pos, speed;
	float r, g, b, a;
	float size, angle, weight;
	float life;

};

class ParticleSystem : public Component
{
private:
	int m_maxParticles;
	std::vector<Particle> m_particlesContainer;
	int m_lastUsedParticle = 0;
	int m_particlesCount;
	float m_delta;

	std::vector<float> m_positionData;
	std::vector<float> m_colourData;

	int FindUnusedParticle();

	std::shared_ptr<ShaderProgram> m_shaderProgram;
	std::shared_ptr<VertexArray> m_particlesVA;

public:
	void onInit(int _maxParticles);
	void onTick();
	void onDisplay();
};
#endif