#include <SDL.h>

// Vector2 struct that stores x/y position
struct Vector2 {
	float x;
	float y;
};
class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();

private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	//
	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of the game.
	Uint32 mTicksCount;

	// Game should continue to run
	bool mIsRunning;
	Vector2 mPaddlePos;
	Vector2 mBallPos;
	Vector2 mBallVel;
	int mPaddleDir;

};