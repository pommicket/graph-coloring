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

#include "graphloader.hpp"

#include <sstream>

#include "utils/errors.hpp"
#include "../level.hpp"
#include "../graphcoloring.hpp"

namespace graphcoloring {

GraphLoader::GraphLoader(const ColorLoader& color_loader_,
	const GlobalLoader& global_loader_)
	: color_loader(color_loader_), global_loader(global_loader_)
{}

void GraphLoader::LoadDocument(const pugi::xml_document& document, Graph& graph)
{
	pugi::xml_node graph_node = document.child("graph");
	for (pugi::xml_node vertex_node : graph_node.children("vertex"))
		ReadVertex(vertex_node, graph);
	for (pugi::xml_node edge_node : graph_node.children("edge"))
		ReadEdge(edge_node, graph);
}

bool GraphLoader::IsVertexProtected(std::string protections, char protection)
	const
{
	bool is_protected = protections.find(protection) != std::string::npos;
	if (global_loader.IsVertexProtected(protection))
		is_protected = true;
	return is_protected;
}
bool GraphLoader::IsEdgeProtected(std::string protections, char protection)
	const
{
	bool is_protected = protections.find(protection) != std::string::npos;
	if (global_loader.IsEdgeProtected(protection))
		is_protected = true;
	return is_protected;
}

void GraphLoader::ReadVertex(pugi::xml_node vertex_node, Graph& graph)
{
	int x = vertex_node.attribute("x").as_int();
	int y = vertex_node.attribute("y").as_int();

	int v_id = graph.AddVertex(x, y);
	Vertex& v = graph.GetVertexByID(v_id);

	if (!vertex_node.attribute("origin").empty())
	{
		v.is_delete_protected = true;
		graph.AddOrigin(v_id);
	}



	if (!vertex_node.attribute("id").empty())
	{
		vertex_ids[vertex_node.attribute("id").value()] = v.id;
	}

	gui::Color color
	    = color_loader.GetColorFromAttribute(vertex_node.attribute("color"));
	v.ChangeColor(color);
	// NOTE: This next line does handle the case where there is no protect
	// specified; pugixml will default to "".
	std::string protections = vertex_node.attribute("protect").value();
	v.is_color_protected  = IsVertexProtected(protections, PROTECT_COLOR);
	v.is_delete_protected = IsVertexProtected(protections, PROTECT_DELETE);
	v.is_edge_protected   = IsVertexProtected(protections, PROTECT_EDGE);
}

void GraphLoader::ReadEdge(pugi::xml_node edge_node, Graph& graph)
{
	std::string v1_name = edge_node.attribute("v1").value();
	std::string v2_name = edge_node.attribute("v2").value();

	int v1 = GetVertexByName(v1_name);
	int v2 = GetVertexByName(v2_name);
	if (v1 == -1)
		utils::errors::Die("There is no vertex named " + v1_name);
	if (v2 == -1)
		utils::errors::Die("There is no vertex named " + v2_name);

	int e_id = graph.AddEdge(v1, v2);
	Edge& e = graph.GetEdgeByID(e_id);
	if (!edge_node.attribute("id").empty())
	{
		edge_ids[edge_node.attribute("id").value()] = e.id;
	}
	gui::Color color
		= color_loader.GetColorFromAttribute(edge_node.attribute("color"));
	e.ChangeColor(color);

	std::string protections = edge_node.attribute("protect").value();
	e.is_color_protected  = IsEdgeProtected(protections, PROTECT_COLOR);
	e.is_delete_protected = IsEdgeProtected(protections, PROTECT_DELETE);
}

void GraphLoader::WriteVertex(const Vertex& vertex, pugi::xml_node& node) const
{
	node.set_name("vertex");
	node.append_attribute("x") = vertex.x;
	node.append_attribute("y") = vertex.y;
	node.append_attribute("id") = vertex.id;
	node.append_attribute("color") = color_loader.GetColorName(vertex.Color())
		.c_str();
	std::stringstream protections;
	if (vertex.is_color_protected) protections << PROTECT_COLOR;
	if (vertex.is_delete_protected) protections << PROTECT_DELETE;
	if (vertex.is_edge_protected) protections << PROTECT_EDGE;
	node.append_attribute("protect") = protections.str().c_str();
}

void GraphLoader::WriteEdge(const Edge& edge, pugi::xml_node& node) const
{
	node.set_name("edge");
	node.append_attribute("v1") = edge.from.id;
	node.append_attribute("v2") = edge.to.id;
	node.append_attribute("id") = edge.id;
	node.append_attribute("color") = color_loader.GetColorName(edge.Color())
		.c_str();
	std::stringstream protections;
	if (edge.is_color_protected) protections << PROTECT_COLOR;
	if (edge.is_delete_protected) protections << PROTECT_DELETE;
	node.append_attribute("protect") = protections.str().c_str();
}

void GraphLoader::WriteGraph(const Graph& graph, pugi::xml_node& node) const
{
	for (const Vertex* v : graph.vertices)
	{
		pugi::xml_node vertex_node = node.append_child("vertex");
		WriteVertex(*v, vertex_node);
	}
	for (const Edge* e : graph.edges)
	{
		pugi::xml_node edge_node = node.append_child("edge");
		WriteEdge(*e, edge_node);
	}
}

int GraphLoader::GetVertexByName(const std::string& name) const
{
	if (vertex_ids.count(name))
		return vertex_ids.at(name);
	else
		return -1;
}

int GraphLoader::GetEdgeByName(const std::string& name) const
{
	if (edge_ids.count(name))
		return edge_ids.at(name);
	else
		return -1;
}

} // namespace graphcoloring
