#include "Resources.h"

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"

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
		material->SetSpecular(LoadFromResources<Texture>("../src/myEngine/engineRes/Grey.png"));
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