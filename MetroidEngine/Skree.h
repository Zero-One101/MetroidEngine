#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#include "Tmx.h"
#include "MetRect.h"

class Skree
{
public:
	Skree(int posX, int posY);
	virtual ~Skree();
	void Update(MetRect samusRect);
	void Draw();
	void CheckCollision(const Tmx::Object* collisionObject);
	bool CheckCollision(MetRect collisionRect);
	bool CheckBeamCollision(MetRect checkRect);
	bool IsDead();
private:
	ALLEGRO_BITMAP* skreeSprites;
	int posX;
	int frameTime = 5;
	int currentFrameTime;
	int currentFrame;
	float posY;
	int nextPosX;
	int nextPosY;
	float moveSpeedY;
	int maxYDistance = 200;
	int skreeWidth = 16;
	int skreeHeight = 16;
	bool falling;
	bool dead;
	int dyingFrames;
	int deadFrames;
	MetRect collisionRect;
	enum Movement
	{
		HANGING = 0,
		FALLING,
		EMBEDDED
	};
	Movement movement;
};

