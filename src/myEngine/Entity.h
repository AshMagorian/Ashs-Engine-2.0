#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <list>
#include <string>
#include <algorithm>
#include <vector>
#include "Exception.h"

class Application;
class Component;
class Transform;
/**
*The entity is an abstract for every "game object" in the app. It comtains a list of Components which gives an entity its characteristics
*/
class Entity
{
	friend class Application;

private:
	std::weak_ptr<Application> application; ///< A reference to the application
	std::list<std::shared_ptr<Component>> components; ///< The collection of components stored in the entity
	std::weak_ptr<Entity> self; ///< A reference to itself
	std::weak_ptr<Transform> transform; ///< The position, rotation adn scale of the entity
	/**
	*\brief called every frame
	*/
	void tick();
	/**
	*\brief called at the end of each frame to display to the screen
	*/
	void display();

public:
	Entity();
	~Entity();
	/**
	*\brief Returns the application
	*/
	std::shared_ptr<Application> getApplication() {return application.lock();}
	/**
	*Creates a shared pointer of class T where T is the desired component. This is then pushed
	*onto the list of components. The paramaters are passed onto the initialisation function.
	*/
	template <typename T, typename... A>
	std::shared_ptr<T> addComponent(A... args)
	{
		std::shared_ptr<T> rtn = std::make_shared<T>();
		rtn->entity = self;
		rtn->began = false;
		components.push_back(rtn);

		rtn->onInit(args...);

		return rtn;
	}
	/**
	*Creates a shared pointer of a desired class T where T is the component to be returned. All of the components 
	*of this entity are then looped through to check if one of them matches the class T. If there is a match then
	*the component is returned. If not then an exception is thrown.
	*/
	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		std::shared_ptr<T> rtn;

		for (std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i)
		{
			rtn = std::dynamic_pointer_cast<T>(*i);
			if (rtn)
			{
				return rtn;
			}
		}

		throw Exception("Componenet not found");
	}
	/**
	*\brief Returns the list of components
	*/
	std::list<std::shared_ptr<Component>> GetComponents()
	{
		return components;
	}
	/**
	*\brief returns the default transform component
	*/
	std::shared_ptr<Transform> GetTransform()
	{
		return transform.lock();
	}

};
#endif