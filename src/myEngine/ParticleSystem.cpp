#include <myEngine/myEngine.h>

void ParticleSystem::onInit(int _maxParticles)
{
	m_maxParticles = _maxParticles;
	m_particlesContainer.resize(m_maxParticles);

	m_particlesVA = std::make_shared<VertexArray>();
	m_particlesVA->MakeParticles(m_maxParticles);

	m_shaderProgram = getApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("particle_shader");

	test = getApplication()->addEntity();
	test->addComponent<SpriteRenderer>(getApplication()->GetResourceManager()->LoadFromResources<Texture>("yellow_diffuse"), true);
	test->GetTransform()->SetPos(glm::vec3(-2.0f, 0.3f, 0.0f));

	test2 = getApplication()->addEntity();
	test2->addComponent<SpriteRenderer>(getApplication()->GetResourceManager()->LoadFromResources<Texture>("yellow_diffuse"), true);
	test2->GetTransform()->SetPos(glm::vec3(-2.0f, 0.3f, 0.0f));

	test3 = getApplication()->addEntity();
	test3->addComponent<SpriteRenderer>(getApplication()->GetResourceManager()->LoadFromResources<Texture>("yellow_diffuse"), true);
	test3->GetTransform()->SetPos(glm::vec3(-2.0f, 0.3f, 0.0f));
}

void ParticleSystem::onTick()
{
	//m_delta = getApplication()->GetDeltaTime();
	m_delta = 0.016f;
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
		int j = FindUnusedParticle();
		//std::cout << j << std::endl;
		m_particlesContainer[j].pos = getEntity()->GetTransform()->GetPos();
		m_particlesContainer[j].speed = glm::vec3(0.0f,5.0f, 0.0f);

		m_particlesContainer[j].r = 0.0f;
		m_particlesContainer[j].g = 1.0f;
		m_particlesContainer[j].b = 0.0f;
		m_particlesContainer[j].a = 1.0f;

		m_particlesContainer[j].size = 0.3f;
		
		m_particlesContainer[j].life = 2.0f;
		
	}

	m_positionData.clear();
	m_colourData.clear();

	//m_positionData.resize(m_maxParticles * 4);
	//m_colourData.resize(m_maxParticles * 4);


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

				//m_positionData[4 * m_particlesCount + 0] = p.pos.x;
				//m_positionData[4 * m_particlesCount + 1] = p.pos.y;
				//m_positionData[4 * m_particlesCount + 2] = p.pos.z;
				//				  
				//m_positionData[4 * m_particlesCount + 3] = p.size;
				// 
				//m_colourData[4 * m_particlesCount + 0] = p.r;
				//m_colourData[4 * m_particlesCount + 1] = p.g;
				//m_colourData[4 * m_particlesCount + 2] = p.b;
				//m_colourData[4 * m_particlesCount + 3] = p.a;

			}
			else
			{
				p.a = 0.0f;
			}
			m_particlesCount++;
		}
	}
	if (m_particlesCount > 50)
	{
		test->GetTransform()->SetPos(m_particlesContainer[50].pos);
	}
	if (m_particlesCount > 150)
	{
		test2->GetTransform()->SetPos(m_particlesContainer[150].pos);
	}
	if (m_particlesCount > 250)
	{
		test3->GetTransform()->SetPos(m_particlesContainer[250].pos);
	}

	std::cout << "particle: " << m_particlesContainer[150].pos.x << ", " << m_particlesContainer[150].pos.y << ", " << m_particlesContainer[150].pos.z << std::endl;
	std::cout << "test: " << test2->GetTransform()->GetPos().x << ", " << test2->GetTransform()->GetPos().y << ", " << test2->GetTransform()->GetPos().z << std::endl;
	//std::cout << m_particlesCount << std::endl;
}

void ParticleSystem::onDisplay()
{
	m_shaderProgram->SetUniform("in_Model", getEntity()->GetTransform()->GetModelMatrix());
	
	//std::cout << m_positionData[4] << ", " << m_positionData[5] << ", " << m_positionData[6] << ", life " << m_particlesContainer[1].life << std::endl;
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