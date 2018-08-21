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

#include "graph.hpp"

#include <sstream>
#include <cassert>
#include <stack>

#include "../level.hpp"
#include "utils/errors.hpp"

namespace graphcoloring {

void Graph::ResetIDs()
{
	Vertex::ResetID();
	Edge::ResetID();
}

Graph::Graph(gui::Window* window_, const gui::Position& viewport_position_,
		bool directed_)
	: window(window_), viewport_position(viewport_position_),
	  edge_vertex(-1)
{
	directed = directed_;
	can_add_new_vertices = true;
	v_keyup_callback =
		window->SetKeyupCallback([this] (gui::Window*) {
			if (can_add_new_vertices && !is_locked)
				AddVertex(window->GetMouseX()+viewport_position.X(),
						  window->GetMouseY()+viewport_position.Y());
		}, GDK_KEY_v);
	e_keyup_callback =
		window->SetKeyupCallback([this] (gui::Window*){EPressed();}, GDK_KEY_e);
}

Graph::~Graph()
{
	Clear();

	window->RemoveKeyupCallback(v_keyup_callback, GDK_KEY_v);
	window->RemoveKeyupCallback(e_keyup_callback, GDK_KEY_e);
}

int Graph::V() const
{
	return vertices.size();
}

int Graph::E() const
{
	return edges.size();
}

void Graph::AddOrigin(int id)
{
	origins.push_back(id);
}

void Graph::Clear()
{
	for (Vertex* v : vertices)
		delete v;
	for (Edge* e : edges)
		delete e;
	vertices.clear();
	edges.clear();
	ResetIDs();
}

void Graph::Lock()
{
	is_locked = true;
	for (Vertex* v : vertices)
		v->Lock();
	for (Edge* e : edges)
		e->Lock();
}

void Graph::Unlock()
{
	is_locked = false;
	for (Vertex* v : vertices)
		v->Unlock();
	for (Edge* e : edges)
		e->Unlock();
}

void Graph::EPressed()
{
	if (!can_add_new_edges || is_locked) return;
	if (edge_vertex != -1 && !HasVertexWithID(edge_vertex)) // Check if vertex was deleted.
		edge_vertex = -1;
	int hover_id = GetHoveringVertex();
	if (edge_vertex == -1)
	{
		edge_vertex = hover_id;
	}
	else if (hover_id != -1)
	{
		if (edge_vertex == hover_id) return; // Disallow self-loops
		if (HasEdge(edge_vertex, hover_id)) return; // Disallow multiple edges between same vertices
		if (GetVertexByID(hover_id).is_edge_protected) return;
		AddEdge(edge_vertex, hover_id);
		edge_vertex = -1; // Reset edge_vertex
	}
	else // User pressed e with no second vertex
	{
		edge_vertex = -1; // Cancel
	}
}

bool Graph::HasVertexWithID(int id) const
{
	for (const Vertex* v : vertices)
		if (v->id == id)
			return true;
	return false;
}

Vertex& Graph::GetVertexByID(int id)
{
	for (Vertex* v : vertices)
		if (v->id == id)
			return *v;
	std::stringstream s;
	s << "Failed to find vertex with id: " << id;
	utils::errors::Die(s.str());
	return *vertices[-1]; // So the compiler doesn't worry...
}

const Vertex& Graph::GetVertexByIDConst(int id) const
{
	for (const Vertex* v : vertices)
		if (v->id == id)
			return *v;
	std::stringstream s;
	s << "Failed to find vertex with id: " << id;
	utils::errors::Die(s.str());
	return *vertices[-1]; // So the compiler doesn't worry...
}


Edge& Graph::GetEdgeByEndpoints(int from, int to)
{
	for (Edge* e : edges)
		if (e->from.id == from && e->to.id == to)
			return *e;

	std::stringstream s;
	s << "Failed to find edge with end-points: " << from << ", " << to;
	utils::errors::Die(s.str());
	return *edges[-1]; // So the compiler doesn't worry...
}

const Edge& Graph::GetEdgeByEndpointsConst(int from, int to) const
{
	for (const Edge* e : edges)
		if (e->HasEndpoints(from, to))
			return *e;

	std::stringstream s;
	s << "Failed to find edge with end-points: " << from << ", " << to;
	utils::errors::Die(s.str());
	return *edges[-1]; // So the compiler doesn't worry...
}

bool Graph::HasEdgeWithID(int id) const
{
	for (const Edge* e : edges)
		if (e->id == id)
			return true;
	return false;
}

Edge& Graph::GetEdgeByID(int id)
{
	for (Edge* e : edges)
		if (e->id == id)
			return *e;

	std::stringstream s;
	s << "Failed to find edge with id: " << id;
	utils::errors::Die(s.str());
	return *edges[-1];
}


const Edge& Graph::GetEdgeByIDConst(int id) const
{
	for (const Edge* e : edges)
		if (e->id == id)
			return *e;
	std::stringstream s;
	s << "Failed to find edge with id: " << id;
	utils::errors::Die(s.str());
	return *edges[-1];
}

int Graph::GetHoveringVertex() const
{
	for (const Vertex* v : vertices)
		if (v->IsHovering())
			return v->id;
	return -1;
}

int Graph::GetHoveringEdge() const
{
	for (const Edge* e : edges)
		if (e->IsHovering())
			return e->id;
	return -1;
}

int Graph::AddVertex(Vertex* v)
{
	vertices.push_back(v);
	std::function<void()> f = [this, v] () {
		RemoveVertex(v->id);
	};
	v->SetDeleteCallback(f);
	DFS();
	return v->id;
}

int Graph::AddVertex(Vertex& v)
{
	return AddVertex(&v);
}

int Graph::AddVertex(int x, int y)
{
	Vertex* v = new Vertex(window, Level::colors[0], x, y, viewport_position);
	return AddVertex(v);
}

int Graph::AddEdge(Edge* e)
{
	edges.push_back(e);
	e->SetDeleteCallback([this, e] () {
		RemoveEdge(e->from.id, e->to.id);
	});
	DFS();
	return e->id;
}

int Graph::AddEdge(Edge& e)
{
	return AddEdge(&e);
}

int Graph::AddEdge(int id1, int id2)
{
	if (!HasVertexWithID(id1)|| !HasVertexWithID(id2))
		utils::errors::Die("Trying to create edge with non-existent vertex.");
	Vertex& v1 = GetVertexByID(id1);
	Vertex& v2 = GetVertexByID(id2);
	Edge* e = new Edge(window, v1, v2, Level::colors[0], viewport_position,
		directed);
	return AddEdge(e);
}

bool Graph::HasEdge(int id1, int id2) const
{
	for (const Edge* e : edges)
		if (e->HasEndpoints(id1, id2))
			return true;
	return false;
}

void Graph::RemoveVertex(int id)
{
	for (int i = 0; i < E(); i++)
	{
		if (edges[i]->from.id == id || edges[i]->to.id == id)
		{
			delete edges[i];
			edges.erase(edges.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < V(); i++)
	{
		if (vertices[i]->id == id)
		{
			delete vertices[i];
			vertices.erase(vertices.begin()+i);
			break;
		}
	}
	DFS();
}

void Graph::RemoveEdge(int id1, int id2)
{
	for (int i = 0; i < E(); i++)
	{
		if (edges[i]->HasEndpoints(id1, id2))
		{
			delete edges[i];
			edges.erase(edges.begin() + i);
			break;

		}
	}
	DFS();
}

int Graph::Degree(int id) const
{
	int degree = 0;
	for (int i = 0; i < E(); i++)
	{
		if (edges[i]->from.id == id || edges[i]->to.id == id)
			degree++;
	}
	return degree;
}

void Graph::DFS()
{
	std::stack<int> vertices;
	for (int v : origins)
		vertices.push(v);
	connected.clear();
	while (!vertices.empty())
	{
		int v = vertices.top();
		vertices.pop();
		if (connected.count(v))
			continue;
		connected.insert(v);
		for (Edge* e : edges)
		{
			if (e->from.id == v && !connected.count(e->to.id))
				vertices.push(e->to.id);
			if (e->to.id == v && !connected.count(e->from.id))
				vertices.push(e->from.id);

		}
	}
}

bool Graph::IsConnected(int id) const
{
	return connected.count(id) > 0;
}

bool Graph::IsConnected() const
{
	for (const Vertex* v : vertices)
		if (!IsConnected(v->id))
			return false;
	return true;
}

void Graph::Render(const std::set<int>& edges_in_path,
				   const std::set<int>& vertices_in_path, int last_vertex)
{
	// Display counter
	window->SetTextSize(COUNTER_TEXT_SIZE);
	window->SetDrawColor(0xCCCCCCFF);
	std::stringstream counter_text;
	counter_text << "Vertices: " << V() << ", Edges: " << E();
	window->DrawText(counter_text.str(), 10, COUNTER_TEXT_SIZE+10);

	if (edge_vertex != -1) // Draw line from edge vertex to mouse.
	{
		if (!HasVertexWithID(edge_vertex))
		{
			edge_vertex = -1;
		}
		else
		{
			Vertex& v = GetVertexByID(edge_vertex);
			window->SetDrawColor(0x888888FF);
			window->DrawLine(v.RenderX(), v.RenderY(),
				window->GetMouseX(), window->GetMouseY());
		}
	}

	for (Vertex* v : vertices)
		v->Render(Degree(v->id), IsConnected(v->id) > 0,
			vertices_in_path.count(v->id) > 0, last_vertex == v->id);
	for (Edge* e : edges)
		e->Render(edges_in_path.count(e->id) > 0);

	for (Vertex* v : vertices)
		v->RenderColorMenu();
	for (Edge* e : edges)
		e->RenderColorMenu();
}

void Graph::Render()
{
	std::set<int> edges_in_path;
	std::set<int> vertices_in_path;
	Render(edges_in_path, vertices_in_path, -1);
}

} // namespace graphcoloring
