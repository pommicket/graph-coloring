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

#ifndef GRAPHCOLORING_LEVELS_GRAPHLOADER_H_
#define GRAPHCOLORING_LEVELS_GRAPHLOADER_H_

#include <memory>

#include "../graphs/graph.hpp"
#include "pugi/pugixml.hpp"
#include "colorloader.hpp"
#include "globalloader.hpp"

namespace graphcoloring {

class GraphLoader {
public:
	GraphLoader(const ColorLoader& color_loader,
		const GlobalLoader& global_loader);
	virtual ~GraphLoader() {}
	// Load document into graph
	void LoadDocument(const pugi::xml_document& document, Graph& graph);
	// Returns id of vertex, or -1 if there is no vertex with that name.
	int GetVertexByName(const std::string& name) const;
	int GetEdgeByName(const std::string& name) const;
	void WriteGraph(const Graph& graph, pugi::xml_node& node) const;
	std::map<std::string, int> vertex_ids;
	std::map<std::string, int> edge_ids;
private:
	bool IsVertexProtected(std::string protections, char protection) const;
	bool IsEdgeProtected(std::string protections, char protection) const;
	void ReadVertex(pugi::xml_node vertex_node, Graph& graph);
	void ReadEdge(pugi::xml_node edge_node, Graph& graph);
	void WriteVertex(const Vertex& vertex, pugi::xml_node& node) const;
	void WriteEdge(const Edge& edge, pugi::xml_node& node) const;
	const ColorLoader& color_loader;
	const GlobalLoader& global_loader;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_GRAPHLOADER_H_
