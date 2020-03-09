#include <myEngine/myEngine.h>

#include "Camera_FirstPerson.h"
#include "SoundToggle.h"
#include "SpinComponent.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argc, char *argv[])
{
	std::shared_ptr<Application> application = Application::init();

	/**
	*Loads all of the resources into the resource manager
	*/
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/textures/yellow.png", "yellow_diffuse");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/textures/orange.png", "orange_diffuse");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/textures/curuthers_diffuse.png", "curuthers_diffuse");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/textures/mimikyu.png", "mimikyu");
	application->GetResourceManager()->CreateResource <VertexArray>("../src/resources/models/curuthers.obj", "curuthers_obj");
	application->GetResourceManager()->CreateResource<Sound>("../src/resources/audio/dixie_horn.ogg", "dixie_horn_ogg");

	application->GetResourceManager()->CreateMaterial("curuthers_mat", application->GetResourceManager()->LoadFromResources<Texture>("curuthers_diffuse"), 32.0f);
	application->GetResourceManager()->CreateMaterial("yellow_mat", application->GetResourceManager()->LoadFromResources<Texture>("yellow_diffuse"), 32.0f);
	application->GetResourceManager()->CreateMaterial("orange_mat", application->GetResourceManager()->LoadFromResources<Texture>("orange_diffuse"), 32.0f);

	/**
	*Creates the camera snd sets it up
	*/
	std::shared_ptr<Entity> firstPersonCamera = application->addEntity();
	firstPersonCamera->addComponent<Camera_FirstPerson>();
	firstPersonCamera->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 10.0f));
	application->GetCamera()->SetCurrentCamera(firstPersonCamera);
	firstPersonCamera->addComponent<BoxCollider>();

	//make point light
	std::shared_ptr<Entity> pointLight = application->MakeCube();
	pointLight->addComponent<PointLight>();
	pointLight->GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
	pointLight->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
	application->GetResourceManager()->CreatePrefab("pointlight", pointLight);

	//make second point light using the first as a reference
	std::shared_ptr<Entity> pointLight2 = application->GetResourceManager()->LoadPrefab("pointlight");
	pointLight2->GetTransform()->SetPos(glm::vec3(-13.5f, 0.0f, 0.0f));
	pointLight2->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("orange_mat"));

	//make spot light
	std::shared_ptr<Entity> spotLight = application->MakeCube();
	spotLight->addComponent<SpotLight>();
	spotLight->GetTransform()->SetPos(glm::vec3(6.0f, 4.0f, 0.0f));
	spotLight->GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));

	/**
	*Creates the spinning model in the middle of the level
	*/
	std::shared_ptr<Entity> model = application->addEntity();
	model->addComponent<Renderer>();
	model->GetComponent<Renderer>()->SetMesh(application->GetResourceManager()->LoadFromResources<VertexArray>("curuthers_obj"));
	model->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("curuthers_mat"));
	model->GetComponent<Renderer>()->SetShader(application->GetResourceManager()->LoadFromResources<ShaderProgram>("lighting_shader"));
	model->addComponent<BoxCollider>();
	model->GetTransform()->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	model->GetTransform()->SetPos(glm::vec3(0.0f, 0.1f, 0.0f));
	model->addComponent<SpinComponent>();

	/**
	*Creates the far wall of the stage
	*/
	std::shared_ptr<Entity> farWall = application->MakeCube();
	farWall->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, -15.0f));
	farWall->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("yellow_mat"));
	farWall->GetTransform()->SetScale(glm::vec3(15.0f, 1.0f, 1.0f));
	farWall->addComponent<BoxCollider>();
	/**
	*Creates the back wall of the stage
	*/
	std::shared_ptr<Entity> backWall = application->MakeCube();
	backWall->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("yellow_mat"));
	backWall->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 15.0f));
	backWall->GetTransform()->SetScale(glm::vec3(15.0f, 1.0f, 1.0f));
	backWall->addComponent<BoxCollider>();
	/**
	*Creates the left wall fo the stage
	*/
	std::shared_ptr<Entity> leftWall = application->MakeCube();
	leftWall->GetTransform()->SetPos(glm::vec3(-15.0f, 0.0f, 0.0f));
	leftWall->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 14.0f));
	leftWall->addComponent<BoxCollider>();
	/**
	*Creates the right wall of the stage
	*/
	std::shared_ptr<Entity> rightWall = application->MakeCube();
	rightWall->GetTransform()->SetPos(glm::vec3(15.0f, 0.0f, 0.0f));
	rightWall->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 14.0f));
	rightWall->addComponent<BoxCollider>();
	/**
	*Creates the floor of the stage
	*/
	std::shared_ptr<Entity> floor = application->MakeCube();
	floor->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("orange_mat"));
	floor->GetTransform()->SetPos(glm::vec3(0.0f, -1.5f, 0.0f));
	floor->GetTransform()->SetScale(glm::vec3(15.0f, 0.5f, 15.0f));

	//make billboard
	std::shared_ptr<Entity> mimikyu = application->addEntity();
	mimikyu->addComponent<SpriteRenderer>(application->GetResourceManager()->LoadFromResources<Texture>("mimikyu"), true);
	mimikyu->GetTransform()->SetPos(glm::vec3(3.0f, 0.0f, 0.0f));
	mimikyu->GetTransform()->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));

	//make partile system
	std::shared_ptr<Entity> particles = application->addEntity();
	particles->addComponent<ParticleSystem>(6000);
	particles->GetTransform()->SetPos(glm::vec3(-3.0f, 20.0f, 0.0f));
	particles->GetTransform()->SetRotation(glm::vec3(0.0f, 0.0f, 180.0f));
	particles->addComponent<SpinComponent>();

	/**
	*Applies sound to the spinning model
	*/
	model->addComponent<SoundComponent>(application->GetResourceManager()->LoadFromResources<Sound>("dixie_horn_ogg"));
	model->addComponent<SoundToggle>();
	/**
	*Runs the game loop from application
	*/
	application->run();
	
  return 0;
}

