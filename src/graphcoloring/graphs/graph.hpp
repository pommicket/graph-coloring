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

#ifndef GRAPHCOLORING_GRAPHS_GRAPH_H_
#define GRAPHCOLORING_GRAPHS_GRAPH_H_

#include <set>

#include "vertex.hpp"
#include "edge.hpp"

namespace graphcoloring {

class Graph {
public:
	Graph(gui::Window* window, const gui::Position& viewport_position,
		bool directed = false);
	virtual ~Graph();
	int V() const;
	int E() const;
	void AddOrigin(int id);
	void Clear(); // Clear all vertices and edges
	void Lock(); // Disallow changes to the graph
	void Unlock(); // Allow changes to the graph
	bool HasVertexWithID(int id) const;
	Vertex& GetVertexByID(int id);
	const Vertex& GetVertexByIDConst(int id) const;
	Edge& GetEdgeByEndpoints(int from, int to);
	const Edge& GetEdgeByEndpointsConst(int from, int to) const;
	bool HasEdgeWithID(int id) const;
	Edge& GetEdgeByID(int id);
	const Edge& GetEdgeByIDConst(int id) const;
	int GetHoveringVertex() const; // Returns ID of vertex which mouse is hovering over, -1 if no such vertex.
	int GetHoveringEdge() const;
	int AddVertex(Vertex& v);
	int AddVertex(int x, int y); // Create vertex with some default parameters.
	int AddEdge(Edge& e);
	int AddEdge(int v1, int v2); // Create edge with some default parameters.
	bool HasEdge(int id1, int id2) const; // Is there an edge between id1 and id2?
	void RemoveVertex(int id);
	void RemoveEdge(int id1, int id2);
	int Degree(int id) const;
	bool IsConnected(int id) const;
	bool IsConnected() const;
	void Render(const std::set<int>& edges_in_path,
			    const std::set<int>& vertices_in_path, int last_vertex);
	void Render();
	bool can_add_new_vertices; // Can the user add more vertices?
	bool can_add_new_edges;
	std::vector<Vertex*> vertices;
	std::vector<Edge*> edges;
private:
	static void ResetIDs(); // Reset edge and vertex IDs.
	int AddVertex(Vertex* v);
	int AddEdge(Edge* e);
	void EPressed(); // e key was pressed
	void DFS(); // Run a DFS on the graph to see which vertices are connected
	static constexpr int COUNTER_TEXT_SIZE = 24;
	gui::Window* const window;
	const gui::Position& viewport_position;
	bool directed;
	std::vector<int> origins;
	std::set<int> connected;
	bool is_locked = false;

	int edge_vertex; // First vertex in edge; -1 if not making edge.
	int v_keyup_callback;
	int e_keyup_callback;
};

} // namespace graphcoloring

#endif // SRC_GRAPHCOLORING_GRAPHS_GRAPH_H_
