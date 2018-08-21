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

#ifndef GRAPHCOLORING_LEVELS_RULELOADER_H_
#define GRAPHCOLORING_LEVELS_RULELOADER_H_

#include "pugi/pugixml.hpp"

#include "graphcoloring/levels/colorloader.hpp"
#include "graphcoloring/graphs/graph.hpp"
#include "boundrule.hpp"
#include "edgerule.hpp"

namespace graphcoloring {

class RuleLoader {
public:
	static constexpr gui::Color ANY_COLOR = 0x00000000;
	RuleLoader();
	virtual ~RuleLoader() {}
	void LoadDocument(const pugi::xml_document& document,
		const ColorLoader& color_loader);
	bool IsValid(const Graph& graph) const; // O(Rules * Edges)
	void RenderRules(gui::Window* window) const;
private:
	void LoadEdgeRule(pugi::xml_node node, const ColorLoader& color_loader);
	void LoadMaximumRule(pugi::xml_node node, const ColorLoader& color_loader);
	static constexpr int RULE_COLUMN_WIDTH = 200;
	std::vector<rules::EdgeRule> edge_rules;
	std::vector<rules::BoundRule> maximum_rules;
	bool connected_rule = false; // true if the graph should be connected
	std::vector<std::unique_ptr<rules::Rule>> all_rules;

};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_RULELOADER_H_
