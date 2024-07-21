#include "Grid.h"
#include <string>
Grid::Grid(sf::RenderTarget& target)
	: m_Target(target)
{
	shape.setSize(sf::Vector2f(size, size));
	shape.setOutlineThickness(1.0f);
	shape.setOutlineColor(sf::Color::Black);
}

Grid::~Grid()
{
}

void Grid::set_pixel(const float& x, const float& y, const sf::Color& color)
{
	shape.setPosition(x * size, y * size);
	shape.setFillColor(color);
	m_Target.draw(shape);
}

void Grid::set_pixel(const game_object& object)
{
	shape.setPosition(object.position.get_x() * size, object.position.get_y() * size);
	shape.setFillColor(object.getColor());
	m_Target.draw(shape);
}
