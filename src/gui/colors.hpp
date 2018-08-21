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

#ifndef GRAPHCOLORING_GUI_COLORS_H_
#define GRAPHCOLORING_GUI_COLORS_H_

#include <cstdint>
#include <string>

#include "pugi/pugixml.hpp"

namespace gui {

typedef uint32_t Color;

namespace colors {

// Color constants
static constexpr Color WHITE =   0xFFFFFFFF;
static constexpr Color BLACK =   0x000000FF;
static constexpr Color RED =     0xFF0000FF;
static constexpr Color GREEN =   0x00FF00FF;
static constexpr Color BLUE =    0x0000FFFF;
static constexpr Color YELLOW =  0xFFFF00FF;
static constexpr Color CYAN =    0x00FFFFFF;
static constexpr Color MAGENTA = 0xFF00FFFF;

extern void Unpack(Color color,
    uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a); // 32-bit color => 8-bit channels
extern Color Pack(uint8_t r, uint8_t g, uint8_t b, uint8_t a); // 8-bit channels => 32-bit color
extern Color Shade(Color color, double factor);
extern Color Light(Color color);
extern Color Dark(Color color);
extern Color FromString(const std::string& str); // Reads #FF00AA or #AA33FFCC
extern Color FromAttribute(pugi::xml_attribute attr);

} // namespace colors
} // namespace gui

#endif // GRAPHCOLORING_GUI_COLORS_H_
