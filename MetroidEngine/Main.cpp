#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>
#include <vector>
#include <algorithm>

#include "main.h"
#include "Tmx.h"
#include "Level.h"
#include "Samus.h"
#include "Skree.h"
#include "Beam.h"

int count = 0;
ALLEGRO_FONT* arial14;
ALLEGRO_EVENT ev;
const int foregroundLayer = 1;

bool SkreeDead(Skree& skree)
{
	return skree.IsDead();
}

bool BeamDead(Beam& beam)
{
	return beam.IsDead();
}

void DrawLayer(int layer, Tmx::Map* map, int width, int height, int tilesX, int tileWidth, int tileHeight, ALLEGRO_BITMAP* tileset)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int tileX = (map->GetLayer(layer)->GetTileId(x, y) % tilesX) * tileWidth;
			int tileY = (map->GetLayer(layer)->GetTileId(x, y) / tilesX) * tileHeight;

			al_draw_bitmap_region(tileset, tileX, tileY, tileWidth, tileHeight, x * tileWidth, y * tileHeight, 0);

		}
	}
}

void Update(Tmx::Map* map, Samus& samus, std::vector<Skree>& skrees, std::vector<Beam>& beams)
{
	const Tmx::ObjectGroup *objectGroup = map->GetObjectGroup(0);
	samus.Update(ev);

	// Iterate through all objects in the collision object group
	for (int j = 0; j < objectGroup->GetNumObjects(); j++)
	{
		// Get an object
		const Tmx::Object* object = objectGroup->GetObjectA(j);			
		samus.CheckCollision(object);	
		for (std::vector<Skree>::iterator iter = skrees.begin(); iter != skrees.end(); iter++)
		{
			iter->CheckCollision(object);
			for (std::vector<Beam>::iterator beamIter = beams.begin(); beamIter != beams.end(); beamIter++)
			{
				if (iter->CheckBeamCollision(beamIter->GetCollisionRect()))
				{
					beamIter->Hit();
				}
			}
		}
		for (std::vector<Beam>::iterator iter = beams.begin(); iter != beams.end(); iter++)
		{
			iter->CheckCollision(object);
		}
	}

	for (std::vector<Skree>::iterator iter = skrees.begin(); iter != skrees.end(); iter++)
	{
		iter->Update(samus.GetCollisionRect());
		if (iter->CheckCollision(samus.GetCollisionRect()))
		{
			samus.Hit();
		}
	}

	for (std::vector<Beam>::iterator iter = beams.begin(); iter != beams.end(); iter++)
	{
		iter->Update();
	}

	if (samus.IsFiring())
	{
		Beam beam = Beam(samus.AimDirection(), samus.GetCollisionRect().X, samus.GetCollisionRect().Y + 6);
		beams.push_back(beam);
	}

	skrees.erase(std::remove_if(std::begin(skrees), std::end(skrees), SkreeDead), std::end(skrees));
	beams.erase(std::remove_if(std::begin(beams), std::end(beams), BeamDead), std::end(beams));

	al_flip_display(); //TODO: Move this back into Draw() Can't use debug mode if I move it back
}

void Draw(int width, int height, int layers, Tmx::Map* map, int tilesX, int tileWidth, int tileHeight, ALLEGRO_BITMAP* tileset, Samus& samus, std::vector<Skree>& skrees, std::vector<Beam>& beams)
{
	//count++;
	al_clear_to_color(al_map_rgb(0, 0, 0));

	DrawLayer(0, map, width, height, tilesX, tileWidth, tileHeight, tileset);
	samus.Draw(arial14);
	for (std::vector<Skree>::iterator iter = skrees.begin(); iter != skrees.end(); iter++)
	{
		iter->Draw();
	}
	for (std::vector<Beam>::iterator iter = beams.begin(); iter != beams.end(); iter++)
	{
		iter->Draw();
	}
	DrawLayer(1, map, width, height, tilesX, tileWidth, tileHeight, tileset);
	//al_draw_textf(arial14, al_map_rgb(255, 0, 255), 320, 320, 0, "Frames: %i", count);
}

int main(void)
{
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_BITMAP* tileset = NULL;
	ALLEGRO_EVENT_QUEUE* eventQueue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	int screenWidth = 640;
	int screenHeight = 640;
	int FPS = 60;
	bool done = false;
	std::vector<Skree> skrees;
	std::vector<Beam> beams;
	
	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "Failed to initalise Allegro!", NULL, NULL);
		return -1;
	}

	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_install_keyboard();

	al_reserve_samples(5);

	display = al_create_display(screenWidth, screenHeight);

	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Failed to initialise display!", NULL, NULL);
		return -2;
	}

	arial14 = al_load_ttf_font(".//assets//fonts//Arial.ttf", 14, 0);

	Tmx::Map* map;
	Level level;
	level.LoadMap("./assets/levels/GameMap2.tmx");
	map = level.GetMap();
	const Tmx::Tileset *tmxTileset = map->GetTileset(0);
	int tileWidth;
	int tileHeight;
	int tilesetWidth;
	int tilesetHeight;
	int tilesX;
	int tilesY;	

	// How many layers the map has
	int layers = map->GetNumLayers();

	// Get first layer
	const Tmx::Layer* currentLayer = map->GetLayer(0);

	int width = currentLayer->GetWidth();
	int height = currentLayer->GetHeight();
	
	tileWidth = map->GetTileWidth();
	tileHeight = map->GetTileHeight();

	printf("Map dimensions: %i by %i\r\n", width, height);
	printf("Tile dimensions: %i by %i\r\n", tileWidth, tileHeight);	

	tileset = al_load_bitmap(".\\assets\\tilesets\\MetroidII.png");
	al_convert_mask_to_alpha(tileset, al_map_rgb(255, 0, 255));
	tilesetWidth = al_get_bitmap_width(tileset);
	tilesetHeight = al_get_bitmap_height(tileset);
	tilesX = tilesetWidth / tileWidth;
	tilesY = tilesetHeight / tileHeight;

	const Tmx::ObjectGroup *spawnGroup = map->GetObjectGroup(1);
	const Tmx::Object *samusSpawn = spawnGroup->GetObjectA(0);
	printf("Object name: %s", samusSpawn->GetName().c_str());
	printf("Object type: %s", samusSpawn->GetType().c_str());

	Samus samus = Samus(samusSpawn->GetX(), samusSpawn->GetY());

	for (int i = 1; i < spawnGroup->GetNumObjects(); i++)
	{
		const Tmx::Object *skreeSpawn = spawnGroup->GetObjectA(i);
		Skree skree = Skree(skreeSpawn->GetX(), skreeSpawn->GetY());
		skrees.push_back(skree);
	}

	timer = al_create_timer(1.0 / FPS);

	eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(display));

	al_start_timer(timer);

	while (!done)
	{
		al_wait_for_event(eventQueue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		Update(map, samus, skrees, beams);
		Draw(width, height, layers, map, tilesX, tileWidth, tileHeight, tileset, samus, skrees, beams);
	}

	al_destroy_bitmap(tileset);
	al_destroy_display(display);

	return 0;
}