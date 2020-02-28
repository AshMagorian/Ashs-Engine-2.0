#include <myEngine/myEngine.h>

#include <cstdlib>
#include <ctime>

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
	//m_delta = 0.016f;
	//std::cout << "delta: " << m_delta << std::endl;

	m_shaderProgram->SetUniform("in_Projection", getApplication()->GetCamera()->GetProjectionMatrix());
	m_shaderProgram->SetUniform("in_View", getApplication()->GetCamera()->GetViewMatrix());

	// Generate 1 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newParticles = (int)(m_delta*200.0);
	if (newParticles > (int)(0.016f*200.0))
		newParticles = (int)(0.016f*200.0);

	//Creates the specified number of particles every frame
	for (int i = 0; i < newParticles; i++)
	{
		float rndm = ((static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) - 0.5) * 2;
		float rndm2 = ((static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) - 0.5) * 2;

		int j = FindUnusedParticle();

		m_particlesContainer[j].pos = getEntity()->GetTransform()->GetPos();
		m_particlesContainer[j].speed = glm::vec3(rndm * 3 ,7.0f, rndm2 * 3);

		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));

		m_particlesContainer[j].r = rndm;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_particlesContainer[j].g = rndm;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_particlesContainer[j].b = rndm;
		m_particlesContainer[j].a = 0.7f;

		m_particlesContainer[j].size = 0.2f;
		
		m_particlesContainer[j].life = 2.0f;
		
	}

	m_positionData.clear();
	m_colourData.clear();

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