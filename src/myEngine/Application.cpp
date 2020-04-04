#include "myEngine.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/**
*\brief Constructor
*/
Application::Application()
{

}

/**
*\brief Cleans up raw pointers
*/
Application::~Application()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	SDL_DestroyWindow(window);
}

/**
*Creates the Applicatino object. Also initialises SDL, glew and ALC. Also loads default resources to the resource manager. If any 
*of these fail then an exception is thrown and caught.
*/
std::shared_ptr<Application> const Application::init()
{
	std::shared_ptr<Application> app = std::make_shared<Application>();
	app->running = false;
	app->self = app;
	try { if (SDL_Init(SDL_INIT_VIDEO) < 0){throw Exception("SDL_VIDEO not initialised");}}
	catch (Exception& e) { std::cout << "myEngine Exception: " << e.what() << std::endl; }

	app->window = SDL_CreateWindow("my game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);




	try { if (!SDL_GL_CreateContext(app->window)) { throw Exception("Window couldn't be created"); } }
	catch (Exception& e) { std::cout << "myEngine Exception: " << e.what() << std::endl; }

	try { if (glewInit() != GLEW_OK) { throw Exception("GLEW couldn't be initialised"); } }
	catch (Exception& e) { std::cout << "myEngine Exception: " << e.what() << std::endl; }

	app->device = alcOpenDevice(NULL);

	try { if (!app->device) { throw Exception("alc OpenDevice failed"); } }
	catch (Exception& e) { std::cout << "myEngine Exception: " << e.what() << std::endl; }

	app->context = alcCreateContext(app->device, NULL);

	try { if (!app->context) { throw Exception("alc CreateContext failed"); } }
	catch (Exception& e) { alcCloseDevice(app->device); std::cout << "myEngine Exception: " << e.what() << std::endl; }

	try { if (!alcMakeContextCurrent(app->context)) { throw Exception("alc MakeContextCurrent failed"); } }
	catch (Exception& e) 
	{ 
		alcDestroyContext(app->context);
		alcCloseDevice(app->device); 
		std::cout << "myEngine Exception: " << e.what() << std::endl; 
	}
	try
	{ 
		if (!app->m_resourceManager)
		{
			throw Exception("Resource manager not loaded"); 
		}
		app->GetResourceManager()->CreateResource<VertexArray>("../src/myEngine/engineRes/Cube.obj", "cube_obj");
		app->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/simpleTex.txt", "simple_shader");
		app->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/lightingShader.txt", "lighting_shader");
		app->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/particleShader.txt", "particle_shader");
		app->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/billboardTexShader.txt", "billboardTex_shader");
		app->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/particleTexShader.txt", "particle_tex_shader");
		app->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/particleMaskShader.txt", "particle_mask_shader");
		app->GetResourceManager()->CreateResource<ShaderProgram>("../src/resources/shaders/weatherShader.txt", "weather_shader");
		app->GetResourceManager()->CreateResource<Texture>("../src/myEngine/engineRes/Grey.png", "grey_diffuse");
		app->GetResourceManager()->CreateMaterial("default_mat", app->GetResourceManager()->LoadFromResources<Texture>("grey_diffuse"), 32.0f);
	}
	catch (Exception& e) { std::cout << "myEngine Exception: " << e.what() << std::endl; }

	app->GetLightManager()->AddShaderProgram(app->GetResourceManager()->LoadFromResources<ShaderProgram>("lighting_shader"));

	/**
	*\brief Makes sure only the nearest faces of objects are drawn
	*/
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return app;
}
/**
* Runs the main game loop. Updates Inputs, updates Entities and then draws them to the updated camera
*/
void Application::run()
{
	running = true;

	int windowWidth = 0;
	int windowHeight = 0;

	m_lightManager->m_application = self;

	while (running)
	{
		/**
		*\brief Starts the timer for this frame
		*/
		m_time->StartOfFrame();
		/**
		*\brief Dynamically sets the window width and height every frame
		*/
		SDL_GetWindowSize(window, &windowWidth, &windowHeight); 


		/**
		*\brief Updates the state of the input object
		*/
		if (!m_input->UpdateKeys()) { stop(); }

		SDL_WarpMouseInWindow(window, 320, 240); // moves the mouse to the middle of the window
		SDL_ShowCursor(SDL_DISABLE); // Hides the cursor

		m_lightManager->UpdateLightShaderValues();

		/**
		*\brief Loops through and updates all entites. Catches any exceptions thrown
		*/
		for (std::list<std::shared_ptr<Entity>>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			try
			{
				(*i)->tick();
			} 

			catch (Exception& e) 
			{ 
				std::cout << "myEngine Exception: " << e.what() << std::endl; 
			}
		}
		/**
		*\brief Resets the background colour and depth buffer
		*/
		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		try
		{
			m_mainCamera->UpdateMatrix(windowWidth, windowHeight);
		}
		catch (Exception& e)
		{
			std::cout << "myEngine Exception: " << e.what() << std::endl;
		}
		/**
		*\brief Displays every entity (If they have a renderer component)
		*/
		for (std::list<std::shared_ptr<Entity>>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			(*i)->display();
		}

		SDL_GL_SwapWindow(window);
		/**
		*\brief Stops the timer and caps the framerate
		*/
		m_time->CapFramerate(60.0f);
	}
}
/**
*\brief Sets the running boolean to false to break out of the game loop
*/
void Application::stop()
{
	running = false;
}
/**
*\brief Returns the DeltaTime value calculated in the Enviroment object
*/
float Application::GetDeltaTime()
{
	return m_time->GetDeltaTime(); 
}
/**
* Creates an entity pointer and pushes in back onto the entity list. Each entity gets a default transform component
*so that the EEntity class can have a GetTransform function. This is faster than calling GetComponent every time
*/
std::shared_ptr<Entity> Application::addEntity()
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	entity->self = entity;
	entity->application = self;
	entity->transform = entity->addComponent<Transform>();
	entities.push_back(entity);
	return entity;
}
/**
*\brief Creates an entity pointer. It then gives it a Renderer component which is set to some default values to produce a
*cube. This makes creating a level from blocks a little easier than having to define the components of a cube each time. 
*The cube entity is then pushed back onto the entity list
*/
std::shared_ptr<Entity> Application::MakeCube()
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	entity->self = entity;
	entity->application = self;
	entity->transform = entity->addComponent<Transform>();
	entity->addComponent<Renderer>(m_resourceManager->LoadFromResources<ShaderProgram>("lighting_shader"),
									m_resourceManager->LoadFromResources<VertexArray>("cube_obj"),
									m_resourceManager->LoadFromResources<Material>("default_mat"));
	entities.push_back(entity);
	return entity;
}