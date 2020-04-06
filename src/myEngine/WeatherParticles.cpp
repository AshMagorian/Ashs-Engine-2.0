#include <myEngine/myEngine.h>
#define GLM_ENABLE_EXPERIMENTAL

#include <cstdlib>
#include <ctime>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>

void WeatherParticles::onInit()
{
	m_startHeight = (m_startVelocity.y * m_particleLife)/-2.0f;

	m_particlesVA = std::make_shared<VertexArray>();
	m_particlesVA->MakeParticles(m_maxParticles);

	m_shaderProgram = getApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("weather_shader");
	m_shaderProgram->SetUniform("in_StartColor", m_startColour);
	m_shaderProgram->SetUniform("in_EndColor", m_endColour);
	m_shaderProgram->SetUniform("in_TotalLife", m_particleLife);

	//init all particles
	m_particlesContainer.resize(m_maxParticles);
	for (int i = 0; i< m_maxParticles; i++)
	{
		float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_particlesContainer[i].life = m_particleLife * rndm;
		m_particlesContainer[i].velocity = m_startVelocity;
		glm::vec3 planePosition = GetRandomPositionOnPlane(m_startVelocity);
		planePosition.y += m_startHeight;
		planePosition.y -= (m_startHeight * 2.0f) * (1.0f - rndm);
		m_particlesContainer[i].pos = planePosition;
		m_particlesContainer[i].size = 1.0f;
		//printf("%f", planePosition.y);
	}
}

void WeatherParticles::onTick()
{
	m_cameraPosition = getApplication()->GetCamera()->GetCurrentCamera()->GetTransform()->GetPos();
	//m_delta = getApplication()->GetDeltaTime();
	m_delta = 0.016f;
	m_shaderProgram->SetUniform("in_Projection", getApplication()->GetCamera()->GetProjectionMatrix());
	m_shaderProgram->SetUniform("in_View", getApplication()->GetCamera()->GetViewMatrix());
	m_shaderProgram->SetUniform("in_PlayerPos", m_cameraPosition);

	m_positionData.clear();
	m_velocityLifeData.clear();

	m_particlesCount = 0;
	for (int i = 0; i < m_maxParticles; i++)
	{
		Particle &p = m_particlesContainer[i];

		if (p.life > 0.0f)
		{
			p.life -= m_delta;

			p.velocity += m_force * m_delta;
			p.pos += p.velocity * m_delta;

			if (glm::sqrt(pow(p.pos.x - m_cameraPosition.x, 2) + pow(p.pos.z - m_cameraPosition.z, 2)) > m_spawnRadius)
			{
				p.pos -= m_cameraPosition;
				p.pos.x *= -0.9f;
				p.pos.z *= -0.9f;
				p.pos += m_cameraPosition;
			}

			p.cameraDistance = glm::length2(p.pos - m_cameraPosition);

			m_positionData.push_back(p.pos.x);
			m_positionData.push_back(p.pos.y);
			m_positionData.push_back(p.pos.z);
			m_positionData.push_back(p.size);

			m_velocityLifeData.push_back(p.velocity.x);
			m_velocityLifeData.push_back(p.velocity.y);
			m_velocityLifeData.push_back(p.velocity.z);
			m_velocityLifeData.push_back(p.life);

			m_particlesCount++;
		}
		else
		{
			//printf("%f", p.pos.y);
			CreateNewParticle(p);
			m_particlesCount++;
		}
	}
	std::sort(m_particlesContainer.begin(), m_particlesContainer.end(), [](Particle _a, Particle _b)
	{ return _a.cameraDistance > _b.cameraDistance; }
	);

}

void WeatherParticles::onDisplay()
{
	m_shaderProgram->DrawParticles(m_particlesVA, m_maxParticles, m_particlesCount, m_positionData, m_velocityLifeData);
}

void WeatherParticles::CreateNewParticle(Particle& _p)
{
	glm::vec3 planePosition = GetRandomPositionOnPlane(m_startVelocity) + m_cameraPosition;
	planePosition.y += m_startHeight;
	glm::vec3 direction = GetRandomSpreadDirection();

	_p.pos = planePosition;
	_p.velocity = m_startVelocity;
	_p.size = 1.0f;
	_p.life = m_particleLife;
}

glm::vec3 WeatherParticles::GetRandomPositionOnPlane(glm::vec3 _avgDirection)
{
	float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
	float rndm2 = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));

	//float radius = rndm * m_spawnRadius;
	//float angle = rndm2 * m_pi * 2.0f;
	//glm::vec3 planePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	//planePosition.x = glm::cos(angle) * radius;
	//planePosition.z = glm::sin(angle) * radius;
	
	float r = m_spawnRadius * sqrt(rndm);
	float theta = rndm2 * 2.0f * m_pi;

	glm::vec3 planePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	planePosition.x = glm::cos(theta) * r;
	planePosition.z = glm::sin(theta) * r;

	return planePosition;
}
glm::vec3 WeatherParticles::GetRandomSpreadDirection()
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

	glm::vec3 direction = glm::vec4(randomSpreadDirection, 1.0f);
	return direction;
}

void WeatherParticles::MakeTextureParticles(std::shared_ptr<Texture> _tex)
{
	m_shaderProgram = getApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("weather_tex_shader");
	m_particlesVA->SetParticleTexCoords();
	m_shaderProgram->SetUniform("in_Texture", _tex);
}
void WeatherParticles::MakeMaskedParticles(std::shared_ptr<Texture> _tex)
{
	m_shaderProgram = getApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("weather_mask_shader");
	m_particlesVA->SetParticleTexCoords();
	m_shaderProgram->SetUniform("in_Texture", _tex);
	m_shaderProgram->SetUniform("in_StartColor", m_startColour);
	m_shaderProgram->SetUniform("in_EndColor", m_endColour);
	m_shaderProgram->SetUniform("in_TotalLife", m_particleLife);
}