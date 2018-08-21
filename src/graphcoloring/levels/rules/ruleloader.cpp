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

#include "ruleloader.hpp"

#include "graphcoloring/graphcoloring.hpp"

namespace graphcoloring {

RuleLoader::RuleLoader() {}

void RuleLoader::LoadDocument(const pugi::xml_document& document,
		const ColorLoader& color_loader)
{
	for (pugi::xml_node rule_node : document.child("rules").children())
	{
		std::string name = rule_node.name();
		if (name == "connected")
			connected_rule = true;
		// These node methods will check the name of the node.
		LoadEdgeRule(rule_node, color_loader);
		LoadMaximumRule(rule_node, color_loader);

	}
}

void RuleLoader::LoadEdgeRule(pugi::xml_node node,
	const ColorLoader& color_loader)
{
	std::string name = node.name();
	if (name != "edge-rule") return;
	rules::EdgeRule rule(node, color_loader);
	edge_rules.push_back(rule);
	all_rules.push_back(std::make_unique<rules::EdgeRule>(rule));
}

void RuleLoader::LoadMaximumRule(pugi::xml_node node,
	const ColorLoader& color_loader)
{
	std::string name = node.name();
	if (name != "vertex-maximum" && name != "edge-maximum"
	 && name != "vertex-minimum" && name != "edge-minimum")
		return;
	rules::BoundRule rule(node, color_loader);
	maximum_rules.push_back(rule);
	all_rules.push_back(std::make_unique<rules::BoundRule>(rule));
}


bool RuleLoader::IsValid(const Graph& graph) const
{
	for (auto& rule : all_rules)
		if (!rule->ObeysRule(graph))
			return false;
	if (connected_rule && !graph.IsConnected())
		return false;
	return true;
}

void RuleLoader::RenderRules(gui::Window* window) const
{
	window->SetDrawColor(GraphColoring::BACKGROUND_COLOR);
		window->Clear();
	int x = 10, y = 10;
	for (auto& rule : all_rules)
	{
		if (y >= window->GetHeight()-2*Vertex::VERTEX_RADIUS-10)
		{
			y = 10;
			x += 50 + RULE_COLUMN_WIDTH;
		}
		int height = rule->Render(window, x, y, RULE_COLUMN_WIDTH);
		y += height + 10;
	}
}

} // namespace graphcoloring
