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

#include "boundrule.hpp"

#include "graphcoloring/level.hpp"
#include "utils/errors.hpp"

namespace graphcoloring {
namespace rules {

BoundRule::BoundRule(gui::Color color_, int bound_, bool bound_type_,
		bool rule_type_)
	: color(color_), bound(bound_), bound_type(bound_type_),
	  rule_type(rule_type_)
{}


BoundRule::BoundRule(pugi::xml_node node, const ColorLoader& color_loader)
{
	LoadFromNode(node, color_loader);
}

void BoundRule::LoadFromNode(pugi::xml_node node,
	const ColorLoader& color_loader)
{
	std::string name(node.name());
	if (name.substr(name.length()-7) == std::string("minimum"))
		bound_type = MINIMUM;
	else
		bound_type = MAXIMUM;

	if (name.substr(0,name.length()-8) == std::string("vertex"))
		rule_type = VERTEX_RULE;
	else
		rule_type = EDGE_RULE;
	color = ColorFromAttribute(node.attribute("color"), color_loader);
	bound = node.attribute(bound_type == MINIMUM ? "min" : "max").as_int(0);
}

bool BoundRule::CheckAllCounts(const Graph& graph) const
{
	std::map<gui::Color, int> counts;
	for (gui::Color color : Level::colors)
		counts[color] = 0;
	if (rule_type == VERTEX_RULE)
	{
		for (const Vertex* v : graph.vertices)
		{
			if (counts.count(v->Color()))
				counts[v->Color()]++;
		}
	}
	else
	{
		for (const Edge* e : graph.edges)
		{
			if (counts.count(e->Color()))
				counts[e->Color()]++;
			else
				counts[e->Color()] = 1;
		}
	}
	for (std::pair<gui::Color, int> count : counts)
	{
		if (bound_type == MINIMUM && count.second < bound)
			return false;
		if (bound_type == MAXIMUM && count.second > bound)
			return false;
	}
	return true;
}

bool BoundRule::ObeysRule(const Graph& graph) const
{
	if (color == SAME_COLOR) // For same, check if any color is out of bounds.
		return CheckAllCounts(graph);

	int count = 0;
	if (rule_type == VERTEX_RULE)
	{
		for (const Vertex* v : graph.vertices)
			if (IsSameColor(v->Color(), color))
				count++;
	}
	else
	{
		for (const Edge* e : graph.edges)
			if (IsSameColor(e->Color(), color))
				count++;
	}
	return bound_type == MINIMUM ? (count >= bound) : (count <= bound);
}

int BoundRule::Render(gui::Window* window, int x, int y, int width) const
{
	int r = Vertex::VERTEX_RADIUS;
	window->SetDrawColor(RenderColor(color));
	if (rule_type == VERTEX_RULE)
	{
		window->DrawCircle(x+r, y+r, r, false);
	}
	else
	{
		window->DrawLine(x, y+r, x+width/4, y+r);
	}
	window->SetTextSize(r*2);
	window->DrawText(std::to_string(bound), gui::Position(x+width, y+r),
		gui::Alignment::RIGHT, gui::Alignment::CENTER);
	window->DrawText(bound_type == MAXIMUM ? ">" : "<",
		gui::Position(x+width/2, y+r),
		gui::Alignment::CENTER, gui::Alignment::CENTER);
	window->SetDrawColor(0xFF0000FF);
	window->DrawLine(x+3*width/8, y, x+5*width/8, y+2*r);
	return r*2;
}


} // namespace rules
} // namespace graphcoloring
