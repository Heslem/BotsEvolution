#pragma once

typedef short brain_type;

namespace simulation {
	struct settings {
		settings() { }
		~settings() { }

		game_type max_age_steps = 3400;
		game_type chance_evolution = 32;

		game_type food_when_start = 200;
		game_type food_for_create_new_bot = 400;
		game_type food_from_light = 8;

		game_type max_food = 500;
		game_type food_for_kill = 120;

		game_type light_level = 25;
		game_type walk_cost = 1;
		game_type attack_cost = 190; // if bot don't kill bot, he loss attack cost
		game_type max_diff_brain = 0;

		bool many_evolition = true;
		game_type bot_brain_size = 128;
		bool can_attack_similar_bot = false;
	};

}