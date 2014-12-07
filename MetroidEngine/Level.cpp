#include "Level.h"

Level::Level()
{
	map = new Tmx::Map();
}


Level::~Level()
{
	//delete map;
}

void Level::LoadMap(char* path)
{
	map->ParseFile(path);
	if (map->HasError())
	{
		printf("Error loading map: %s", map->GetErrorText());
	}

	printf("Loaded map\r\n");
}

Tmx::Map* Level::GetMap()
{
	return this->map;
}
