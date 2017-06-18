#ifndef GAME_H
#define GAME_H

#include "Core.h"

using namespace std;

enum class State { RUNNING, EXIT };
enum class WindowFlag { WINDOWED, FULLSCREEN, EXCLUSIVE_FULLSCREEN, BORDERLESS};

namespace Engine {
	class Game
	{
	public:
		Game();
		~Game();
		void Start(string title, unsigned int width, unsigned int height, bool vsync, WindowFlag windowFlag, unsigned int targetFrameRate, float timeScale);
	protected:
		virtual void Init() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void ControllerDownMapping(int key);
		virtual void KeyboardDownMapping(int key);
		virtual void KeyboardUpMapping(int key);
		virtual void ControllerUpMapping(int key);
		virtual void Render() = 0;
		unsigned int screenWidth, screenHeight;
		SDL_Window* window;
	private:
		SDL_GameController *controller = nullptr;
		unsigned int lastFrame = 0, last = 0, _fps = 0, fps = 0;
		float targetFrameTime = 0, timeScale;
		State state;
		float GetDeltaTime();
		void GetFPS();
		void PollInput();
		void PollStateInput();
		void ControllerInit();
		void Err(string errorString);
		void LimitFPS();
		
	};
}
#endif

