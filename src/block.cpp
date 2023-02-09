#include <SDL.h>
#include <SDL_image.h>
#include <Windows.h>
#include "vectors.h"

class Block
{
public:
	Vector2 position;
	Uint8 blockType;

	void RenderBlock(Vector2 camPosition, SDL_Rect blockRect, SDL_Renderer* renderer)
	{
		SDL_Rect* tempRect = &blockRect;
		tempRect->x = position.x;
		tempRect->y = position.y;

		SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
		SDL_RenderFillRect(renderer, tempRect);

	}
};

class BlockCollum
{
public:
	int x;
	Block blocks[16];
};