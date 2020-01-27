#ifndef APPLICATION_H
#define APPLICATION_H
#include <memory>
#include <list>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>

class Entity;
class ShaderProgram;
class VertexArray;
class Camera;
class Environment;
class Input;
class Resources;
class Resource;
class Lights;
/**
*The Application class is where all of the entities in the game are stored. the application initialises all of the libraries required to run the
*game and then runs the game loop. Almost every class in the engine has direct access to the app so it is a way of linking everything together
*/
class Application
{
private:
	bool running;
	std::list<std::shared_ptr<Entity>> entities; ///< A list of all of teh entities stored in the application
	std::weak_ptr<Application> self; ///< A Stores weak pointer to itself

	std::shared_ptr<Camera> m_mainCamera = std::make_shared<Camera>(); ///< A pointer to the main camera in the game
	std::shared_ptr<Environment> m_time = std::make_shared<Environment>(); ///< A pointer to the environment object so DeltaTime can be accessed
	std::shared_ptr<Input> m_input = std::make_shared<Input>(); ///< A Pointer to the Input object so that inputs can be accessed
	std::shared_ptr<Resources> m_resourceManager = std::make_shared<Resources>(); ///< A pointer to the resource manager for accesibility
	std::shared_ptr<Lights> m_lightManager = std::make_shared<Lights>();

	SDL_Window *window; ///< The SDL window to be created
	ALCdevice* device; ///< Audio device to be created
	ALCcontext* context; ///< Audio context to be created

public:
	/**
	*\brief Constructor
	*/
	Application();
	/**
	*\brief Cleanup of raw pointers
	*/
	~Application();
	/**
	*\brief Initialises SDL, glew and ALC. Also loads default resources to the resource manager
	*/
	static std::shared_ptr<Application> const init();
	/**
	*\brief Enters the main loop
	*/
	void run();
	/**
	*\brief Sets the running boolean to false to break out of the game loop
	*/
	void stop();
	/**
	*\brief Returns the main camera
	*/
	std::shared_ptr<Camera> GetCamera() { return m_mainCamera; }
	/**
	*\brief Returns the stored input object
	*/
	std::shared_ptr<Input> GetInput() { return m_input; }
	/**
	*\brief Returns the stored resource manager object
	*/
	std::shared_ptr<Resources> GetResourceManager() { return m_resourceManager; }

	std::shared_ptr<Lights> GetLightManager() { return m_lightManager; }
	/**
	*\brief Returns the DeltaTime value calculated in the Enviroment object
	*/
	float GetDeltaTime();
	/**
	*\brief Creates an entity pointer and pushes in back onto the entity list
	*/
	std::shared_ptr<Entity> addEntity();
	/**
	*\brief Creates an entity pointer for a cube object and pushes in back onto the entity list
	*/
	std::shared_ptr<Entity> MakeCube();
	/**
	*Searches through the components of all the entities and checks if they have 
	*a predefined component <T>. If said Entity does have that component then 
	*the Entity is pushed back onto the vector passed in the paramater.
	*/
	template <typename T>
	void GetEntities(std::vector<std::shared_ptr<Entity>> &_entities)
	{
		std::shared_ptr<T> testComponent; ///< An arbitrary component used for testing the component of the Entity
		///Loop through all Entities
		for (std::list<std::shared_ptr<Entity>>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			///Gets the list of components of that entity
			std::list<std::shared_ptr<Component>> components = (*i)->GetComponents();
			///Loop through all components of that Entity
			for (std::list<std::shared_ptr<Component>>::iterator j = components.begin(); j != components.end(); ++j)
			{
				testComponent = std::dynamic_pointer_cast<T>(*j); ///< Tries to cast the Component onto the predefined test component
				///If testComponent doesnt exist then the cast was unsuccesful meaning the Component isn't T
				if (testComponent)
				{
					_entities.push_back(*i); ///<Pushes back the required component
					testComponent = NULL; /// Resets the testComponent for more testing
				}
			}
		}
	}




};
#endif