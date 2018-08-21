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

#include "edgerule.hpp"

namespace graphcoloring {
namespace rules {

EdgeRule::EdgeRule(gui::Color vcolor1, gui::Color vcolor2, gui::Color ecolor)
	: vertex_color1(vcolor1), vertex_color2(vcolor2), edge_color(ecolor)
{}

EdgeRule::EdgeRule(pugi::xml_node node, const ColorLoader& color_loader)
{
	LoadFromNode(node, color_loader);
}

void EdgeRule::LoadFromNode(pugi::xml_node node,const ColorLoader& color_loader)
{
	vertex_color1 = ColorFromAttribute(node.attribute("v1"),   color_loader);
	vertex_color2 = ColorFromAttribute(node.attribute("v2"),   color_loader);
	edge_color    = ColorFromAttribute(node.attribute("edge"), color_loader);
}

bool EdgeRule::ObeysRule(const Edge& edge) const
{
	ResetSameColor();
	if (!IsSameColor(edge.Color(), edge_color)) return true;

	gui::Color v1 = edge.from.Color();
	gui::Color v2 = edge.to.Color();
	bool obeys
		= !((IsSameColor(v1,vertex_color1) && IsSameColor(v2,vertex_color2))
		 || (IsSameColor(v2,vertex_color1) && IsSameColor(v1,vertex_color2)));
	ResetSameColor();
	return obeys;
}

bool EdgeRule::ObeysRule(const Graph& graph) const
{
	for (const Edge* e : graph.edges)
		if (!ObeysRule(*e))
			return false;
	return true;
}

int EdgeRule::Render(gui::Window* window, int x, int y, int width) const
{
	int r = Vertex::VERTEX_RADIUS;
	// First vertex
	window->SetDrawColor(RenderColor(vertex_color1));
	window->DrawCircle(x+r, y+r, r, false);
	// Edge
	window->SetDrawColor(RenderColor(edge_color));
	window->DrawLine(x+2*r, y+r, x+width-2*r, y+r);
	// Second vertex
	window->SetDrawColor(RenderColor(vertex_color2));
	window->DrawCircle(x+width-r, y+r, r, false);
	// Red line
	window->SetDrawColor(0xFF0000FF);
	window->DrawLine(x+width/2-r/2, y+r/2, x+width/2+r/2, y+3*r/2);
	return r*2;

}

} // namespace rules
} // namespace graphcoloring
