#pragma once
#include "sfml.h"
#include "game/world.h"

class Core
{
	Core();

	void render();
	void pollEvents();
	void drawSimulationSettings(world& cur_world);

	sf::RenderWindow* _window;
	sf::Thread _renderThread;

	world* _world;
public:
	void operator=(Core const&) = delete;
	Core(const Core&) = delete;
	~Core();

	static Core& getInstance()
	{
		static Core instance;
		return instance;
	}

	void run();
};

