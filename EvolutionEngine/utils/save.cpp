#include "save.h"
#include <iostream>
#include <fstream>


void save::save_objects(std::vector<game_object*> game_objects, const char* filename)
{
	std::ofstream outfile;
	outfile.open(filename, std::ofstream::out | std::ofstream::trunc);

	for (size_t i = 0; i < game_objects.size(); ++i)
	{
		outfile << game_objects[i]->get_save().c_str() << std::endl << "end." << std::endl;
	}

	outfile.close();
}
