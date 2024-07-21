#include "bot.h"
#include <random>
#include <string>
#include <iostream>

bot::bot(world& my_world) : current_world(my_world), food(my_world.world_settings.food_when_start)
{
	randomize_brain();

	// start data of brain, it's need because bot's quick die without it

	brain[0] = 7;
	brain[1] = 7;
	brain[2] = 7;
	brain[3] = 5;
	brain[4] = 2;
	brain[5] = 6;
	brain[6] = 1;
	brain[7] = 5;
	brain[8] = 1;
	brain[9] = 4;
}

bot::~bot()
{
}

bot::bot(const bot& copy_bot) : current_world(copy_bot.current_world), food(copy_bot.current_world.world_settings.food_when_start), id(copy_bot.id)
{
	for (brain_type i = 0; i < BRAIN_SIZE; ++i) {
		this->brain[i] = copy_bot.brain[i];
	}
	evolition();
}

bot& bot::operator=(const bot& right_bot)
{
	for (brain_type i = 0; i < BRAIN_SIZE; ++i) {
		this->brain[i] = right_bot.brain[i];
	}

	this->food = right_bot.food;
	this->current_world = right_bot.current_world;
	this->counter = right_bot.counter;
	this->position = right_bot.position;
	this->id = right_bot.id;

	return *this;
}

void bot::update()
{
	if (steps > current_world.world_settings.max_age_steps ||
		food < 0) {
		destroy();
		return;
	}
	else {
		steps++; 
		food--;
	}

	brain_type command = brain[counter];
	if (command < MAX_COMMAND) {
		// move left
		if (command == 1) { // move -x
			if (current_world.is_free_cell(this->position.get_x() - 1, this->position.get_y())) {
				position.set_x(this->position.get_x() - 1);
				food -= current_world.world_settings.walk_cost;
			}
		}
		// move right
		else if (command == 2) { // move +x
			if (current_world.is_free_cell(this->position.get_x() + 1, this->position.get_y()))
			{
				position.set_x(this->position.get_x() + 1);
				food -= current_world.world_settings.walk_cost;
			}
		}
		// move down
		else if (command == 3) { // move +y
			if (current_world.is_free_cell(this->position.get_x(), this->position.get_y() + 1)) {
				position.set_y(this->position.get_y() + 1);
				food -= current_world.world_settings.walk_cost;
			}
		}
		// move up
		else if (command == 4) { // move -y
			if (current_world.is_free_cell(this->position.get_x(), this->position.get_y() - 1)) {
				position.set_y(this->position.get_y() - 1);
				food -= current_world.world_settings.walk_cost;
			}
		}
		// spawn new bot command
		else if (command == 5) { // spawn new bot
			if (food > current_world.world_settings.food_for_create_new_bot) {
				bool create = true;

				increase_counter();

				int x = this->position.get_x();
				int y = this->position.get_y();

				switch (brain[counter])
				{
				case 1:
					x--;
					break;
				case 2:
					x++;
					break;

				case 3:
					y++;
					break;

				case 4:
					y--;
					break;
				default:
					create = false;
					break;
				}
				if (create) {

					vector2 positionBot = vector2(x, y);
					if (positionBot.get_x() == x || position.get_y() == y) {

						if (current_world.is_free_cell(positionBot)) {
							bot* sub_bot = new bot(*this);

							sub_bot->position = positionBot;
							current_world.create(*sub_bot);

							food -= current_world.world_settings.food_for_create_new_bot;
						}
					}
				}
			}
		}
		// kill bot command
		else if (command == 6) {
			
			game_type x = this->position.get_x();
			game_type y = this->position.get_y();
			increase_counter();
			switch (brain[counter]) //getting side
			{
			case 1:
				x--;
				break;
			case 2:
				x++;
				break;

			case 3:
				y++;
				break;

			case 4:
				y--;
				break;
			}

			bool isBorder =
				((x > world_size - 1 || x < 1) ||
				(y > world_size_y - 1 || y < 1));

			if (!(x == this->position.get_x() && y == this->position.get_y()) && !isBorder) {

				if (current_world.is_busy_cell(x, y))
				{
					size_t index = current_world.get_index_game_object(x, y);
					if (index != -1) {
						if (current_world.game_objects[index]->get_id() != this->id)
						{
							current_world.game_objects[index]->destroy();
						}
						food += current_world.world_settings.food_for_kill;
					}
				}
			}
			food -= current_world.world_settings.attack_cost;

		} // kill
		// get food from light command
		else if (command == 7) { // get food
			if (food < current_world.world_settings.max_food &&
				position.get_y() < current_world.world_settings.light_level)
			{ 

				food += current_world.world_settings.food_from_light;
			}

		}
		// goto command
		else if (command == 8) { // goto
			increase_counter();
			counter = brain[counter] - 1;
		}
		// if command
		else if (command == 9) {
			increase_counter();
			brain_type conditionType = brain[counter];
			increase_counter();
			bool result = getConditionResult(conditionType);

			brain_type end_counter = brain[counter] - 1;

			if (!result) {
				counter = end_counter;
			}

		}
	}
	//color_b = counter * 3;
	increase_counter();
}

const std::string bot::get_save() const
{
	std::string brainData("");
	for (unsigned char i = 0; i < current_world.world_settings.bot_brain_size; i++) {
		brainData.append("{" + std::to_string(brain[i]) + "}");
	}

	return std::string("bot: ") + "\n" +
		"id: " + std::to_string(id) + "\n" + 
		"x: " + std::to_string(position.get_x()) + "\n" +
		"y: " + std::to_string(position.get_y()) + "\n" +
		"food: " + std::to_string(food) + "\n" +
		"brain: " + brainData;
}

static std::random_device device;
static std::mt19937 rng(device());

void bot::randomize_brain()
{
	std::uniform_int_distribution<std::mt19937::result_type> c_evo(1, current_world.world_settings.bot_brain_size);
	for (brain_type i = 0; i < current_world.world_settings.bot_brain_size; ++i) {
		brain[i] = c_evo(rng);
	}
	generate_id();
}

void bot::evolition()
{
	std::uniform_int_distribution<std::mt19937::result_type> c_evo(1, current_world.world_settings.bot_brain_size);

	std::uniform_int_distribution<std::mt19937::result_type> change_evo(0, current_world.world_settings.chance_evolution);

	if (!current_world.world_settings.many_evolition) {
		std::uniform_int_distribution<std::mt19937::result_type> b_evo(0, current_world.world_settings.bot_brain_size - 1);
		if (change_evo(rng) == current_world.world_settings.chance_evolution / 2) {
			brain[b_evo(rng)] = c_evo(rng);
		}
	}
	else {
		for (size_t i = 0; i < current_world.world_settings.bot_brain_size; ++i)
		{
			if (change_evo(rng) == current_world.world_settings.chance_evolution / 2) {
				brain[i] = c_evo(rng);
			}
		}
	}
	generate_id();
}

void bot::increase_counter()
{
	if (counter > current_world.world_settings.bot_brain_size - 1)
		counter = 0; 
	else 
		counter++;
}

const bool bot::compare_brain_bots(const game_type& a, const game_type& b)
{
	return a == b;
}

void bot::generate_id()
{
	id = 0;
	for (game_type i = 0; i < current_world.world_settings.bot_brain_size; ++i)
	{
		id += (game_type)brain[i];
	}

	color_r = 0;
	size_t i = 0;
	for (; i < 10; ++i)
	{

		if (brain[i] == 6)
			color_r += brain[i] * 5;
	}
	color_g = 0;
	for (; i < 16; ++i)
	{
		if (brain[i] == 7)
			color_g += brain[i] * 5;
	}
	color_b = 0;
	for (; i < 20; ++i)
	{
		if (brain[i] == 5)
			color_b += brain[i] * 5;
	}

}

const bool bot::getConditionResult(const game_type& conditionType) const
{
	bool result = false;

	// near bot
	if (conditionType == 1) {
		int x = this->position.get_x() - 1;
		int y = this->position.get_y();

		if (current_world.is_busy_cell(x, y)) {
			result = current_world.get_index_game_object(x, y) != -1;
		}
	}
	else if (conditionType == 2) {
		int x = this->position.get_x() + 1;
		int y = this->position.get_y();

		if (current_world.is_busy_cell(x, y)) {
			result = current_world.get_index_game_object(x, y) != -1;
		}
	}
	else if (conditionType == 3) {
		int x = this->position.get_x();
		int y = this->position.get_y() + 1;

		if (current_world.is_busy_cell(x, y)) {
			result = current_world.get_index_game_object(x, y) != -1;
		}
	}
	else if (conditionType == 4) {
		int x = this->position.get_x();
		int y = this->position.get_y() - 1;

		if (current_world.is_busy_cell(x, y)) {
			result = current_world.get_index_game_object(x, y) != -1;
		}
	}

	// is my bot 
	else if (conditionType == 5) {
		int x = this->position.get_x() - 1;
		int y = this->position.get_y();

		if (current_world.is_busy_cell(x, y)) {
			result = current_world.game_objects[current_world.get_index_game_object(x, y)]->get_id() == get_id();
		}
	}
	else if (conditionType == 6) {
		int x = this->position.get_x() + 1;
		int y = this->position.get_y();

		if (current_world.is_busy_cell(x, y)) {
			result = current_world.game_objects[current_world.get_index_game_object(x, y)]->get_id() == get_id();
		}
	}
	else if (conditionType == 7) {
		int x = this->position.get_x();
		int y = this->position.get_y() + 1;

		if (current_world.is_busy_cell(x, y)) {
			result = current_world.game_objects[current_world.get_index_game_object(x, y)]->get_id() == get_id();
		}
	}
	else if (conditionType == 8) {
		int x = this->position.get_x();
		int y = this->position.get_y() - 1;

		if (current_world.is_busy_cell(x, y)) {
			result = current_world.game_objects[current_world.get_index_game_object(x, y)]->get_id() == get_id();
		}
	}

	return result;
}
