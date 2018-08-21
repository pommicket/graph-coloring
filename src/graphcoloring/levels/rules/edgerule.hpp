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


#ifndef GRAPHCOLORING_LEVELS_RULES_EDGERULE_H_
#define GRAPHCOLORING_LEVELS_RULES_EDGERULE_H_

#include "graphcoloring/graphs/graph.hpp"
#include "rules.hpp"
#include "rule.hpp"

namespace graphcoloring {
namespace rules {

class EdgeRule : public Rule {
public:
	EdgeRule(gui::Color vertex_color1 = ANY_COLOR,
			 gui::Color vertex_color2 = ANY_COLOR,
			 gui::Color edge_color    = ANY_COLOR);
	EdgeRule(pugi::xml_node node, const ColorLoader& color_loader);
	void LoadFromNode(pugi::xml_node node, const ColorLoader& color_loader);
	bool ObeysRule(const Graph& graph) const;
	int Render(gui::Window* window, int x, int y, int width) const;
	virtual ~EdgeRule() {}
private:
	bool ObeysRule(const Edge& edge) const;
	gui::Color vertex_color1;
	gui::Color vertex_color2;
	gui::Color edge_color;

};

} // namespace rules
} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_RULES_EDGERULE_H_
