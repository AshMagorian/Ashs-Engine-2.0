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
	//m_delta = 0.016f;
	//std::cout << "delta: " << m_delta << std::endl;
	m_rotMatrix = getEntity()->GetTransform()->GetRotationMatrix();

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

		glm::vec3 planePosition = glm::vec3(m_positionOffset.x * rndm, 0.0f , m_positionOffset.y * rndm2);

		planePosition = glm::vec4(planePosition, 1.0f) * getEntity()->GetTransform()->GetModelMatrix();
		



		m_particlesContainer[j].pos = planePosition;
		//m_particlesContainer[j].pos = getEntity()->GetTransform()->GetPos();

		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		rndm2 = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));

		float angle = rndm * m_spreadAngle;
		float x = cos(abs((m_pi / 2.0f) - glm::radians(angle)));
		float y = sin(abs((m_pi / 2.0f) - glm::radians(angle)));
		angle = rndm2 * m_pi * 2.0f;
		float x2 = cos(angle) * x;
		float y2 = sin(angle) * x;
		glm::vec3 randomSpreadDirection = glm::vec3(x2, y, y2);

		glm::quat p = glm::quat(randomSpreadDirection.x, randomSpreadDirection.y, randomSpreadDirection.z, 0.0f);
		glm::quat p1 = m_quat * p * glm::conjugate(m_quat);
		glm::vec3 direction = glm::vec3(p1.x, p1.y, p1.z); 
		direction = glm::vec4(direction, 1.0f) * m_rotMatrix;

		m_particlesContainer[j].velocity = m_speed * direction;

		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_particlesContainer[j].r = rndm;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_particlesContainer[j].g = rndm;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_particlesContainer[j].b = rndm;
		m_particlesContainer[j].a = 0.7f;

		m_particlesContainer[j].size = 0.2f;
		
		m_particlesContainer[j].life = m_particleLife;
		
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
				p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * m_delta * 0.5f;
				p.pos += p.velocity * m_delta;
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