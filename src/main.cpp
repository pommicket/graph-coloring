////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Leo Tenenbaum
// This file is part of GraphColoring.
//
// GraphColoring is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GraphColoring is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GraphColoring.  If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include <cstring>

#include "graphcoloring/graphcoloring.hpp"

#define GRAPHCOLORING_VERSION "GraphColoring v. 0.0.0"

int main(int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "--version"))
		{
			std::cout << GRAPHCOLORING_VERSION << std::endl;
			return 0;
		}
	}

	graphcoloring::GraphColoring graphColoring;
	graphColoring.Start();
    return 0;
}
