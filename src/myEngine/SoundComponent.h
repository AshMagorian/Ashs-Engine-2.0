#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H
#include "Component.h"

class Sound;
/**
*This is a component which can be added to an entity. It contains the sound file stored in the Sound object.
*It can then call play on the sound file when required
*/
class SoundComponent : public Component
{
private:
	std::shared_ptr<Sound> m_soundSource; ///< The sound resource
	bool m_isPlaying = false; ///< Toggles playing the sound


public:
	/**
	*\brief sets the sound resource
	*/
	void onInit(std::shared_ptr<Sound> _sound);
	/**
	*\brief Called every frame
	*/
	void onTick();
	/**
	*\brief Plays the sound
	*/
	void Play();
	/**
	*\brief toggles the isPlaying bool
	*/
	void SetIsPlaying(bool _val) { m_isPlaying = _val;}

	bool Clone(std::shared_ptr<Entity> _entity);
};
#endif