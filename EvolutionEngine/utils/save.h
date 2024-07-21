#pragma once
#include <vector>
#include "../game/game_object.h"

struct save final
{
public:
	void save_objects(std::vector<game_object*> game_objects, const char* filename);
};

