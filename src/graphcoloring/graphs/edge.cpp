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

#include "edge.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

#include "utils/geometry.hpp"
#include "utils/errors.hpp"

namespace graphcoloring {

int Edge::id_counter = 0;

void Edge::ResetID()
{
	id_counter = 0;
}


Edge::Edge(gui::Window* window_, Vertex& from_, Vertex& to_, gui::Color color_,
	const gui::Position& viewport_position_, bool directed_)
	: id(id_counter++), from(from_), to(to_), window(window_),
	  viewport_position(viewport_position_)
{
	color = color_;
	directed = directed_;
	color_menu = nullptr;
	mousedown_callback_id =
		window->SetMousedownCallback([this] (gui::Window*, int x, int y) {
			MouseCallback(x, y);
		});
	x_keyup_callback_id =
		window->SetKeyupCallback([this] (gui::Window*) {
			if (hovering && !is_delete_protected && delete_callback
			 && !is_locked)
				delete_callback();
		}, GDK_KEY_x);
}

Edge::~Edge()
{
	window->RemoveMousedownCallback(mousedown_callback_id);
	window->RemoveKeyupCallback(x_keyup_callback_id, GDK_KEY_x);
}

int Edge::OtherEndpoint(int vertex_id) const
{
	if (from.id == vertex_id) return to.id;
	if (to.id == vertex_id) return from.id;
	utils::errors::Die("No other endpoint.");
	return -1;
}

void Edge::Lock()
{
	is_locked = true;
}

void Edge::Unlock()
{
	is_locked = false;
}

bool Edge::IsHovering() const
{
	return hovering;
}

bool Edge::IsAdjacentTo(const Edge& edge) const
{
	return edge.HasEndpoint(from.id) || edge.HasEndpoint(to.id);
}

void Edge::SetDeleteCallback(std::function<void()> delete_callback_)
{
	delete_callback = delete_callback_;
}

gui::Color Edge::Color() const
{
	return color;
}

bool Edge::ChangeColor(gui::Color new_color)
{
	if (is_color_protected) return false;
	color = new_color;
	return true;
}

bool Edge::HasEndpoints(int id1, int id2) const
{
	if (from.id == id1 && to.id == id2)
		return true;
	return !directed && from.id == id2 && to.id == id1;
}

bool Edge::HasEndpoint(int v_id) const
{
	return from.id == v_id || to.id == v_id;
}

void Edge::Render(bool is_in_path)
{
	int dist = utils::geometry::PointToLineSegmentDistance(
		window->GetMouseX(), window->GetMouseY(),
		from.RenderX(), from.RenderY(), to.RenderX(), to.RenderY(),
		EDGE_CLICK_TOLERANCE);
	hovering = dist < EDGE_CLICK_TOLERANCE && dist != -1;
	int from_x = from.RenderX();
	int from_y = from.RenderY();
	int to_x   =   to.RenderX();
	int to_y   =   to.RenderY();

	double theta1 = utils::geometry::LineAngle(from_x, from_y, to_x, to_y);
	double theta2 = utils::geometry::LineAngle(to_x, to_y, from_x, from_y);

	int x1 = from_x + Vertex::VERTEX_RADIUS * std::cos(theta1);
	int y1 = from_y + Vertex::VERTEX_RADIUS * std::sin(theta1);

	int x2 = to_x + Vertex::VERTEX_RADIUS * std::cos(theta2);
	int y2 = to_y + Vertex::VERTEX_RADIUS * std::sin(theta2);

	window->SetDrawColor(is_in_path ? gui::colors::WHITE : color);
	window->DrawLine(x1, y1, x2, y2);

	if (directed)
	{
		double theta3 = theta2 - M_PI / 4;
		double theta4 = theta2 + M_PI / 4;

		int x3 = x2 + ARROW_SIZE * std::cos(theta3);
		int y3 = y2 + ARROW_SIZE * std::sin(theta3);

		int x4 = x2 + ARROW_SIZE * std::cos(theta4);
		int y4 = y2 + ARROW_SIZE * std::sin(theta4);

		window->DrawLine(x2, y2, x3, y3);
		window->DrawLine(x2, y2, x4, y4);
	}

}

void Edge::RenderColorMenu()
{
	if (color_menu != nullptr)
		color_menu->Render();
}


void Edge::MouseCallback(int mouse_x, int mouse_y)
{
	if (!hovering || color_menu != nullptr || is_color_protected
	 || is_locked)
		return;

	std::function<void(gui::Color)> callback = [this] (gui::Color color) {
		assert(ChangeColor(color));
		color_menu = nullptr;
	};

	color_menu = std::make_unique<ColorMenu>(window,
		mouse_x+viewport_position.X(), mouse_y+viewport_position.Y(),
		viewport_position, callback);

	std::function<void()> close_callback = [this] () {
		color_menu = nullptr;
	};
	color_menu->SetCloseCallback(close_callback);
}


} // namespace graphcoloring
