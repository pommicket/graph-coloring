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

#include "rules.hpp"

namespace graphcoloring {
namespace rules {

static gui::Color same_color = ANY_COLOR;

void ResetSameColor() { same_color = ANY_COLOR; }

bool IsSameColor(gui::Color color1, gui::Color color2)
{
	if (color1 == ANY_COLOR || color2 == ANY_COLOR)
		return true;

	if (same_color == ANY_COLOR)
	{
		if (color1 == SAME_COLOR && color2 != SAME_COLOR)
		{
			same_color = color2;
			return true;
		}
		if (color2 == SAME_COLOR && color1 != SAME_COLOR)
		{
			same_color = color1;
			return true;
		}
	}
	if (color1 == SAME_COLOR && color2 == same_color)
		return true;
	if (color2 == SAME_COLOR && color1 == same_color)
		return true;

	return color1 == color2;
}

gui::Color RenderColor(gui::Color color)
{
	if (color == ANY_COLOR)
		return gui::colors::WHITE;
	if (color == SAME_COLOR)
		return 0x666666FF;
	return color;
}

gui::Color ColorFromAttribute(pugi::xml_attribute attr,
	const ColorLoader& color_loader)
{
	if (attr.empty()) return ANY_COLOR;
	if (std::string(attr.value()) == "same") return SAME_COLOR;
	return attr.empty() ? ANY_COLOR : color_loader.GetColorFromAttribute(attr);
}

} // namespace rules
} // namespace graphcoloring
