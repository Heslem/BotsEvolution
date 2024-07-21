#pragma once
#include "random"
#include "../game/world_config.h"

extern "C" {
	typedef int game_type;
}

struct vector2
{
private:
	game_type x_;
	game_type y_;

public:
	vector2();
	vector2(const game_type& x, const game_type& y);

	const game_type& get_x() const noexcept {
		return x_;
	}

	const game_type& get_y() const noexcept {
		return y_;
	}

	void set_x(const game_type& x) {
		if (x > world_size_x - 1) {
			x_ = world_size_x - 1;
		}
		else if (x < 1) {
			x_ = 1;
		}
		else {
			x_ = x;
		}
	}

	void set_y(const game_type& y) {
		if (y > world_size_y - 1) {
			y_ = world_size_y - 1;
		}
		else if (y < 1) {
			y_ = 1;
		}
		else {
			y_ = y;
		}
	}

	static const vector2 random() {
		return vector2(1 + rand() % world_size_x, 1 + rand() % world_size_y);
	}

	vector2& operator += (const vector2 b)
	{
		this->x_ += b.x_;
		this->y_ += b.y_;
		return *this;
	}
};

