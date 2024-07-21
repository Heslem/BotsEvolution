#pragma once
#include "game_object.h"
#include <vector>
#include "../utils/vector2.h"
#include "SimulationSettings.h"

constexpr unsigned long long save_interval = 3000000;

struct world final
{
public:
	world();
	~world();
	void update();
	void create(game_object& object);

	const bool is_busy_cell(const game_type& x, const game_type& y) const;
	const bool is_free_cell(const game_type& x, const game_type& y) const;

	const bool is_busy_cell(const vector2& position) const;
	const bool is_free_cell(const vector2& position) const;
	const size_t get_index_game_object(const game_type& x, const game_type& y) const;

	void save_world();
	void tryToSaveWorld();

	void generate();
	std::vector<game_object*> game_objects;

	unsigned long count_saves = 0;
	unsigned long next_steps_to_save = save_interval;
	unsigned long steps = 0;
	unsigned long deadCount = 0;

	simulation::settings world_settings;


private:
	void clear_collision();
	int collisions[world_size];
};

