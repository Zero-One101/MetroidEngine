#pragma once
#include "Tmx.h"
#include "MetRect.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>

class Samus
{
public:
	Samus(int posX, int posY);
	virtual ~Samus();
	void Update(ALLEGRO_EVENT event);
	void Draw(ALLEGRO_FONT* arial14);
	void CheckCollision(const Tmx::Object *collisionObject);
	void Hit();
	MetRect GetCollisionRect();
	int GetHealth();
	bool IsFiring();
	int AimDirection();
private:
	int immunePassed;
	int immunityFrames;
	int health = 99;
	int nextPosX;
	int nextPosY;
	int posX;
	int posY;
	int spawnFrames = 339;
	int currentSpawnFrames;
	int frameTime = 5;
	int currentFrameTime = 0;
	int currentFrame = 0;
	int currentSJumpFrame = 0;
	int aiming;
	int previousAim;
	float moveSpeedX;
	float moveSpeedY;
	int samusWidth = 16;
	int samusHeight = 32;
	float gravity = 0.1;
	float maxMoveSpeed = 2.0;
	float jumpStrength = 3.0;
	bool applyGravity;
	bool hasJumped = false;
	bool immune = false;
	bool debug = false;
	bool spawned = false;
	bool playingLoop;
	bool firing = false;
	bool spinJump = false;
	MetRect collisionRect;
	ALLEGRO_BITMAP* samusSprites;
	ALLEGRO_BITMAP* topPoseGFX;
	ALLEGRO_BITMAP* bottomPoseGFX;
	ALLEGRO_SAMPLE* spawnSample;
	ALLEGRO_SAMPLE* sr388BeginSample;
	ALLEGRO_SAMPLE* sr388LoopSample;
	ALLEGRO_SAMPLE_INSTANCE* sr388BeginInstance;
	enum Direction
	{
		LEFT = -1,
		RIGHT = 1,
	};
	Direction direction;
	enum TopPose
	{
		T_SPAWNING = 0,
		T_STANDING,
	};
	TopPose topPose;
	enum BottomPose
	{
		B_SPAWNING = 0,
		B_STANDING,
	};
	BottomPose bottomPose;
};