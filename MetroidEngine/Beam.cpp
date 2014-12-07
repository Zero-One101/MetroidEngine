#include "Beam.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

Beam::Beam(int direction, int posX, int posY)
{
	beamTexture = al_load_bitmap(".\\assets\\sprites\\beam.png");
	al_convert_mask_to_alpha(beamTexture, al_map_rgb(255, 0, 255));
	dead = false;
	if (direction == 0)
	{
		this->direction = UP;
		beamWidth = 4;
		beamHeight = 6;
	}
	else if (direction == -1)
	{
		this->direction = LEFT;
		beamWidth = 6;
		beamHeight = 4;
	}
	else if (direction == 1)
	{
		this->direction = RIGHT;
		beamWidth = 6;
		beamHeight = 4;
	}
	this->posX = posX;
	this->posY = posY;
	collisionRect.X = this->posX;
	collisionRect.Y = this->posY;
	collisionRect.W = beamWidth;
	collisionRect.H = beamHeight;
}

Beam::~Beam()
{
	//al_destroy_bitmap(beamTexture);
}

void Beam::Update()
{
	if (!dead)
	{
		if (direction == UP)
		{
			posY -= moveSpeed;
		}
		else if (direction == LEFT)
		{
			posX -= moveSpeed;
		}
		else if (direction == RIGHT)
		{
			posX += moveSpeed;
		}

		collisionRect.X = posX;
		collisionRect.Y = posY;
	}
}

void Beam::Draw()
{
	if (!dead)
	{
		//al_draw_rectangle(collisionRect.X, collisionRect.Y, collisionRect.X + collisionRect.W, collisionRect.Y + collisionRect.H, al_map_rgb(0, 255, 255), 1);
		if (direction != UP)
		{
			al_draw_bitmap_region(beamTexture, 0, 2, beamWidth, beamHeight, posX, posY, 0);
		}
		else
		{
			al_draw_bitmap_region(beamTexture, 8, 0, beamWidth, beamHeight, posX, posY, 0);
		}
	}
}

bool Beam::IsDead()
{
	return dead;
}

MetRect Beam::GetCollisionRect()
{
	return collisionRect;
}

void Beam::Hit()
{
	dead = true;
}

void Beam::CheckCollision(const Tmx::Object* collisionObject)
{
	MetRect objectCol(collisionObject->GetX(), collisionObject->GetY(), collisionObject->GetWidth(), collisionObject->GetHeight());

	if (collisionRect.X < objectCol.X + objectCol.W && collisionRect.X + collisionRect.W > objectCol.X
		&& collisionRect.Y < objectCol.Y + objectCol.H && collisionRect.Y + collisionRect.H > objectCol.Y)
	{
		dead = true;
	}
}
