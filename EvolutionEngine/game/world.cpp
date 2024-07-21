#include "world.h"
#include "bot.h"
#include "../utils/save.h"
#include <string>

#define get_index(x, y) (y * world_size_x + x)

world::world()
{
	generate();
}

world::~world()
{
}

void world::update()
{
	
	if (game_objects.empty()) {
		generate();
	}

	clear_collision();
	for (std::size_t i = 0; i < this->game_objects.size(); ++i)
	{
		if (this->game_objects[i]->isAlive()) {
			this->game_objects[i]->update();
		}
		else {

			delete this->game_objects[i];
			this->game_objects.erase(this->game_objects.begin() + i);
			deadCount++;
			i--;
		}
	}

	tryToSaveWorld();

}

const bool world::is_busy_cell(const game_type& x, const game_type& y) const
{
	if (x < 1 || x > world_size_x - 2 ||
		y < 1 || y > world_size_y) return true;

	return collisions[get_index(x, y)] != -1;
}

const bool world::is_free_cell(const game_type& x, const game_type& y) const
{
	return !is_busy_cell(x, y);
}

const bool world::is_busy_cell(const vector2& position) const
{
	return is_busy_cell(position.get_x(), position.get_y());
}

const bool world::is_free_cell(const vector2& position) const
{
	return !is_busy_cell(position.get_x(), position.get_y());
}

const size_t world::get_index_game_object(const game_type& x, const game_type& y) const
{
	return (size_t)collisions[get_index(x, y)];
}

void world::save_world()
{
	static const std::string path_to_folder("saves/");

	save current_save;
	std::string name = (path_to_folder + "save_" + std::to_string(count_saves) + ".botworld");

	current_save.save_objects(game_objects, name.c_str());
	this->count_saves++;
	this->next_steps_to_save += save_interval;
}

void world::tryToSaveWorld()
{
	if (this->steps > this->next_steps_to_save) {
		save_world();
	}
	else {
		this->steps++;
	}
}

void world::generate()
{
	for (std::size_t i = 0; i < 1000; i++)
	{
		bot* b = new bot(*this);
		b->position = vector2::random();
		create(*b);
	}
	steps = 0;
	next_steps_to_save = save_interval;
}

void world::create(game_object& object)
{
	this->collisions[get_index(object.position.get_x(), object.position.get_y())] = true;
	this->game_objects.push_back(&object);
}

void world::clear_collision()
{
	for (size_t i = 0; i < world_size; i++)
	{
		collisions[i] = -1;
	}

	int size = (int)game_objects.size();
	for (int i = 0; i < size; ++i)
	{
		this->collisions[get_index(game_objects[i]->position.get_x(), game_objects[i]->position.get_y())] = i;
	}
}
