#include "Game.h"


Game::Game()
{
	mWindow = nullptr;
	mIsRunning = true;
	mBallPos = { 
		1024 / 2, // x position
		768 / 2   // y position
	};
	mPaddlePos = { 
		0, 		// x position
		768 / 2 // y position
	};

}

bool Game::Initialize()
{
	// Returns true if initiliazation is successful
	// Returns false if it fails
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window Text
		100, // Top Left x-coordinate of window
		100, // Top left y-coordinate of window
		1024, // Width of window
		768, // Height of window
		0 // Flags (0 for no flags set))
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// initialize renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1, // Which graphics driver.  -1 by default let SDL decide
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	// Both window and SDL initialization creation are successful
	return true;
}

// Destroys SDL_Window and closes SDL
void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		//UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;

	// While there's still events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// Handle different event types here
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

void Game::GenerateOutput()
{
	
	SDL_SetRenderDrawColor(mRenderer,
		0,   // R
		0,   // G
		255, // B
		255  // A
	);

	// CLear the back buffer
	SDL_RenderClear(mRenderer);

	// Swap the front and back buffers
	SDL_RenderPresent(mRenderer);

	// Setting the color to white
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Create a rectangle object SDL_Rect to reference to.
	SDL_Rect wall{
		0,				// Top Left x
		0,				// Top left y
		1024,			// Width
		thickness		// Height
	};

	// Draw the top border
	SDL_RenderFillRect(mRenderer, &wall);

	// Configure bottom border
	wall = {
		0,
		768 - thickness,
		1024,
		thickness
	};
	// Draw Bottom border
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Configure right border
	wall = {
		1024-thickness,
		0,
		thickness,
		768
	};
	// Draw right border
	SDL_RenderFillRect(mRenderer, &wall);

	


	// Create a ball object
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x + thickness), // x position
		static_cast<int>(mPaddlePos.y - thickness / 2),	// y position
		thickness,										// width
		100												// height
	};
	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_RenderPresent(mRenderer);

}
