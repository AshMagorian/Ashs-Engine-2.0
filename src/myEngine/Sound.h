#ifndef SOUND_H
#define SOUND_H

#include <vector>
#include <AL/al.h>

#include "Resource.h"
/**
*The Sound class is a resource which loads and contains the sound file
*/
class Sound : public Resource
{
private:
	ALuint id;

public:
	/**
	*\brief Calls the Load function
	*/
	Sound(std::string _path);
	~Sound() { alDeleteBuffers(1, &id); }
	/**
	*\brief Loads the sound from the path provided
	*/
	void Load(std::string _path);
	/**
	*\brief Reads the ogg file and passes the information into the buffer
	*/
	void LoadOggFile(const std::string& fileName, std::vector<char>& buffer,
		ALenum &format, ALsizei &freq);

	ALuint GetId() { return id; }
};
#endif