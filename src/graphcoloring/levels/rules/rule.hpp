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

#ifndef GRAPHCOLORING_LEVELS_RULES_RULE_H_
#define GRAPHCOLORING_LEVELS_RULES_RULE_H_

#include "graphcoloring/graphs/graph.hpp"

namespace graphcoloring {
namespace rules {

class Rule {
public:
	virtual ~Rule() {}
	virtual bool ObeysRule(const Graph& graph) const = 0;
	virtual int Render(gui::Window* window, int x, int y, int width) const = 0; // Returns height
};

} // namespace rules
} // namespace graphcoloring

#endif /* GRAPHCOLORING_LEVELS_RULES_RULE_H_ */
