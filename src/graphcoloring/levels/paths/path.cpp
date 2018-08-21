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

#include "path.hpp"

#include <cassert>
#include <algorithm>

namespace graphcoloring {

constexpr gui::Color Path::ANY_COLOR;

Path::Path(gui::Window* window_,
	Graph& graph_, const RuleLoader& rule_loader_,
	const ColorLoader& color_loader_)
	: window(window_), graph(graph_), rule_loader(rule_loader_),
	  color_loader(color_loader_)
{
	window->SetMouseupCallback([this] (gui::Window*,int,int) {
		RightClick();
	}, GDK_BUTTON_SECONDARY);
	window->SetKeyupCallback([this] (gui::Window*) {
		ResetPath();
	}, GDK_KEY_q);
}

void Path::LoadFromNode(pugi::xml_node node)
{
	std::string name = node.name();

	if (name == "cycle")
		type = Type::CYCLE;
	else
		type = Type::PATH;

	points_starting_value = node.attribute("points").as_int(0);

	for (pugi::xml_node point_node : node.children())
	{
		std::string name = point_node.name();
		if (name != "edge") continue;
		std::string color_str = point_node.attribute("color").value();
		gui::Color color;
		if (color_str == "any")
			color = ANY_COLOR;
		else
			color =
				color_loader.GetColorFromAttribute(
						point_node.attribute("color"));
		color_operations[color] = LoadOperation(point_node);
	}
}

void Path::LoadFromDocument(const pugi::xml_document& document)
{
	for (pugi::xml_node node : document.children())
	{
		std::string name = node.name();
		if (name != "cycle" && name != "path") continue;
		LoadFromNode(node);
		break;
	}
}

Path::operation_t Path::LoadOperation(pugi::xml_node node)
{
	int val = node.attribute("val").as_int(0);
	std::map<std::string, operation_t> operations = {
			{"+", [val] (int a) { return a + val; }},
			{"-", [val] (int a) { return a - val; }},
			{"*", [val] (int a) { return a * val; }},
			{"/", [val] (int a) { return a / val; }},
			{"%", [val] (int a) { return a % val; }}
	};
	std::string op = node.attribute("op").value();
	return operations[op];
}

void Path::RightClick()
{
	if (!IsPath()) return;
	if (!rule_loader.IsValid(graph)) return;
	int vertex_id = graph.GetHoveringVertex();
	if (vertex_id == -1) return;
	graph.Lock();
	if (last_vertex == -1)
	{
		last_vertex = vertex_id;
		if (first_vertex == -1)
			first_vertex = vertex_id;
		return;
	}
	if (last_vertex == vertex_id) // Remove end of path
	{
		if (path.size() == 0)
		{
			ResetPath();
			return;
		}
		int edge_id = path.at(path.size()-1);
		path.pop_back();
		last_vertex =
			graph.GetEdgeByIDConst(edge_id).OtherEndpoint(last_vertex);
		return;
	}
	if (!graph.HasEdge(last_vertex, vertex_id))
	{
		return;
	}
	int edge_id = graph.GetEdgeByEndpointsConst(last_vertex, vertex_id).id;
	if (std::find(path.begin(), path.end(), edge_id) != path.end()) // Duplicate edge
		return;
	path.push_back(edge_id);
	last_vertex = vertex_id;
}

void Path::ResetPath()
{
	if (!IsPath()) return;
	graph.Unlock();
	path = std::vector<int>();
	is_making_path = false;
	first_vertex = -1;
	last_vertex = -1;
}

std::vector<int> Path::GetPath() const
{
	return path;
}

std::set<int> Path::PathEdgeSet() const
{
	return std::set<int>(path.begin(), path.end());

}

std::set<int> Path::PathVertexSet() const
{
	std::set<int> vertices;
	if (last_vertex != -1)
		vertices.insert(last_vertex);
	for (int e : path)
	{
		const Edge& edge = graph.GetEdgeByIDConst(e);
		vertices.insert(edge.from.id);
		vertices.insert(edge.to.id);
	}
	return vertices;
}

int Path::LastVertex() const
{
	return last_vertex;
}

bool Path::IsMakingPath() const
{
	return is_making_path;
}

bool Path::IsPath() const
{
	return type != Type::NO_PATH;
}

int Path::Points() const
{
	if (!IsPath()) return 0;
	if (type == Type::CYCLE)
	{
		if (path.size() < 2)
			return 0;
		if (first_vertex != last_vertex)
			return 0;
	}
	int points = points_starting_value;
	for (int e : path)
	{
		const Edge& edge = graph.GetEdgeByIDConst(e);
		if (color_operations.count(edge.Color())
		 && color_operations.at(edge.Color()))
			points = color_operations.at(edge.Color())(points);

		if (color_operations.count(ANY_COLOR)
		  && color_operations.at(ANY_COLOR))
			points = color_operations.at(ANY_COLOR)(points);
	}
	return points;
}

} // namespace graphcoloring
