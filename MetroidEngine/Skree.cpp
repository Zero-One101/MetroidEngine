#include "Tmx.h"
#include "Skree.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

Skree::Skree(int posX, int posY)
{
	skreeSprites = al_load_bitmap(".//assets//sprites//skree.png");
	al_convert_mask_to_alpha(skreeSprites, al_map_rgb(255, 0, 255));
	this->posX = posX;
	this->posY = posY;
	maxYDistance = 200;
	skreeWidth = 16;
	skreeHeight = 24;
	collisionRect.X = this->posX;
	collisionRect.Y = this->posY;
	collisionRect.W = skreeWidth;
	collisionRect.H = skreeHeight;
	moveSpeedY = 4;
	falling = false;
	dead = false;
	dyingFrames = 0;
	deadFrames = 60;
	movement = HANGING;
	currentFrameTime = 0;
	currentFrame = 1;
}

Skree::~Skree()
{
}

void Skree::Update(MetRect samusRect)
{
	if (!dead)
	{
		if (samusRect.X == posX && movement == HANGING)
		{
			movement = FALLING;
		}
		if (movement == FALLING)
		{
			posY += moveSpeedY;
		}
		collisionRect.X = posX;
		collisionRect.Y = posY;

		if (movement == EMBEDDED)
		{
			dyingFrames++;
			if (dyingFrames == deadFrames)
			{
				dead = true;
			}
		}
	}
}

void Skree::Draw()
{
	if (!dead)
	{
		//al_draw_filled_rectangle(posX, posY, posX + skreeWidth, posY + skreeHeight, al_map_rgb(0, 255, 0));
		//al_draw_rectangle(collisionRect.X, collisionRect.Y, collisionRect.X + collisionRect.W, collisionRect.Y + collisionRect.H, al_map_rgb(255, 0, 0), 2);
		if (movement == HANGING)
		{
			al_draw_bitmap_region(skreeSprites, currentFrame * skreeWidth, 0, skreeWidth, skreeHeight, posX, posY, 0);
		}
		else
		{
			al_draw_bitmap_region(skreeSprites, currentFrame * skreeWidth, 0, skreeWidth, skreeHeight, posX, posY, 0);
			currentFrameTime++;
			if (currentFrameTime == frameTime)
			{
				currentFrame++;
				if (currentFrame > 3)
				{
					currentFrame = 0;
				}
				currentFrameTime = 0;
			}
		}
	}
}

void Skree::CheckCollision(const Tmx::Object *collisionObject)
{
	if (movement == FALLING)
	{
		MetRect objectCol(collisionObject->GetX(), collisionObject->GetY(), collisionObject->GetWidth(), collisionObject->GetHeight());

		nextPosY = posY + moveSpeedY;
		MetRect skreeCol(nextPosX, posY + moveSpeedY, skreeWidth, skreeHeight);

		// Check for Y-axis collision
		if (posX < objectCol.X + objectCol.W && posX + skreeCol.W > objectCol.X
			&& skreeCol.Y < objectCol.Y + objectCol.H && skreeCol.Y + skreeCol.H > objectCol.Y)
		{
			movement = EMBEDDED;
		}
	}
}

bool Skree::CheckCollision(MetRect checkRect)
{
	if (collisionRect.X < checkRect.X + checkRect.W && collisionRect.X + collisionRect.W > checkRect.X
		&& collisionRect.Y < checkRect.Y + checkRect.H && collisionRect.Y + collisionRect.H > checkRect.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Skree::IsDead()
{
	return dead;
}

bool Skree::CheckBeamCollision(MetRect checkRect)
{
	if (collisionRect.X < checkRect.X + checkRect.W && collisionRect.X + collisionRect.W > checkRect.X
		&& collisionRect.Y < checkRect.Y + checkRect.H && collisionRect.Y + collisionRect.H > checkRect.Y)
	{
		dead = true;
		return true;
	}
	else
	{
		return false;
	}
}
