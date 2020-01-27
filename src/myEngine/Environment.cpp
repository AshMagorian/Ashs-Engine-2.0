#include "Environment.h"

/**
*\brief Calculates DeltaTime at the start of each frame
*/
void Environment::StartOfFrame()
{
	m_currentTime = SDL_GetTicks();		// Calculate deltatime
	m_deltaTime = (float)(m_currentTime - m_lastTime) / 1000.0f;
}
/**
*\brief Pauses each frame to make sure framerate is smooth
*/
void Environment::CapFramerate(float _framerate)
{
	if (m_deltaTime < (1.0f / _framerate))		//caps framerate
	{
		SDL_Delay((unsigned int)(((1.0f / _framerate) - m_deltaTime)*1000.0f));
	}
	m_lastTime = m_currentTime;
}