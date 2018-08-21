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

#ifndef GRAPHCOLORING_GRAPHS_VERTEX_H_
#define GRAPHCOLORING_GRAPHS_VERTEX_H_

#include "gui/colors.hpp"
#include "gui/window.hpp"
#include "colormenu.hpp"

namespace graphcoloring {

class Vertex {
public:
	static void ResetID();
	Vertex(gui::Window* window, gui::Color color, int x, int y,
		const gui::Position& viewport_position);
	virtual ~Vertex();
	void Lock();
	void Unlock();
	void SetDeleteCallback(std::function<void()> delete_callback);
	bool operator==(const Vertex other) const;
	gui::Color Color() const;
	bool ChangeColor(gui::Color new_color);
	int RenderX() const; // x-position when rendered
	int RenderY() const;
	bool IsHovering() const { return hovering; }; // Is the mouse hovering over this vertex?
	void Render(int degree, bool filled = false, bool is_in_path = false,
		bool is_last_vertex = false);
	void RenderColorMenu();
	static constexpr int VERTEX_RADIUS = 40;
	int x;
	int y;
	const int id;
	bool is_color_protected = false;
	bool is_edge_protected = false;
	bool is_delete_protected = false;
	static int moving_vertex; // id of vertex that is being moved, or -1 if no vertex is being moved.
private:
	void SetClickCallback();
	void SetMoveCallbacks();
	void SetDeleteKeyCallback();
	void MouseCallback(int mouse_x, int mouse_y);
	void CheckIfMoving();
	static int id_counter;
	std::function<void()> delete_callback;
	int mousedown_callback_id;
	int m_keydown_callback_id;
	int m_keyup_callback_id;
	int x_keyup_callback_id;
	gui::Window* const window;
	gui::Color color;
	bool hovering = false;
	bool is_locked = false;
	const gui::Position& viewport_position;
	std::unique_ptr<ColorMenu> color_menu;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_GRAPHS_VERTEX_H_
