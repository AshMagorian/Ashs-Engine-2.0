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
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/textures/yellow.png");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/textures/orange.png");
	application->GetResourceManager()->CreateResource<Texture>("../src/resources/textures/curuthers_diffuse.png");
	application->GetResourceManager()->CreateResource <VertexArray>("../src/resources/models/curuthers.obj");
	application->GetResourceManager()->CreateResource<Sound>("../src/resources/audio/dixie_horn.ogg");

	application->GetResourceManager()->CreateMaterial("curuthersMat", application->GetResourceManager()->LoadFromResources<Texture>("../src/resources/textures/curuthers_diffuse.png"), 32.0f);
	application->GetResourceManager()->CreateMaterial("yellowMat", application->GetResourceManager()->LoadFromResources<Texture>("../src/resources/textures/yellow.png"), 32.0f);
	application->GetResourceManager()->CreateMaterial("orangeMat", application->GetResourceManager()->LoadFromResources<Texture>("../src/resources/textures/orange.png"), 32.0f);

	//make point light
	std::shared_ptr<Entity> pointLight = application->MakeCube();
	pointLight->addComponent<PointLight>();
	pointLight->GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
	pointLight->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
	application->GetResourceManager()->CreatePrefab("pointlight", pointLight);

	//make point light
	std::shared_ptr<Entity> pointLight2 = application->MakeCube();
	pointLight2->addComponent<PointLight>();
	pointLight2->GetTransform()->SetPos(glm::vec3(-13.5f, 0.0f, 0.0f));
	pointLight2->GetTransform()->SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
	
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
	model->GetComponent<Renderer>()->SetMesh(application->GetResourceManager()->LoadFromResources<VertexArray>("../src/resources/models/curuthers.obj"));
	model->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("curuthersMat"));
	model->GetComponent<Renderer>()->SetShader(application->GetResourceManager()->LoadFromResources<ShaderProgram>("../src/resources/shaders/lightingShader.txt"));
	model->addComponent<BoxCollider>();
	model->GetTransform()->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	model->GetTransform()->SetPos(glm::vec3(0.0f, 0.1f, 0.0f));
	model->addComponent<SpinComponent>();

	/**
	*Creates the far wall of the stage
	*/
	std::shared_ptr<Entity> farWall = application->MakeCube();
	farWall->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, -15.0f));
	farWall->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("yellowMat"));
	farWall->GetTransform()->SetScale(glm::vec3(15.0f, 1.0f, 1.0f));
	farWall->addComponent<BoxCollider>();
	/**
	*Creates the back wall of the stage
	*/
	std::shared_ptr<Entity> backWall = application->MakeCube();
	backWall->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("yellowMat"));
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
	floor->GetComponent<Renderer>()->SetMaterial(application->GetResourceManager()->LoadFromResources<Material>("orangeMat"));
	floor->GetTransform()->SetPos(glm::vec3(0.0f, -1.5f, 0.0f));
	floor->GetTransform()->SetScale(glm::vec3(15.0f, 0.5f, 15.0f));

	/**
	*Creates the camera snd sets it up
	*/
	std::shared_ptr<Entity> firstPersonCamera = application->addEntity();
	firstPersonCamera->addComponent<Camera_FirstPerson>();
	firstPersonCamera->GetTransform()->SetPos(glm::vec3(0.0f, 0.0f, 10.0f));
	application->GetCamera()->SetCurrentCamera(firstPersonCamera);
	firstPersonCamera->addComponent<BoxCollider>();

	/**
	*Applies sound to the spinning model
	*/
	model->addComponent<SoundComponent>(application->GetResourceManager()->LoadFromResources<Sound>("../src/resources/audio/dixie_horn.ogg"));
	model->addComponent<SoundToggle>();
	/**
	*Runs the game loop from application
	*/
	application->run();
	
  return 0;
}

