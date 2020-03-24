#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Component.h"


class ShaderProgram;
class VertexArray;

class Entity;

struct Particle
{
	glm::vec3 pos, velocity;
	float cameraDistance;
	float r, g, b, a;
	float size, angle, weight;
	float life = -1.0f;
};

class ParticleSystem : public Component
{
private:
	std::vector<Particle> m_particlesContainer;
	
	bool m_started = false;
	bool m_firstParticles = false;

	int m_lastUsedParticle = 0;
	int m_particlesCount = 0;
	float m_pi = 3.14159f;
	float m_delta = 0.0f;
	float m_newParticles = 0;

	int m_maxParticles = 600;
	float m_spreadAngle = 40.0f;
	float m_speed = 5.0f;
	glm::vec3 m_force = glm::vec3(0.0f, -1.0f, 0.0f);
	float m_particleLife = 5.0f;
	glm::vec2 m_positionOffset = glm::vec2(10.0f, 10.0f);
	int m_particlesPerSecond = 150;
	glm::vec3 m_offsetRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec4 m_startColour = glm::vec4(1.0f, 1.0f, 0.0f, 0.7f);
	glm::vec4 m_endColour = glm::vec4(1.0f, 0.0f, 1.0f, 0.7f);

	glm::quat m_quat;
	glm::mat4 m_localRotMatrix;
	glm::mat4 m_rotMatrix;

	std::vector<float> m_positionData;
	std::vector<float> m_colourData;

	int FindUnusedParticle();
	glm::mat4 GetLocalRotationMatrix(glm::mat4 _model);
	glm::vec3 GetRandomPositionOnPlane(glm::vec3 _avgDirection);
	glm::vec3 GetRandomSpreadDirection();

	std::shared_ptr<ShaderProgram> m_shaderProgram;
	std::shared_ptr<VertexArray> m_particlesVA;

public:
	void onInit(int _maxParticles);
	void onTick();
	void onDisplay();

	void SetSpreadAngle(float _angle) { m_spreadAngle = _angle; }
	void SetSpeed(float _speed) { m_speed = _speed; }
	void SetParticleLife(float _life) { m_particleLife = _life; }
	void SetPositionOffset(float _x, float _y) { m_positionOffset.x = _x; m_positionOffset.y = _y; }
	void SetParticlesPerSecond(int _particles) { m_particlesPerSecond = _particles; }
	void SetOffsetRotation(glm::vec3 _rot) { m_offsetRotation = _rot; }
	void SetColours(glm::vec4 _start, glm::vec4 _end) { m_startColour = _start; m_endColour = _end; }


};
#endif