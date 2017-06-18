#include "Game.h"


Engine::Game::Game()
{
}


Engine::Game::~Game()
{
}

void Engine::Game::Start(string windowTitle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, WindowFlag windowFlag, unsigned int targetFrameRate, float timeScale)
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	//Tell SDL that we want a double buffered window so we don't get any flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->timeScale = timeScale;

	if (targetFrameRate > 0) {
		targetFrameTime = 1000.0f / targetFrameRate;
	}


	Uint32 flags = SDL_WINDOW_OPENGL;

	if (WindowFlag::EXCLUSIVE_FULLSCREEN == windowFlag) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	if (WindowFlag::FULLSCREEN == windowFlag) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if (WindowFlag::BORDERLESS == windowFlag) {
		flags |= SDL_WINDOW_BORDERLESS;
	}

	// Setup SDL Window
	window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
	if (window == nullptr) {
		Err("Failed to create SDL window!");
	}

	//Set up our OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		Err("Failed to create SDL_GL context!");
	}

	//Set up glew (optional but recommended)
	GLenum glewStat = glewInit();
	if (glewStat != GLEW_OK) {
		Err("Failed to initialize glew!");
	}

	//Set VSYNC
	SDL_GL_SetSwapInterval(vsync);

	this->state = State::RUNNING;

	// Init delta time calculation
	last = SDL_GetTicks();

	Init();
	ControllerInit();
	//Will loop until we set _gameState to EXIT
	while (State::RUNNING == state) {
		float deltaTime = GetDeltaTime();
		GetFPS();
		PollInput();
		PollStateInput();
		Update(deltaTime);
		Render();
		SDL_GL_SwapWindow(window);
		LimitFPS();

		//print only once every 60 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 60) {
			cout << "FPS: " << fps << endl;
			frameCounter = 0;
		}
		//if (controller != NULL) SDL_GameControllerClose(controller);
		//controller = nullptr;
	}

}


float Engine::Game::GetDeltaTime()
{
	unsigned int time = SDL_GetTicks();
	unsigned int delta = time - lastFrame;
	lastFrame = time;

	return delta * timeScale;
}

void Engine::Game::GetFPS()
{
	if (SDL_GetTicks() - last > 1000) {
		fps = _fps;
		_fps = 0;
		last += 1000;
	}
	_fps++;
}

void Engine::Game::PollInput()
{
	SDL_Event evt;
	int evt2  = SDL_GameControllerEventState(SDL_QUERY);
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evt)) {
		switch (evt.type) {
		case SDL_QUIT:
			state = State::EXIT;
			break;
		case SDL_KEYDOWN:
			KeyboardDownMapping(evt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			KeyboardUpMapping(evt.key.keysym.sym);
				break;
		case  SDL_CONTROLLERBUTTONDOWN:
			ControllerDownMapping(evt.cbutton.button);
			break;
		case  SDL_CONTROLLERBUTTONUP:
			ControllerUpMapping(evt.cbutton.button);
			break;
		}
	}
}

void Engine::Game::PollStateInput() 
{
	//Get the keystates
	int keystates = SDL_GameControllerEventState(SDL_ENABLE);
	

	//If up is pressed
	if (keystates == SDL_CONTROLLER_BUTTON_DPAD_UP)
	{
		cout << "Kepencet UPsdfsdf" << endl;
	}
}

void Engine::Game::ControllerInit()
{	
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			break;
		}
	}
}


//Prints out an error message and exits the game
void Engine::Game::Err(string errorString)
{
	cout << errorString << endl;
	SDL_Quit();
	exit(1);
}

void Engine::Game::LimitFPS()
{
	//Limit the FPS to the max FPS
	SDL_Delay((Uint32)(targetFrameTime));

}

void Engine::Game::ControllerDownMapping(int key)
{

}

void Engine::Game::KeyboardDownMapping(int key)
{
}

void Engine::Game::KeyboardUpMapping(int key)
{
}

void Engine::Game::ControllerUpMapping(int key)
{
}
