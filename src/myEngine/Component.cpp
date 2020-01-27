#include "Component.h"

#include "Application.h"
#include "Entity.h"

Component::Component()
{

}

Component::~Component()
{

}
/**
*\brief Returns the entity of this Component
*/
std::shared_ptr<Entity> Component::getEntity()
{
	return entity.lock();
}
/**
*\brief A shortcut to return the Application
*/
std::shared_ptr<Application> Component::getApplication()
{
	return getEntity()->getApplication();
}

void Component::onInit()
{

}
void Component::onBegin()
{

}
void Component::onTick()
{

}
void Component::onDisplay()
{

}