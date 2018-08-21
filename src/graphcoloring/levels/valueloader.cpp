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

#include "valueloader.hpp"

#include "../level.hpp"
#include "utils/errors.hpp"

namespace graphcoloring {

ValueLoader::ValueLoader() : objective_points(-1) {}

void ValueLoader::AddNode(pugi::xml_node node)
{
	variables[node.attribute("id").value()] = Value(node);
}

void ValueLoader::LoadGraph(const GraphLoader& graph_loader)
{
	for (const std::pair<std::string, int>& v_id : graph_loader.vertex_ids)
		variables[v_id.first] = v_id.second;
	for (const std::pair<std::string, int>& e_id : graph_loader.edge_ids)
		variables[e_id.first] = e_id.second;
}


void ValueLoader::LoadColors(const ColorLoader& color_loader)
{
	for (std::pair<std::string, gui::Color> color : color_loader.color_names)
	{
		variables[color.first] = (int)color.second;
	}
}

void ValueLoader::LoadDocument(const pugi::xml_document& document)
{
	for (pugi::xml_node node : document.child("values").children())
	{
		std::string name = node.name();
		if (name == "comment") continue;
		if (name == "include")
		{
			pugi::xml_document doc;
			std::string filename = node.attribute("file").value();
			std::string path = "assets/levels/" + filename + ".xml";
			doc.load_file(path.c_str());
			LoadDocument(doc);
		}
		else
		{
			AddNode(node);
		}
	}
}


int ValueLoader::VariableValue(const Graph& graph,
	const std::string& name) const
{
	if (variables.count(name) == 0)
		utils::errors::Die("Variable not found: " + name);
	std::function<Value(std::string)> lookup_variable
		= [this] (std::string name)->Value {
			if (variables.count(name) == 0)
				utils::errors::Die("Variable not found: " + name);
			return variables.at(name);
		};
	return variables.at(name).Eval(graph, lookup_variable);
}

int ValueLoader::Points(const Graph& graph) const
{
	return VariableValue(graph, "points");
}

int ValueLoader::ObjectivePoints(const Graph& graph) const
{
	return VariableValue(graph, "objective");
}

} // namespace graphcoloring
