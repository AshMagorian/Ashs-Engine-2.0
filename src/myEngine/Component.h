#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <string>

class Entity;
class Application;
/**
*An abstract class for all of the components in the game
*/
class Component
{
	friend class Entity;

public:
	Component();
	virtual ~Component();
	/**
	*\brief Returns the entity of this Component
	*/
	std::shared_ptr<Entity> getEntity();
	/**
	*\brief A shortcut to return the Application 
	*/
	std::shared_ptr<Application> getApplication();
private:
	std::weak_ptr<Entity> entity; ///< a reference to the entity which owns this component
	bool began; ///< Determines if onBegin should run
	/**
	*\brief Runs on startup
	*/
	virtual void onInit();
	/**
	*\brief Runs on the first frame
	*/
	virtual void onBegin();
	/**
	*\brief Runs every frame
	*/
	virtual void onTick();
	/**
	*\brief Calls the rendering functions (Only useful for the Render class)
	*/
	virtual void onDisplay();
};
#endif