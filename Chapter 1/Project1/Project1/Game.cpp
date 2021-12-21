#include "Game.h"
const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() : 
	mWindow(nullptr), 
	mRenderer(nullptr), 
	mTicksCount(0), 
	mPaddleDir(0),
	mIsRunning(true)
{}

	//mBallPos = { 
	//	1024 / 2, // x position
	//	768 / 2   // y position
	//};
	//mPaddlePos = { 
	//	0, 		// x position
	//	768 / 2 // y position
	//};



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
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
	return true;
}


void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
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

	// Update paddle directionb ased on W/S keys
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	if (mPaddleDir != 0)
	{

		// Update teh y position of the paddle based on the direction
		// 300.0f pixels/second
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

		// Make sure paddle doesn't move off screen!
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;


	// Is the ball overlapping with the paddle to interact?
	float diff = mPaddlePos.y - mBallPos.y;
	// Take the absolute value of the difference
	diff = (diff > 0.0f) ? diff : -diff;

	if (
		// Our y-difference is small enough
		diff <= paddleH / 2.0f &&
		// Ball is at the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&

		// The ball is moving to the left
		mBallVel.x < 0.0f
		)
	{
		mBallVel.x *= -1.0f;
	}
	// Did the ball go off screen? (if so end the game)
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}

	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		// Then go downwards
		mBallVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?

	else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
	{
		// Then go upwards
		mBallVel.y *= -1;
	}
	// did the ball collide with the right wall?
	else if (mBallPos.x >= (1024 - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1;
	}
}


void Game::GenerateOutput()
{
	
	SDL_SetRenderDrawColor(
		mRenderer,
		0,   // R
		0,   // G
		255, // B
		255  // A
	);

	// CLear the back buffer
	SDL_RenderClear(mRenderer);

	//// Swap the front and back buffers
	//SDL_RenderPresent(mRenderer);

	// Draw walls 
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw top wall
	SDL_Rect wall{
		0,				// Top Left x
		0,				// Top left y
		1024,			// Width
		thickness		// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw right wall
	wall = {
		1024 - thickness,
		0,
		thickness,
		1024
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw a ball object
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// Draw Paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x), // x position
		static_cast<int>(mPaddlePos.y - paddleH/ 2),	// y position
		thickness,										// width
		static_cast<int>(paddleH)						// height
	};
	SDL_RenderFillRect(mRenderer, &paddle);


	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);

}



// Destroys SDL_Window and closes SDL
void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
