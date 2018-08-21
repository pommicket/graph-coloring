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

#ifndef GRAPHCOLORING_LEVELS_RULES_BOUNDRULE_H_
#define GRAPHCOLORING_LEVELS_RULES_BOUNDRULE_H_

#include "graphcoloring/graphs/graph.hpp"

#include "rules.hpp"
#include "rule.hpp"

namespace graphcoloring {
namespace rules {

class BoundRule : public Rule {
public:
	BoundRule(gui::Color color = ANY_COLOR,
		int bound = 0, bool bound_type = MAXIMUM,
		bool rule_type = VERTEX_RULE);
	BoundRule(pugi::xml_node node, const ColorLoader& color_loader);
	void LoadFromNode(pugi::xml_node node, const ColorLoader& color_loader);
	virtual ~BoundRule() {}
	bool ObeysRule(const Graph& graph) const;
	int Render(gui::Window* window, int x, int y, int width) const;
private:
	bool CheckAllCounts(const Graph& graph) const;
	static constexpr bool EDGE_RULE    = false;
	static constexpr bool VERTEX_RULE  = true;
	static constexpr bool MINIMUM = false;
	static constexpr bool MAXIMUM = true;
	gui::Color color;
	int bound;
	bool bound_type;
	bool rule_type;
};

} // namespace rules
} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_RULES_BOUNDRULE_H_
