#include <myEngine/myEngine.h>
#define GLM_ENABLE_EXPERIMENTAL

#include <cstdlib>
#include <ctime>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>

// http://ogldev.atspace.co.uk/www/tutorial28/tutorial28.html

void ParticleSystem::onInit(int _maxParticles)
{
	m_maxParticles = _maxParticles;
	m_particlesContainer.resize(m_maxParticles);

	m_particlesVA = std::make_shared<VertexArray>();
	m_particlesVA->MakeParticles(m_maxParticles);

	m_shaderProgram = getApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("particle_shader");

	//create the quat

	float yaw = glm::radians(m_offsetRotation.y);
	float pitch = glm::radians(m_offsetRotation.x);
	float roll = glm::radians(m_offsetRotation.z);
	
	float cy = cos(yaw * 0.5f);
	float sy = sin(yaw * 0.5f);
	float cp = cos(pitch * 0.5f);
	float sp = sin(pitch * 0.5f);
	float cr = cos(roll * 0.5f);
	float sr = sin(roll * 0.5f);

	m_quat.w = cy * cp * cr + sy * sp * sr;
	m_quat.x = cy * cp * sr - sy * sp * cr;
	m_quat.y = sy * cp * sr + cy * sp * cr;
	m_quat.z = sy * cp * cr - cy * sp * sr;
}

void ParticleSystem::onTick()
{
	

	m_delta = getApplication()->GetDeltaTime();
	m_rotMatrix = getEntity()->GetTransform()->GetRotationMatrix();
	m_localRotMatrix = GetLocalRotationMatrix(m_localRotMatrix);

	m_shaderProgram->SetUniform("in_Projection", getApplication()->GetCamera()->GetProjectionMatrix());
	m_shaderProgram->SetUniform("in_View", getApplication()->GetCamera()->GetViewMatrix());

	int newParticles = 0;
	if (m_delta * m_particlesPerSecond > 0.016f* m_particlesPerSecond)
		m_newParticles += 0.016f* m_particlesPerSecond;
	else
		m_newParticles += m_delta * m_particlesPerSecond;

	if (m_newParticles > 1.0f)
	{
		if (m_started == false) 
		{
			m_started = true; m_firstParticles = true;
		}
		newParticles = (int)(m_newParticles);
		m_newParticles = 0.0f;
	}
	if (m_started == false) { return; }

	m_firstParticles = false;



	glm::vec4 averageDirection = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) * m_rotMatrix * m_localRotMatrix;

	//Creates the specified number of particles every frame
	for (int i = 0; i < newParticles; i++)
	{
		int j = FindUnusedParticle();

		glm::vec3 planePosition = GetRandomPositionOnPlane(averageDirection);
		glm::vec3 direction = GetRandomSpreadDirection();

		m_particlesContainer[j].pos = planePosition;
		m_particlesContainer[j].velocity = m_speed * direction;

		//float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		//m_particlesContainer[j].r = rndm;
		//rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		//m_particlesContainer[j].g = rndm;
		//rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		//m_particlesContainer[j].b = rndm;
		//m_particlesContainer[j].a = 0.7f;

		m_particlesContainer[j].r = m_startColour.x;
		m_particlesContainer[j].g = m_startColour.y;
		m_particlesContainer[j].b = m_startColour.z;
		m_particlesContainer[j].a = m_startColour.w;

		m_particlesContainer[j].size = 0.3f;
		
		m_particlesContainer[j].life = m_particleLife;
		
	}

	m_positionData.clear();
	m_colourData.clear();

	glm::vec3 cameraPosition = getApplication()->GetCamera()->GetCurrentCamera()->GetTransform()->GetPos();

	m_particlesCount = 0;
	for (int i = 0; i < m_maxParticles; i++)
	{
		Particle &p = m_particlesContainer[i];

		if (p.life > 0.0f)
		{
			p.life -= m_delta;
			//p.life -= 0.01f;
			if (p.life > 0.0f)
			{
				p.velocity += m_force *m_delta;
				p.pos += p.velocity * m_delta;
				p.cameraDistance = glm::length2(p.pos - cameraPosition);

				float x = 1.0f - (p.life / m_particleLife);

				p.r = m_startColour.x + ((m_endColour.x - m_startColour.x) * x);
				p.g = m_startColour.y + ((m_endColour.y - m_startColour.y) * x);
				p.b = m_startColour.z + ((m_endColour.z - m_startColour.z) * x);
				p.a = m_startColour.w + ((m_endColour.w - m_startColour.w) * x);

				m_positionData.push_back(p.pos.x);
				m_positionData.push_back(p.pos.y);
				m_positionData.push_back(p.pos.z);
				m_positionData.push_back(p.size);
				
				m_colourData.push_back(p.r);
				m_colourData.push_back(p.g);
				m_colourData.push_back(p.b);
				m_colourData.push_back(p.a);
			}
			else
			{
				p.a = 0.0f;
			}
			m_particlesCount++;
		}
	}
	std::sort(m_particlesContainer.begin(), m_particlesContainer.end(), [](Particle _a, Particle _b)
	{ return _a.cameraDistance > _b.cameraDistance; }
	);
}

void ParticleSystem::onDisplay()
{
	if (m_started == false) { return; }
	m_shaderProgram->DrawParticles(m_particlesVA, m_maxParticles, m_particlesCount, m_positionData, m_colourData);
}

int ParticleSystem::FindUnusedParticle()
{
	for (int i = m_lastUsedParticle; i < m_maxParticles; i++) 
	{
		if (m_particlesContainer[i].life < 0) {
			m_lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < m_lastUsedParticle; i++) 
	{
		if (m_particlesContainer[i].life < 0) {
			m_lastUsedParticle = i;
			return i;
		}
	}
	m_lastUsedParticle = 0;
	return 0; // All particles are taken, override the first one
}

glm::mat4 ParticleSystem::GetLocalRotationMatrix(glm::mat4 _model)
{
	_model = glm::mat4(1.0f);
	_model = glm::rotate(_model, glm::radians(m_offsetRotation.y), glm::vec3(0, 1, 0));
	_model = glm::rotate(_model, glm::radians(m_offsetRotation.x), glm::vec3(1, 0, 0));
	_model = glm::rotate(_model, glm::radians(m_offsetRotation.z), glm::vec3(0, 0, 1));
	return _model;
}

glm::vec3 ParticleSystem::GetRandomPositionOnPlane(glm::vec3 _avgDirection)
{
	float rndm = ((static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) - 0.5f) * 2.0f;
	float rndm2 = ((static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) - 0.5f) * 2.0f;

	glm::vec3 planePosition = glm::vec3(m_positionOffset.x * rndm, 0.0f, m_positionOffset.y * rndm2);

	glm::vec3 upVector;
	if (_avgDirection.y < 0.98f)
	{
		upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else { upVector = glm::vec3(1.0f, 0.0f, 0.0f); }
	glm::vec3 rightVector = glm::normalize(glm::cross(glm::vec3(_avgDirection.x, _avgDirection.y, _avgDirection.z), upVector));
	upVector = glm::normalize(glm::cross(rightVector, glm::vec3(_avgDirection.x, _avgDirection.y, _avgDirection.z)));

	planePosition = getEntity()->GetTransform()->GetPos() + (upVector * planePosition.z) + (rightVector * planePosition.x);
	return planePosition;
}

glm::vec3 ParticleSystem::GetRandomSpreadDirection()
{
	float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
	float rndm2 = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));

	float angle = rndm * m_spreadAngle;
	float x = cos(abs((m_pi / 2.0f) - glm::radians(angle)));
	float y = sin(abs((m_pi / 2.0f) - glm::radians(angle)));
	angle = rndm2 * m_pi * 2.0f;
	float x2 = cos(angle) * x;
	float y2 = sin(angle) * x;
	glm::vec3 randomSpreadDirection = glm::vec3(x2, y, y2);

	//p = glm::quat(randomSpreadDirection.x, randomSpreadDirection.y, randomSpreadDirection.z, 0.0f);
	//p1 = m_quat * p * glm::conjugate(m_quat);
	//direction = glm::vec3(p1.x, p1.y, p1.z); 
	//direction = glm::vec4(direction, 1.0f) * m_rotMatrix;

	glm::vec3 direction = glm::vec4(randomSpreadDirection, 1.0f) * m_rotMatrix * m_localRotMatrix;
	return direction;
}