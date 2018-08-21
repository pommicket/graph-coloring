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

#ifndef GRAPHCOLORING_GRAPHS_EDGE_H_
#define GRAPHCOLORING_GRAPHS_EDGE_H_

#include "vertex.hpp"

#include "gui/window.hpp"

namespace graphcoloring {

class Edge {
public:
	static void ResetID();
	Edge(gui::Window* window, Vertex& from, Vertex& to,
		gui::Color color, const gui::Position& viewport_position,
		bool directed = false);
	virtual ~Edge();
	int OtherEndpoint(int vertex_id) const;
	void Lock();
	void Unlock();
	bool IsHovering() const;
	bool IsAdjacentTo(const Edge& edge) const;
	void SetDeleteCallback(std::function<void()> delete_callback);
	bool ChangeColor(gui::Color new_color); // Returns false if edge is color protected.
	gui::Color Color() const;
	bool HasEndpoint(int id) const; // Does this edge have this endpoint?
	bool HasEndpoints(int id1, int id2) const; // Does this edge have these endpoints?
	void Render(bool is_in_path = false);
	void RenderColorMenu(); // Color menu rendering is handled separately
	const int id;
	Vertex& from;
	Vertex& to;
	bool is_color_protected = false;
	bool is_delete_protected = false;
private:
	void MouseCallback(int mouse_x, int mouse_y);
	static constexpr int EDGE_CLICK_TOLERANCE = 10;
	static constexpr int ARROW_SIZE = 10;
	static int id_counter;
	gui::Window* const window;
	gui::Color color;
	std::unique_ptr<ColorMenu> color_menu;
	const gui::Position& viewport_position;
	bool directed;
	bool hovering = false;
	bool is_locked = false;
	int mousedown_callback_id;
	int x_keyup_callback_id;
	std::function<void()> delete_callback;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_GRAPHS_EDGE_H_
