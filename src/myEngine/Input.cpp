#include "Input.h"
#include <iostream>

/**
*\brief Polls an event to check the keyboard state and mouse state
*/
bool Input::UpdateKeys()
{
	ClearKeys();

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: m_quit = true; break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) { m_quit = true; }

			if (event.key.keysym.sym == SDLK_w) { if (!IsKey(w)) { m_keys.push_back(w); m_pressedKeys.push_back(w); } } ///< If 'w' isn't in the vector then add it to the vector
			if (event.key.keysym.sym == SDLK_a) { if (!IsKey(a)) { m_keys.push_back(a); m_pressedKeys.push_back(a); } } ///< If 'a' isn't in the vector then add it to the vector
			if (event.key.keysym.sym == SDLK_s) { if (!IsKey(s)) { m_keys.push_back(s); m_pressedKeys.push_back(s); } } ///< If 's' isn't in the vector then add it to the vector
			if (event.key.keysym.sym == SDLK_d) { if (!IsKey(d)) { m_keys.push_back(d); m_pressedKeys.push_back(d); } } ///< If 'd' isn't in the vector then add it to the vector
			if (event.key.keysym.sym == SDLK_SPACE) { if (!IsKey(SDLK_SPACE)) { m_keys.push_back(SDLK_SPACE); m_pressedKeys.push_back(SDLK_SPACE); } } ///< If 'space' isn't in the vector then add it to the vector
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_w) 
			{ 
				m_releasedKeys.push_back(w); 
				///Removes this key from the held keys vector
				for (int i = 0; i < (int)m_keys.size(); ++i)
				{
					if (m_keys.at(i) == w) { m_keys.erase(m_keys.begin() + i); break; }
				}
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				m_releasedKeys.push_back(a);
				///Removes this key from the held keys vector
				for (int i = 0; i < (int)m_keys.size(); ++i)
				{
					if (m_keys.at(i) == a) { m_keys.erase(m_keys.begin() + i); break; }
				}
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				m_releasedKeys.push_back(s);
				///Removes this key from the held keys vector
				for (int i = 0; i < (int)m_keys.size(); ++i)
				{
					if (m_keys.at(i) == s) { m_keys.erase(m_keys.begin() + i); break; }
				}
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				m_releasedKeys.push_back(d);
				///Removes this key from the held keys vector
				for (int i = 0; i < (int)m_keys.size(); ++i)
				{
					if (m_keys.at(i) == d) { m_keys.erase(m_keys.begin() + i); break; }
				}
			}
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				m_releasedKeys.push_back(SDLK_SPACE);
				///Removes this key from the held keys vector
				for (int i = 0; i < (int)m_keys.size(); ++i)
				{
					if (m_keys.at(i) == SDLK_SPACE) { m_keys.erase(m_keys.begin() + i); break; }
				}
			}
		}
		if (event.type == SDL_MOUSEMOTION)
		{
			m_mouseX = event.motion.x;
			m_mouseY = event.motion.y;
			m_deltaMouseX = (int)(640/2) - m_mouseX;  ///< deltaX = centre of screen - mouseX
			m_deltaMouseY = (int)(480/2) - m_mouseY;	 ///< deltaY = centre of screen - mouseY
		}
	}

	if (m_quit == true) { return false; }
	return true;
}
/**
*\brief Checks if a specific key stored in the keys vector
*/
bool Input::IsKey(char _key)
{
	int k = (key)_key;
	for (int i = 0; i < m_keys.size(); ++i)
	{
		if (m_keys.at(i) == k)
		{
			return true;
		}
	}
	return false;
}
/**
*\brief Checks if a specific key is stored in the pressedKeys vector
*/
bool Input::IsKeyPressed(char _key)
{
	int k = (key)_key;
	for (int i = 0; i < (int)m_pressedKeys.size(); ++i)
	{
		if (m_pressedKeys.at(i) == k)
		{
			return true;
		}
	}
	return false;
}
/**
*\brief Checks if a specific key is stored in the releasedKeys vector
*/
bool Input::IsKeyReleased(char _key)
{
	int k = (key)_key;
	for (int i = 0; i < (int)m_releasedKeys.size(); ++i)
	{
		if (m_releasedKeys.at(i) == k)
		{
			return true;
		}
	}
	return false;
}
/**
*\brief clears the pressed keys and released keys from the previous frame
*/
void Input::ClearKeys()
{
	if (!m_pressedKeys.empty()) { m_pressedKeys.clear(); }
	if (!m_releasedKeys.empty()) { m_releasedKeys.clear(); }
}