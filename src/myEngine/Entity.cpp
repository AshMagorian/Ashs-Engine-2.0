#include "Entity.h"
#include "Application.h"
#include "Component.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

/**
*\brief runs through the onTick functions of each component. Also runs the onBegin function on teh first frame
*/
void Entity::tick()
{
	for (std::list < std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i)
	{
		if ((*i)->began == false)
		{
			(*i)->onBegin();
			(*i)->began = true;
		}
		(*i)->onTick();
	}
}
/**
*\brief Runs through each component's display function
*/
void Entity::display()
{
	for (std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i)
	{
		(*i)->onDisplay();
	}
}