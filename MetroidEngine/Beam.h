#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

#include "MetRect.h"
#include "Tmx.h"

class Beam
{
public:
	Beam(int direction, int posX, int posY);
	virtual ~Beam();
	void Update();
	void Draw();
	bool IsDead();
	void CheckCollision(const Tmx::Object* collisionObject);
	MetRect GetCollisionRect();
	void Hit();
private:
	ALLEGRO_BITMAP* beamTexture = NULL;
	int posX;
	int posY;
	int nextPosX;
	int nextPosY;
	int beamWidth;
	int beamHeight;
	float moveSpeed = 3.0;
	bool dead = false;
	enum Direction
	{
		LEFT = 0,
		RIGHT,
		UP
	};
	Direction direction;
	MetRect collisionRect;
};

