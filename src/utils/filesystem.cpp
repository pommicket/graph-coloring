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


#include "filesystem.hpp"

#include <cstdio>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>

#include "errors.hpp"
#include "windows.hpp"

#ifdef WINDOWS
#include <windows.h>
#endif

namespace utils {
namespace filesystem {

void remove_file(std::string path)
{
	if (remove(path.c_str()))
		utils::errors::Die(std::string("Failed to delete ") + path);
}

void copy_file(std::string src, std::string dest)
{
	std::ifstream source(src, std::ios::binary);
	std::ofstream destination(dest, std::ios::binary);

	destination << source.rdbuf();
}

void create_directory(std::string path)
{
#ifdef WINDOWS
	mkdir(path.c_str());
#else
	mkdir(path.c_str(), 0777);
#endif
}

bool directory_exists(std::string path)
{
	DIR* dir = opendir(path.c_str());
	if (dir)
	{
		closedir(dir);
		return true;
	}
	return false;
}

} // namespace filesystem
} // namespace utils
