#include "Entity.h"
#include "Application.h"
#include "Component.h"
#include "Transform.h"

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

std::shared_ptr<Entity> Entity::Clone()
{
	std::shared_ptr<Entity> rtn = getApplication()->addEntity();

	rtn->GetTransform()->SetPos(this->GetTransform()->GetPos());
	rtn->GetTransform()->SetRotation(this->GetTransform()->GetRotation());
  	rtn->GetTransform()->SetScale(this->GetTransform()->GetScale());


	for (std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i)
	{

		if ((*i)->Clone(rtn) == false)
		{
			//std::shared_ptr<Component> tmp;
			//(tmp) = (*i);
			//rtn->GetComponents().push_back(tmp);
			//tmp->onInit();

			//std::cout << "Component not cloned!" << std::endl;
		}
	}
	return rtn;
}