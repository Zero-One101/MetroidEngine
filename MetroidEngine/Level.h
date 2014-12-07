#include "Tmx.h"

class Level
{
public:
	Level();
	virtual ~Level();
	void LoadMap(char* path);
	Tmx::Map* GetMap();

private:
	Tmx::Map* map;
};
