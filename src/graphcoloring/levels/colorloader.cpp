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

#include "colorloader.hpp"

#include "utils/errors.hpp"
#include "../level.hpp"
#include "../graphcoloring.hpp"

namespace graphcoloring {

ColorLoader::ColorLoader()
{
}

void ColorLoader::LoadDocument(const pugi::xml_document& document)
{
	Level::colors.clear();
	pugi::xml_node colors_node = document.child("colors");
	for (pugi::xml_node color_node : colors_node.children("color"))
	{
		std::string name = color_node.attribute("name").value();
		gui::Color color =
			gui::colors::FromAttribute(color_node.attribute("color"));
		int vertex_points = color_node.attribute("vertex-points").as_int(0);
		int edge_points = color_node.attribute("edge-points").as_int(0);
		color_names[name] = color;
		vertex_color_points[color] = vertex_points;
		edge_color_points[color] = edge_points;
		Level::colors.push_back(color);
	}
}

gui::Color ColorLoader::GetColorByName(std::string name) const
{
	if (color_names.count(name) == 0)
		utils::errors::Die("Could not find color: " + name);
	return color_names.at(name);
}

gui::Color ColorLoader::GetColorFromAttribute(pugi::xml_attribute attr) const
{
	if (attr.empty())
		return Level::colors[0];
	else
		return GetColorByName(attr.value());
}

std::string ColorLoader::GetColorName(gui::Color color) const
{
	for (std::pair<std::string, gui::Color> color_name : color_names)
		if (color_name.second == color)
			return color_name.first;
	utils::errors::Die("Could not find color.");
	return "";
}

int ColorLoader::GetVertexPoints(gui::Color color) const
{
	if (vertex_color_points.count(color) == 0)
		utils::errors::Die("Could not find color.");
	return vertex_color_points.at(color);
}

int ColorLoader::GetEdgePoints(gui::Color color) const
{
	if (edge_color_points.count(color) == 0)
		utils::errors::Die("Could not find color.");
	return edge_color_points.at(color);
}

void ColorLoader::RenderColorPoints(gui::Window* window) const
{
	window->SetDrawColor(GraphColoring::BACKGROUND_COLOR);
	window->Clear();
	int x = 10, y = 10;
	const int w = COLOR_POINTS_COLUMN_WIDTH, h = Vertex::VERTEX_RADIUS * 2;
	for (gui::Color color : Level::colors)
	{
		if (edge_color_points.at(color) == 0) continue;
		RenderEdgeColorPoints(window, color, x, y);
		y += h + 10;
		if (y > window->GetHeight()-h-10)
		{
			y = 10;
			x += w + 10;
		}
	}
	for (gui::Color color : Level::colors)
	{
		if (vertex_color_points.at(color) == 0) continue;
		RenderVertexColorPoints(window, color, x, y);
		y += h + 10;
		if (y > window->GetHeight()-h-10)
		{
			y = 10;
			x += w + 10;
		}
	}

}

void ColorLoader::RenderEdgeColorPoints(gui::Window* window, gui::Color color,
	int x, int y) const
{
	const int w = COLOR_POINTS_COLUMN_WIDTH, h = Vertex::VERTEX_RADIUS*2;
	window->SetDrawColor(rules::RenderColor(color));
	window->DrawLine(x, y+h/2, x+w/4, y+h/2);
	window->SetTextSize(48);
	window->DrawText(std::to_string(GetEdgePoints(color)),
		gui::Position(x+w, y), gui::Alignment::RIGHT, gui::Alignment::TOP);
}

void ColorLoader::RenderVertexColorPoints(gui::Window* window, gui::Color color,
	int x, int y) const
{
	const int w = COLOR_POINTS_COLUMN_WIDTH, r = Vertex::VERTEX_RADIUS;
	window->SetDrawColor(rules::RenderColor(color));
	window->DrawCircle(x+r, y+r, r, false);
	window->SetTextSize(48);
	window->DrawText(std::to_string(GetVertexPoints(color)),
		gui::Position(x+w, y+r), gui::Alignment::RIGHT, gui::Alignment::CENTER);
}



} // namespace graphcoloring
