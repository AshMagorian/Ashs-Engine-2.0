#include "SoundComponent.h"
#include "myEngine.h"
#include "Exception.h"

/**
*\brief sets the sound resource
*/
void SoundComponent::onInit(std::shared_ptr<Sound> _sound)
{
	m_soundSource = _sound;
}
/**
*\brief Called every frame. checks if isPlaying is set to true. Plays if true
*/
void SoundComponent::onTick()
{
	if (m_isPlaying == true)
	{
		Play();
		m_isPlaying = false;
	}
}

/**
*\Sets the listener as the position of the camera and the source as the object containing the sound component.
*Then plays the sound. Throws and catches an exception if the sound resource doesn't exist
*/
void SoundComponent::Play()
{
	GLuint sourceId = 0;
	alGenSources(1, &sourceId);
	alListener3f(AL_POSITION,	getApplication()->GetCamera()->GetCurrentCamera()->GetTransform()->GetPos().x,
								getApplication()->GetCamera()->GetCurrentCamera()->GetTransform()->GetPos().y, 
								getApplication()->GetCamera()->GetCurrentCamera()->GetTransform()->GetPos().z );
	alSource3f(sourceId, AL_POSITION, getEntity()->GetTransform()->GetPos().x, 
								 getEntity()->GetTransform()->GetPos().y, 
								 getEntity()->GetTransform()->GetPos().z);
	try
	{
		if (!m_soundSource)
		{
			throw Exception("Sound cannot be played, sound source doesn't exist");
		}
	}
	catch (Exception& e)
	{
		std::cout << "myEngine Exception: " << e.what() << std::endl;
		return;
	}

	alSourcei(sourceId, AL_BUFFER, m_soundSource->GetId());

	alSourcePlay(sourceId);
}