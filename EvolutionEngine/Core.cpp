#include "Core.h"

#include <mutex>
#include "game/Grid.h"
#include "game/SimulationSettings.h"

#include "imgui/imgui.h"
#include "imgui/imgui-sfml.h"
#include <iostream>


Core::Core()
	: _renderThread(&Core::render, this)
{
}

Core::~Core()
{
	ImGui::SFML::Shutdown();
	delete _world;
}


std::mutex m;

void Core::run()
{
	_window = new sf::RenderWindow(sf::VideoMode(1200, 720), "Evolution Engine");
	_window->setFramerateLimit(30);
	_window->setActive(false);
	ImGui::SFML::Init(*_window);
	ImGui::SFML::SetCurrentWindow(*_window);

	_world = new world();

	_renderThread.launch();


	while (_window->isOpen()) {

		pollEvents();

		m.lock();
		_world->update();
		m.unlock();
	}

	
}

void Core::render()
{
	_window->setActive(true);

	Grid grid(*_window);

	sf::Clock deltaClock;

	world& cur_world = *_world;

	while (_window->isOpen()) {

		m_Window->clear();
		ImGui::SFML::Update(*m_Window, deltaClock.restart());
		
		m.lock();

		size_t size = cur_world.game_objects.size();
		for (size_t i = 0; i < size; ++i)
		{
			grid.set_pixel(*cur_world.game_objects[i]);

		}

		drawSimulationSettings(cur_world);

		m.unlock();

		ImGui::SFML::Render(*m_Window);
		m_Window->display();
	}
}

void Core::pollEvents()
{
	sf::Event event;
	while (_window->pollEvent(event)) {
		ImGui::SFML::ProcessEvent(*_window, event);
		if (event.type == sf::Event::Closed)
		{
			_window->close();
		}
	}
}

void Core::drawSimulationSettings(world& cur_world)
{
	ImGui::Begin("Simulation editor");
	ImGui::InputInt("Max food", &cur_world.world_settings.max_food);
	ImGui::InputInt("Food from light", &cur_world.world_settings.food_from_light);
	ImGui::InputInt("Light level", &cur_world.world_settings.light_level);
	ImGui::InputInt("Food for kill", &cur_world.world_settings.food_for_kill);
	ImGui::InputInt("Walk cost", &cur_world.world_settings.walk_cost);
	ImGui::InputInt("Attack cost", &cur_world.world_settings.attack_cost);
	ImGui::InputInt("Create cost", &cur_world.world_settings.food_for_create_new_bot);
	ImGui::InputInt("Food when start", &cur_world.world_settings.food_when_start);
	ImGui::Separator();
	ImGui::InputInt("Max bot age", &cur_world.world_settings.max_age_steps, 50);
	ImGui::Separator();
	ImGui::InputInt("Chance evolition", &cur_world.world_settings.chance_evolution, 2);
	ImGui::Checkbox("Super evoliton", &cur_world.world_settings.many_evolition);
	ImGui::Separator();
	ImGui::InputInt("Bot brain size", &cur_world.world_settings.bot_brain_size);
	ImGui::Separator();
	ImGui::Text(("Count of steps: " + std::to_string(cur_world.steps / 100)).c_str());
	ImGui::Separator();

	if (ImGui::Button("Regenerate")) {
		cur_world.generate();
	}
	if (ImGui::Button("Save world")) {
		cur_world.save_world();
	}
	ImGui::End();
}
