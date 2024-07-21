#pragma once
#include "world_config.h"
#include "../utils/vector2.h"
#include "game_object.h"

class Grid sealed
{
public:
	Grid(sf::RenderTarget& target);
	~Grid();
	Grid(const Grid&) = delete;

	void set_pixel(const float& x, const float& y, const sf::Color& color);
	void set_pixel(const game_object& object);
private:
	sf::RenderTarget& m_Target;

	sf::RectangleShape shape;

	const game_type size = 10;
};

