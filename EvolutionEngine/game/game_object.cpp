#include "game_object.h"

game_object::game_object()
{
}

game_object::~game_object()
{
}

void game_object::move(const vector2& movement)
{
	this->position += movement;
}

void game_object::destroy()
{
	this->alive = false;
}
