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

#include "colors.hpp"
#include "utils/errors.hpp"

#include <iostream>


#define SHADE_FACTOR 1.5

namespace gui {
namespace colors {

void Unpack(Color color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a)
{
    *r = (color >> 24) & 0xFF;
    *g = (color >> 16) & 0xFF;
    *b = (color >>  8) & 0xFF;
    *a = (color >>  0) & 0xFF;
}

Color Pack(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return (r << 24) + (g << 16) + (b << 8) + (a << 0);
}

static uint8_t shade_channel(uint8_t channel, double factor)
{
    if (channel * factor > 0xFF)
        channel = 0xFF;
    else
        channel *= factor;
    return channel;
}

Color Shade(Color color, double factor)
{
    uint8_t r, g, b, a;
    Unpack(color, &r, &g, &b, &a);
    r = shade_channel(r, factor);
    g = shade_channel(g, factor);
    b = shade_channel(b, factor);
    a = shade_channel(a, factor);
    return Pack(r, g, b, a);
}

Color Light(Color color)
{
    return Shade(color, SHADE_FACTOR);
}

Color Dark(Color color)
{
    return Shade(color, 1/SHADE_FACTOR);
}

Color FromString(const std::string& str)
{
	uint8_t r, g, b, a;
	try
	{
		r = std::stoi(str.substr(1,2),nullptr,16);
		g = std::stoi(str.substr(3,2),nullptr,16);
		b = std::stoi(str.substr(5,2),nullptr,16);
		if (str.length() >= 9)
			a = std::stoi(str.substr(7,2),nullptr,16);
		else
			a = 255;

		return Pack(r, g, b, a);
	}
	catch (std::invalid_argument&)
	{
		utils::errors::Die("Invalid color string: " + str);
	}
	return 0;
}

Color FromAttribute(pugi::xml_attribute attr)
{
	return FromString(std::string(attr.value()));
}

} // namespace colors
} // namespace gui
