#include "SDL.h"
#include <SDL_image.h>
#undef main
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "block.h"
#include <stdlib.h>
#include "vectors.h"
#include <list>
#include <ctime>


#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

SDL_Renderer* renderer;
SDL_Window* window;
bool isRunning;
bool fullscreen = false;

World world;

SDL_Rect blockRectDefault;

void Render();
void HandleFPS();
void Update();
void Input();
void GenerateWorld();
void Jump();

int frameCount, timerFPS, lastFrame, fps;
static int lastTime;

bool collisionDirections[4];
bool jumping = false;

Vector2 cameraPosition;

int playerX = (WINDOW_WIDTH / 2) - 20;
int playerY = (WINDOW_HEIGHT / 2) - 45;
Vector2 playerPosition;

SDL_Rect testRect;
SDL_Texture* testTexture;

bool hasCheckedCollisionForFrame = false;
int iTest = 0;
int jumpVelocity = 0;

void GameInit()
{
	playerPosition.x = (WINDOW_WIDTH / 2) - 20;
	playerPosition.y = ((WINDOW_HEIGHT / 2) - 45) + 5;

	blockRectDefault.w = 40;
	blockRectDefault.h = 40;

	cameraPosition.x = 0;
	cameraPosition.y = 0;


	for (int i = 0; i < 4; i++)
	{
		collisionDirections[i] = false;
	}

	fullscreen = false;
	int flags = 0;
	if (fullscreen) {
		flags = flags | SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "***************************\nTerrariaClone by PancakeHat\n***************************\n";
		std::cout << "Initialized subsystems...\n";

		window = SDL_CreateWindow("TerrariaClone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, flags);
		if (window) {
			std::cout << "Created window...\n";
			SDL_SetWindowMinimumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Created renderer...\n";
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			isRunning = true;
		}
		std::cout << "Game has been started, we are ready to roll!\n";
		std::cout << "\nControls:\n'SPACE' - Jump\n'A' - Move left\n'D' - Move right\n'Up/Down/Left/Right - NoClip in any direction\n";
	}

	SDL_Surface* tmpSurface = IMG_Load("assets/test.png");
	testTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	GenerateWorld();
}

int randint(int MAX_NUMBER)
{
	srand(time(0));
	return (rand() % MAX_NUMBER);
}

void GenerateWorld()
{
	for (int i = 0; i < 32; i++)
	{
		world.blockCollums[i].x = (i * 40);
		for (int k = 0; k < 5; k++)
		{
			world.blockCollums[i].blocks[k].position.y = (k * 40) + 640;
			world.blockCollums[i].blocks[k].position.x = world.blockCollums[i].x;
			world.blockCollums[i].blocks[k].blockType = 1;
		}
	}
}

int main() {
	GameInit();

	// update loop
	while (isRunning) {
		Update();
		HandleFPS();
		Render();
		Input();

		int timerFPS = SDL_GetTicks() - lastFrame;
		if (timerFPS < (1000 / 60))
		{
			SDL_Delay((1000 / 60) - timerFPS);
		}
	}

	// closes the game
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);	//error here
	SDL_Quit();


	return 0;
}

void CheckPlayerCollisions(Block blockToCheck)
{
	/*
	0 - no moving left
	1 - no moving right
	2 - no moving down
	3 - no moving up
	*/

	if (playerPosition.x >= blockToCheck.position.x)
	{	
		if(!(playerPosition.y + 90 <= blockToCheck.position.y + 5))
			if (!(playerPosition.y >= blockToCheck.position.y - 5))
				collisionDirections[0] = true;
	}

	if (playerPosition.x + 20 <= blockToCheck.position.x)
	{
		if (!(playerPosition.y + 90 <= blockToCheck.position.y + 5))
			if (!(playerPosition.y >= blockToCheck.position.y - 5))
				collisionDirections[1] = true;
	}

	if(playerPosition.y <= blockToCheck.position.y)
	{
		collisionDirections[2] = true;	
	}

	if (playerPosition.y >= blockToCheck.position.y + 40)
	{
		collisionDirections[3] = true;	
	}

	//if ((playerPosition.x + 20 >= blockToCheck.position.x) && ((playerPosition.x + 20) <= (blockToCheck.position.x + 40)))

}

// render
void Render() {

	hasCheckedCollisionForFrame = false;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// draw here
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);


	collisionDirections[0] = false;
	collisionDirections[1] = false;
	collisionDirections[2] = false;
	collisionDirections[3] = false;

	SDL_Rect rect;
	rect.x = playerPosition.x - cameraPosition.x;
	rect.y = playerPosition.y - cameraPosition.y;
	rect.w = 40;
	rect.h = 90;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
	/* Add corners to the player
	rect.w = 10;
	rect.h = 10;
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
	rect.y = (playerPosition.y + 80) - cameraPosition.y;
	rect.w = 10;
	rect.h = 10;
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
	*/

	for (BlockCollum blockCollum : world.blockCollums)
	{
		for (Block block : blockCollum.blocks)
		{
			block.RenderBlock(cameraPosition, blockRectDefault, renderer, playerPosition);
			if (block.IsColliding(playerPosition))
			{
				CheckPlayerCollisions(block);
				//std::cout << "Collision on frame " << iTest << "; Block :: (" << block.position.x << ", " << block.position.y << ")\n";
				//std::cout << "Coll :: " << collisionDirections[0] << ", " << collisionDirections[1] << ", " << collisionDirections[2] << ", " << collisionDirections[3] << "\n";
			}
		}
	}


	//SDL_RenderCopy(renderer, testTexture, NULL, &rect);

	SDL_RenderPresent(renderer);
}

// update (stuff that happens every frame)
void Update() 
{
	iTest++;
	//std::cout << "Coll :: " << collisionDirections[0] << ", " << collisionDirections[1] << ", " << collisionDirections[2] << ", " << collisionDirections[3] << "\n";
	Jump();
}

void Jump()
{
	if (jumping)
	{
		if (jumpVelocity < 20)
		{
			jumpVelocity++;
			playerPosition.y -= 20 - jumpVelocity;
			cameraPosition.y -= 20 - jumpVelocity;
		}
		else if (jumpVelocity >= 20 && jumpVelocity < 25)
		{
			jumpVelocity++;
			playerPosition.y += jumpVelocity - 10;
			cameraPosition.y += jumpVelocity - 10;
		}
		else if(jumpVelocity >= 25)
			jumping = false;
	}
}

// called every frame to get input
void Input()
{
	// check if the program is being closed
	SDL_Event e;
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			isRunning = false;

		if (keystates[SDL_SCANCODE_ESCAPE]) 
			isRunning = false;
	}

	// get input
	if (GetKeyState(VK_SPACE) & 0x8000 && collisionDirections[2])
	{
		if (!jumping)
		{
			jumpVelocity = 0;
			jumping = true;
		}
	}

	if (!collisionDirections[2] && !(GetKeyState(VK_UP) & 0x8000) && !jumping)
	{
		playerPosition.y += 10;
		cameraPosition.y += 10;
	}
	/*
	if (GetKeyState('S') & 0x8000 && !collisionDirections[2])
	{
		playerPosition.y += 5;
		cameraPosition.y += 5;
	}
	*/
		
	if (GetKeyState('D') & 0x8000 && !collisionDirections[1])
	{
		playerPosition.x += 5;
		cameraPosition.x += 5;
	}
		
	if (GetKeyState('A') & 0x8000 && !collisionDirections[0])
	{
		playerPosition.x -= 5;
		cameraPosition.x -= 5;
	}

	if (GetKeyState(VK_UP) & 0x8000)
	{
		playerPosition.y -= 5;
		cameraPosition.y -= 5;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		playerPosition.y += 5;
		cameraPosition.y += 5;
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		playerPosition.x += 5;
		cameraPosition.x += 5;
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		playerPosition.x -= 5;
		cameraPosition.x -= 5;
	}
	
}


void HandleFPS()
{
	lastFrame = SDL_GetTicks();
	if (lastFrame > (lastFrame + 1000))
	{
		lastTime = lastFrame;
		fps = frameCount;
		frameCount = 0;
	}
	frameCount++;
}