#include "vector2.h"

vector2::vector2()
{
	set_x(1);
	set_y(1);
}

vector2::vector2(const game_type& x, const game_type& y)
{
	set_x(x);
	set_y(y);
}
