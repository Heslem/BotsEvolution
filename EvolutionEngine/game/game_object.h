#pragma once
#include "../utils/vector2.h"
#include "../sfml.h"

struct game_object
{
public:
	virtual void update() = 0;
	virtual const std::string get_save() const = 0;
	virtual const game_type& get_id() const = 0;
public:

	game_object();
	virtual ~game_object();

	void move(const vector2& movement);

	const bool& isAlive() const noexcept { return alive; }
	void destroy();



	sf::Uint8 color_r;
	sf::Uint8 color_g;
	sf::Uint8 color_b;

	const sf::Color getColor() const noexcept { return sf::Color(color_r, color_g, color_b); }

	vector2 position;
private:
	bool alive = true;
};

