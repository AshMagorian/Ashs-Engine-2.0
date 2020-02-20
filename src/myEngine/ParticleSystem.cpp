#include <myEngine/myEngine.h>

void ParticleSystem::onInit(int _maxParticles)
{
	m_maxParticles = _maxParticles;
	m_particlesContainer.resize(m_maxParticles);

	m_particlesVA = std::make_shared<VertexArray>();
	m_particlesVA->MakeParticles(m_maxParticles);

	m_shaderProgram = getApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("particle_shader");
}

void ParticleSystem::onTick()
{
	m_delta = getApplication()->GetDeltaTime();
	// Generate 1 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newParticles = (int)(m_delta*10000.0);
	if (newParticles > (int)(0.016f*1000.0))
		newParticles = (int)(0.016f*1000.0);

	//Creates the specified number of particles every frame
	for (int i = 0; i < newParticles; i++)
	{
		int j = FindUnusedParticle();
		m_particlesContainer[j].pos = getEntity()->GetTransform()->GetPos();
		m_particlesContainer[j].speed = glm::vec3(0.0f,10.0f, 0.0f);

		m_particlesContainer[j].r = 1.0f;
		m_particlesContainer[j].g = 0.0f;
		m_particlesContainer[j].b = 0.0f;
		m_particlesContainer[j].a = 1.0f;

		m_particlesContainer[j].size = 0.3f;
		
		m_particlesContainer[j].life = 2.0f;
	}

	m_positionData.clear();
	m_colourData.clear();
	m_particlesCount = 0;
	for (int i = 0; i < m_maxParticles; i++)
	{
		Particle p = m_particlesContainer[i];

		if (p.life > 0.0f)
		{
			p.life -= m_delta;
			if (p.life > 0.0f)
			{
				p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * m_delta * 0.5f;
				p.pos += p.speed * m_delta;
				//p.cameradistance = glm::length2(p.pos - CameraPosition);

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
}

void ParticleSystem::onDisplay()
{
	m_shaderProgram->SetUniform("in_Model", getEntity()->GetTransform()->GetModelMatrix());
	m_shaderProgram->DrawParticles(m_particlesVA, m_maxParticles, m_particlesCount, m_positionData, m_colourData);
}

int ParticleSystem::FindUnusedParticle()
{
	for (int i = m_lastUsedParticle; i < m_maxParticles; i++) {
		if (m_particlesContainer[i].life < 0) {
			m_lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < m_lastUsedParticle; i++) {
		if (m_particlesContainer[i].life < 0) {
			m_lastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}