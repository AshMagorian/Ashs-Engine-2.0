#include <vector>
#include <SDL2/SDL.h>

class Appication;
/**
*The input class stores all of the input values from the mouse and keyboard which can then be accessed in the game
*/
class Input
{
private:
	/**
	*\This enum labels the characters as their respecive ascii values which can then be stored as int's in the vectors
	*/
	enum key 
	{
		w = 119,
		a = 97,
		s = 115,
		d = 100,
		space = 32
	};

	std::vector<int> m_keys; ///< Keys which are held down
	std::vector<int> m_pressedKeys; ///<Keys which have been pressed this frame
	std::vector<int> m_releasedKeys; ///< Keys which have been released this frame

	int m_deltaMouseX = 0; ///< The amount the mouse has moved in the x axis this frame
	int m_deltaMouseY = 0; ///< The amount the mouse has moved in the y axis this frame
	int m_mouseX = 0; ///< the current mouse x position
	int m_mouseY = 0; ///< the current mouse y position

	SDL_Event event = { 0 }; ///< Stores the PollEvent

	bool m_quit = false; ///< Sets true if the screen has closed or esc is pushed

public:

	Input() {}
	/**
	*\brief Polls an event to check the keyboard state and mouse state
	*/
	bool UpdateKeys();
	/**
	*\brief Checks if a specific key is held down
	*/
	bool IsKey(char _key); 
	/**
	*\brief Checks if a specific key has been pressed this frame
	*/
	bool IsKeyPressed(char _key);
	/**
	*\brief Checks if a specific key has been released this frame
	*/
	bool IsKeyReleased(char _key);
	/**
	*\brief returns the change in movement of the mouse in the x axis
	*/
	int GetDeltaMouseX() { return m_deltaMouseX; }
	/**
	*\brief returns the change in movement of the mouse in the y axis
	*/
	int GetDeltaMouseY() { return m_deltaMouseY; }
	/**
	*\brief clears the pressed keys and released keys from the previous frame
	*/
	void ClearKeys();

};