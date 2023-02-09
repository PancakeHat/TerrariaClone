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
bool fullscreen;

Block testBlock;
BlockCollum testCollum;

SDL_Rect blockRectDefault;

void Render();
void HandleFPS();
void Update();
void Input();

int frameCount, timerFPS, lastFrame, fps;
static int lastTime;

Vector2 cameraPosition;

int playerX = (WINDOW_WIDTH / 2) - 20;
int playerY = (WINDOW_HEIGHT / 2) - 45;
Vector2 playerPosition;

SDL_Rect testRect;
SDL_Texture* testTexture;

void GameInit()
{
	playerPosition.x = (WINDOW_WIDTH / 2) - 20;
	playerPosition.y = (WINDOW_HEIGHT / 2) - 45;

	testBlock.position.x = 80;
	testBlock.position.y = 80;

	blockRectDefault.w = 40;
	blockRectDefault.h = 40;
	testCollum.x;

	cameraPosition.x = 0;
	cameraPosition.y = 0;

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
	}

	SDL_Surface* tmpSurface = IMG_Load("assets/test.png");
	testTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
}

int main() {
	GameInit();

	// update loop
	while (isRunning) {
		Input();
		Update();
		HandleFPS();
		Render();

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

// render
void Render() {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// draw here
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect;
	rect.x = playerPosition.x;
	rect.y = playerPosition.y;
	rect.w = 40;
	rect.h = 90;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);

	testBlock.RenderBlock(cameraPosition, blockRectDefault, renderer);

	//SDL_RenderCopy(renderer, testTexture, NULL, &rect);

	SDL_RenderPresent(renderer);
}

// update (stuff that happens every frame)
void Update() {

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
	if (GetKeyState('W') & 0x8000)
	{
		playerPosition.y -= 5;
		cameraPosition.y -= 5;
	}
	if (GetKeyState('S') & 0x8000)
		playerPosition.y += 5;
	if (GetKeyState('D') & 0x8000)
		playerPosition.x += 5;
	if (GetKeyState('A') & 0x8000)
		playerPosition.x -= 5;
	
	/*
	// temp
	if (GetKeyState(VK_UP) & 0x8000)
		testBlock.position.y -= 40;
	if (GetKeyState(VK_DOWN) & 0x8000)
		testBlock.position.y += 40;
	if (GetKeyState(VK_RIGHT) & 0x8000)
		testBlock.position.x += 40;
	if (GetKeyState(VK_LEFT) & 0x8000)
		testBlock.position.x -= 40;
	*/
	
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