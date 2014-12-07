#include "Samus.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <math.h>

Samus::Samus(int posX, int posY)
{
	this->posX = posX;
	this->posY = posY;
	nextPosX = this->posX;
	nextPosY = this->posY;
	topPose = T_SPAWNING;
	bottomPose = B_SPAWNING;
	collisionRect.X = this->posX;
	collisionRect.Y = this->posY;
	collisionRect.W = samusWidth;
	collisionRect.H = samusHeight;
	immunePassed = 0;
	immunityFrames = 30;
	currentSpawnFrames = 0;
	spawnSample = al_load_sample(".//assets//audio//MII_Start.wav");
	sr388BeginSample = al_load_sample(".//assets//audio//MII_BeginSR388_Start.wav");
	sr388BeginInstance = al_create_sample_instance(sr388BeginSample);
	al_attach_sample_instance_to_mixer(sr388BeginInstance, al_get_default_mixer());
	sr388LoopSample = al_load_sample(".//assets//audio//MII_BeginSR388_Loop.wav");
	al_play_sample(spawnSample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
	samusSprites = al_load_bitmap(".//assets//sprites//samus.png");
	al_convert_mask_to_alpha(samusSprites, al_map_rgb(255, 0, 255));
}

Samus::~Samus()
{
	al_destroy_bitmap(samusSprites);
	al_destroy_bitmap(topPoseGFX);
	al_destroy_bitmap(bottomPoseGFX);
	al_destroy_sample(spawnSample);
	al_destroy_sample(sr388BeginSample);
	al_destroy_sample(sr388LoopSample);
}

void Samus::Update(ALLEGRO_EVENT event)
{
	if (spawned)
	{
		if (!al_get_sample_instance_playing(sr388BeginInstance) && !playingLoop)
		{
			al_play_sample(sr388LoopSample, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
			playingLoop = true;
		}
	}

	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		if (event.keyboard.keycode == ALLEGRO_KEY_X)
		{
			if (!hasJumped)
			{
				moveSpeedY -= jumpStrength;
				if (moveSpeedX != 0)
				{
					spinJump = true;
				}
				else
				{
					spinJump = false;
				}
			}
		}
		if (event.keyboard.keycode == ALLEGRO_KEY_Z && spawned)
		{
			firing = true;
		}
		if (event.keyboard.keycode == ALLEGRO_KEY_UP)
		{
			previousAim = aiming;
			aiming = 0; // up
		}
		if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
		{

		}
		if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
		{
			moveSpeedX -= maxMoveSpeed;
			direction = LEFT;
			aiming = -1; // left
		}
		if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
		{
			moveSpeedX += maxMoveSpeed;
			direction = RIGHT;
			aiming = 1; // right
		}
		if (event.keyboard.keycode == ALLEGRO_KEY_D)
		{
			debug = !debug;
		}
	}
	else if (event.type == ALLEGRO_EVENT_KEY_UP)
	{
		if (event.keyboard.keycode == ALLEGRO_KEY_UP)
		{
			//moveSpeedY = 0;
			aiming = previousAim;
		}
		if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
		{
			//moveSpeedY = 0;
		}
		if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
		{
			moveSpeedX += maxMoveSpeed;
		}
		if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
		{
			moveSpeedX -= maxMoveSpeed;
		}
	}

	if (event.type == ALLEGRO_EVENT_TIMER)
	{
		if (!spawned)
		{
			currentSpawnFrames++;
			if (currentSpawnFrames == spawnFrames)
			{
				spawned = true;
				currentSpawnFrames = 0;
				al_play_sample_instance(sr388BeginInstance);
				direction = LEFT;
			}
		}
		if (spawned)
		{
			posX = nextPosX;
			posY = nextPosY;
		}
		collisionRect.X = posX;
		collisionRect.Y = posY;
		if (immune)
		{
			immunePassed++;
			if (immunePassed == immunityFrames)
			{
				immune = false;
				immunePassed = 0;
			}
		}
	}

	if (applyGravity)
	{
		moveSpeedY += gravity;
		if (debug)
		{
			al_draw_filled_rectangle(20, 100, 30, 110, al_map_rgb(0, 255, 0));
		}
		hasJumped = true;
	}

	applyGravity = true;
}

void Samus::Draw(ALLEGRO_FONT* arial14)
{
	// Name shadowing. Terrible and lazy, but it works
	int samusWidth = 32;
	int samusHeight = 48;
	int posX = this->posX - 8;
	int posY = this->posY - 16;

	if (immunePassed % 2 == 0)
	{
		if (!spawned)
		{
			al_draw_bitmap_region(samusSprites, 0, 0, samusWidth, samusHeight, posX, posY, 0);
		}
		else
		{
			if (moveSpeedX == 0 && moveSpeedY == 0)
			{
				if (direction == LEFT)
				{
					al_draw_bitmap_region(samusSprites, 32, 0, samusWidth, samusHeight, posX, posY, ALLEGRO_FLIP_HORIZONTAL);
				}
				else
				{
					al_draw_bitmap_region(samusSprites, 32, 0, samusWidth, samusHeight, posX, posY, 0);
				}
			}
			else if (moveSpeedX != 0 && moveSpeedY == 0)
			{
				if (direction == LEFT)
				{
					al_draw_bitmap_region(samusSprites, currentFrame * samusWidth, samusHeight, samusWidth, samusHeight, posX, posY, ALLEGRO_FLIP_HORIZONTAL);
				}
				else
				{
					al_draw_bitmap_region(samusSprites, currentFrame * samusWidth, samusHeight, samusWidth, samusHeight, posX, posY, 0);
				}

				currentFrameTime++;
				if (currentFrameTime == frameTime)
				{
					currentFrame++;
					currentFrameTime = 0;
					if (currentFrame > 2)
					{
						currentFrame = 0;
					}
				}
			}

			if (hasJumped && !spinJump)
			{
				if (direction == LEFT)
				{
					al_draw_bitmap_region(samusSprites, samusWidth * 3, 0, samusWidth, samusHeight, posX, posY, ALLEGRO_FLIP_HORIZONTAL);
				}
				else
				{
					al_draw_bitmap_region(samusSprites, samusWidth * 3, 0, samusWidth, samusHeight, posX, posY, 0);
				}
			}
			else if (hasJumped && spinJump)
			{
				if (direction == LEFT)
				{
					al_draw_bitmap_region(samusSprites, currentSJumpFrame * samusWidth, samusHeight * 2, samusWidth, samusHeight, posX, posY, ALLEGRO_FLIP_HORIZONTAL);
				}
				else
				{
					al_draw_bitmap_region(samusSprites, currentSJumpFrame * samusWidth, samusHeight * 2, samusWidth, samusHeight, posX, posY, 0);
				}

				currentFrameTime++;
				if (currentFrameTime == frameTime)
				{
					currentSJumpFrame++;
					if (currentSJumpFrame > 3)
					{
						currentSJumpFrame = 0;
					}
					currentFrameTime = 0;
				}
			}
		}
	}

	if (debug)
	{
		al_draw_textf(arial14, al_map_rgb(255, 255, 255), 20, 20, 0, "moveSpeedX: %f", moveSpeedX);
		al_draw_textf(arial14, al_map_rgb(255, 255, 255), 20, 40, 0, "moveSpeedY: %f", moveSpeedY);

		al_draw_textf(arial14, al_map_rgb(255, 255, 255), 20, 60, 0, "posX: %i", posX);
		al_draw_textf(arial14, al_map_rgb(255, 255, 255), 20, 80, 0, "posY: %i", posY);
	}

	al_draw_textf(arial14, al_map_rgb(255, 255, 255), 20, 100, 0, "Energy: %i", health);
}

void Samus::CheckCollision(const Tmx::Object *collisionObject)
{
	MetRect objectRect(collisionObject->GetX(), collisionObject->GetY(), collisionObject->GetWidth(), collisionObject->GetHeight());

	if (debug)
	{
		//al_draw_rectangle(objectPosX, objectPosY, objectWidth + objectPosX, objectHeight + objectPosY, al_map_rgb(176, 255, 0), 2);
		al_draw_rectangle(objectRect.X, objectRect.Y, objectRect.W + objectRect.X, objectRect.H + objectRect.Y, al_map_rgb(176, 255, 0), 2);
		al_draw_rectangle(collisionRect.X, collisionRect.Y, collisionRect.X + collisionRect.W, collisionRect.Y + collisionRect.H, al_map_rgb(0, 255, 0), 2);
		//al_flip_display();
	}

	nextPosX = posX + moveSpeedX;
	nextPosY = posY + moveSpeedY;

	MetRect objectCol(collisionObject->GetX(), collisionObject->GetY(), collisionObject->GetWidth(), collisionObject->GetHeight());
	MetRect samusCol(nextPosX, posY + moveSpeedY, samusWidth, samusHeight);

	if (debug)
	{
		al_draw_rectangle(samusCol.X, samusCol.Y, samusCol.X + samusCol.W, samusCol.Y + samusCol.H, al_map_rgb(0, 0, 255), 2);
	}

	// Check for X-axis collision
	if (samusCol.X < objectCol.X + objectCol.W && samusCol.X + samusCol.W > objectCol.X
		&& posY < objectCol.Y + objectCol.H && posY + samusCol.H > objectCol.Y)
	{
		//moveSpeedX = 0;
		if (moveSpeedX > 0)
		{
			posX = objectCol.X - samusCol.W;
		}
		else
		{
			posX = objectCol.X + objectCol.W;
		}
	}

	// Check for Y-axis collision
	if (posX < objectCol.X + objectCol.W && posX + samusCol.W > objectCol.X
		&& samusCol.Y < objectCol.Y + objectCol.H && samusCol.Y + samusCol.H > objectCol.Y)
	{
		if (moveSpeedY > 0)
		{
			posY = objectCol.Y - samusCol.H;
		}
		moveSpeedY = 0.0;
		applyGravity = false;
		hasJumped = false;
	}

	nextPosY = posY + moveSpeedY;
}

MetRect Samus::GetCollisionRect()
{
	return collisionRect;
}

void Samus::Hit()
{
	if (!immune)
	{
		health -= 30;
		immune = true;
	}
}

int Samus::GetHealth()
{
	return health;
}

bool Samus::IsFiring()
{
	if (firing)
	{
		firing = false; // Preventing 60 beams per second from spawning
		return true;
	}
	else
	{
		return firing;
	}
}

int Samus::AimDirection()
{
	return aiming;
}