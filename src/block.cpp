#include <SDL.h>
#include <SDL_image.h>
#include <Windows.h>
#include "vectors.h"
#include <iostream>

class Block
{
public:
	Vector2 position;
	Uint8 blockType;

	bool IsColliding(Vector2 playerPosition)
	{
		// if we line up on the y axis
		if(((playerPosition.y >= position.y) && ((playerPosition.y) <= (position.y + 40)) || (((playerPosition.y + 90) >= position.y) && ((playerPosition.y + 90) <= (position.y + 40)) || ((playerPosition.y + 45) >= position.y) && (playerPosition.y + 45) <= (position.y + 40))))
		{
			// if we line up on the x axis
			if (((playerPosition.x >= position.x) && ((playerPosition.x) <= (position.x + 40)) || (((playerPosition.x + 40) >= position.x) && ((playerPosition.x + 40) <= (position.x + 40)))))
			{
				return true;
			}
		}

		return false;
	}

	void RenderBlock(Vector2 camPosition, SDL_Rect blockRect, SDL_Renderer* renderer, Vector2 playerPos)
	{
		if (blockType != 0)
		{
			SDL_Rect* tempRect = &blockRect;
			tempRect->x = ((int)position.x - (int)camPosition.x);
			tempRect->y = ((int)position.y - (int)camPosition.y);

			SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
			SDL_RenderFillRect(renderer, tempRect);

			tempRect->x = ((int)position.x + 5 - (int)camPosition.x);
			tempRect->y = ((int)position.y + 5 - (int)camPosition.y);
			tempRect->w = 30;
			tempRect->h = 30;

			SDL_SetRenderDrawColor(renderer, 0, 96, 0, 255);
			SDL_RenderFillRect(renderer, tempRect);
		}

	}
};

class BlockCollum
{
public:
	int x;
	Block blocks[16];
};

class World
{
public:
	BlockCollum blockCollums[32];
};