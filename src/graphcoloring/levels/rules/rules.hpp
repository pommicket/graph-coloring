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


#ifndef GRAPHCOLORING_LEVELS_RULES_RULES_H_
#define GRAPHCOLORING_LEVELS_RULES_RULES_H_

#include "pugi/pugixml.hpp"
#include "../colorloader.hpp"

namespace graphcoloring {
namespace rules {

constexpr gui::Color ANY_COLOR = 0;
constexpr gui::Color SAME_COLOR = 1; // Refers to patterns like same-red-same, which can mean blue-red-blue, red-red-red, etc.
extern void ResetSameColor(); // Forget about "same" color
extern bool IsSameColor(gui::Color color1, gui::Color color2);
extern gui::Color RenderColor(gui::Color color); // Turns ANY_COLOR into white.
extern gui::Color ColorFromAttribute(pugi::xml_attribute attr,
	const ColorLoader& color_loader); // ANY_COLOR if attribute is empty.

} // namespace rules
} // namespace graphcoloring



#endif /* SRC_GRAPHCOLORING_LEVELS_RULES_RULES_HPP_ */
