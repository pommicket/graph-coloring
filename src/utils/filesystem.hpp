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
#ifndef GRAPHCOLORING_UTILS_FILESYSTEM_H_
#define GRAPHCOLORING_UTILS_FILESYSTEM_H_

#include <string>

namespace utils {
namespace filesystem {

extern void remove_file(std::string path);
extern void copy_file(std::string src, std::string dest);
extern void create_directory(std::string path);
extern bool directory_exists(std::string path);

} // namespace filesystem
} // namespace utils

#endif // GRAPHCOLORING_UTILS_FILESYSTEM_H_
