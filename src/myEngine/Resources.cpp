#include "Resources.h"

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Entity.h"

Resources::Resources()
{
	//CreateResource<VertexArray>("../src/myEngine/Cube.obj");
	//CreateResource<ShaderProgram>("../src/resources/shaders/simpleTex.txt");
	//CreateResource<Texture>("../src/myEngine/BlankColour.png");
}

std::shared_ptr<Material> Resources::CreateMaterial(std::string _name, std::shared_ptr<Texture> _diff, std::shared_ptr<Texture> _spec, float _shine)
{
	std::shared_ptr<Material> material;
	try
	{
		material = std::make_shared<Material>();
		material->SetDiffuse(_diff);
		material->SetSpecular(_spec);
		material->SetShininess(_shine);
	}
	catch (Exception& e)
	{
		std::cout << "myEngine Exception: " << e.what() << std::endl;
		return NULL;
	}
	material->SetPath(_name);
	m_resources.push_back(material);
	std::cout << _name << " created" << std::endl;
	return material;
}
std::shared_ptr<Material> Resources::CreateMaterial(std::string _name, std::shared_ptr<Texture> _diff, float _shine)
{
	std::shared_ptr<Material> material;
	try
	{
		material = std::make_shared<Material>();
		material->SetDiffuse(_diff);
		material->SetSpecular(LoadFromResources<Texture>("grey_diffuse"));
		material->SetShininess(_shine);
	}
	catch (Exception& e)
	{
		std::cout << "myEngine Exception: " << e.what() << std::endl;
		return NULL;
	}
	material->SetPath(_name);
	material->SetName(_name);
	m_resources.push_back(material);
	std::cout << _name << " created" << std::endl;
	return material;
}

void Resources::CreatePrefab(std::string _id, std::shared_ptr<Entity> _entity)
{
	Prefab temp;
	temp.id = _id;
	temp.entity = *_entity;
	m_prefabs.push_back(temp);
	std::cout << _id << " saved to prefabs" << std::endl;
}

std::shared_ptr<Entity> Resources::LoadPrefab(std::string _id)
{
	try
	{
		std::shared_ptr<Entity> rtn;
		for (std::list<Prefab>::iterator i = m_prefabs.begin(); i != m_prefabs.end(); ++i)
		{
			if ((*i).id == _id)
			{
				rtn = std::make_shared<Entity>((*i).entity);
				rtn = rtn->Clone();
				std::cout << _id << " loaded from prefabs" << std::endl;
				return rtn;
			}
		}
		throw Exception(_id + " cannot be loaded from Prefabs");
	}
	catch (Exception& e)
	{
		std::cout << "myEngine Exception: " << e.what() << std::endl;
	}
	return NULL;
}