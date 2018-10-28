#pragma once
#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <chrono>
#include <sstream>

#include "global.h"

class Game;

/////////////////////////////////////////////////////////////////////////////
class GameState
{
protected:
	Game& game;
	SDL::Renderer renderer;

public:
	GameState(Game& game, SDL::Renderer& renderer) : game(game), renderer(renderer) {}

	virtual bool IsFPSLimited() const { return true; }

	virtual void Init() {}
	virtual void Uninit() {}

	virtual void Events(const int frame, const float deltaT) = 0;
	virtual void Update(const int frame, const float deltaT) = 0;
	virtual void Render(const int frame, const float deltaT) = 0;
};

/////////////////////////////////////////////////////////////////////////////
class Game
{
protected:
	SDL::Init     init;
	SDL::Window   window;
	SDL::Renderer renderer;

	TTF::Init     initTTF;
	IMG::Init     initIMG;

	bool isRunning = true;
	int  frame     = 0;

	int currentStateIdx = -1;
	int nextStateIdx    = -1;

	GameState*            currentState = nullptr;
	Array<GameState*, 10> allStates;

public:
	bool IsRunning() const { return isRunning; }

	Game(SDL::Rect windowSize = SDL::Rect(1024, 768), bool vSync = true)
	{
		init.initialize(SDL::Init::Flags::EVERYTHING);
		initTTF.initialize();
		initIMG.initialize(IMG::Init::Flags::EVERYTHING);

		// Create an application window with the following settings:
		window = SDL::Window(
			"An SDL2 window",
			SDL::Window::Pos::CENTERED, // initial x position
			SDL::Window::Pos::CENTERED, // initial y position
			windowSize.w,               // width, in pixels
			windowSize.h,               // height, in pixels
			SDL::Window::Flags::OPENGL |
			SDL::Window::Flags::SHOWN
		);

		// Check that the window was successfully created
		if (!window)
		{
			// In the case that the window could not be made...
			printf("Could not create window: %s\n", SDL::Error());
			THROW_SDL_ERROR(1);
		}

		renderer = SDL::Renderer(
			window,
			-1,
			SDL::Renderer::Flags::ACCELERATED |
			(vSync
				? SDL::Renderer::Flags::PRESENTVSYNC
				: SDL::Renderer::Flags::NONE)
		);
	}

	virtual bool HandleEvent(const Event evt)
	{
		switch (evt.type())
		{
		case Event::Type::QUIT:
			isRunning = false;
			return true;
			break;

		case Event::Type::KEYDOWN:
			{
				auto        kd       = evt.Keyboard();
				SDL::Keysym what_key = kd.keysym;

				if ((what_key.mod()      &  SDL::Keymod::ALT) != SDL::Keymod::NONE &&
					(what_key.scancode() == SDL::Scancode::F4))
				{
					Event(Event::Type::QUIT).Push();
					return true;
				}
				break;
			}
		}
		return false;
	}

	virtual void SetNextState(int index)
	{
		nextStateIdx = index;
	}

	virtual int Run()
	{
		Duration deltaT = Duration::zero();
		Duration deltaTNeeded = Duration::zero();
		TimePoint start = Clock::now();

		while (IsRunning())
		{
			start = Clock::now();

			ActivateNextState();

			const float deltaTF = std::chrono::duration<float>(deltaT).count();
			const float deltaTFNeeded = std::chrono::duration<float>(deltaTNeeded).count();
			
			std::ostringstream oss;
			oss << (deltaTFNeeded * 1000.0f) << "ms";
			SDL::C::SDL_SetWindowTitle(window, oss.str().c_str());

			currentState->Events(frame, deltaTF);
			currentState->Update(frame, deltaTF);
			currentState->Render(frame, deltaTFNeeded);

			deltaTNeeded = Clock::now() - start;

			if (currentState->IsFPSLimited())
			{
				using namespace std::chrono_literals;
				SDL::Delay(std::max(Duration::zero(), 16ms - deltaTNeeded));
				deltaT = Clock::now() - start;
			}

			++frame;
		}
		return 0;
	}

protected:
	virtual void ActivateNextState()
	{
		if (	nextStateIdx != currentStateIdx 
			&&	nextStateIdx != -1)
		{
			if (allStates[nextStateIdx] == nullptr)
			{
				// Load the state or die (for now just die)
				isRunning = false;
			}
			else
			{
				if (currentState != nullptr)
					currentState->Uninit();

				currentStateIdx = nextStateIdx;
				currentState = allStates[currentStateIdx];
				currentState->Init();
			}
		}
	}
};

#endif // GAMEBASE_H
