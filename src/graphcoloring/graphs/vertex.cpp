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

#include "vertex.hpp"

#include <cassert>
#include <iostream>

#include "utils/geometry.hpp"
#include "gui/button.hpp"
#include "../graphcoloring.hpp"

namespace graphcoloring {

int Vertex::id_counter = 0;
int Vertex::moving_vertex = -1;

void Vertex::ResetID()
{
	id_counter = 0;
}

Vertex::Vertex(gui::Window* window_, gui::Color color_, int x_, int y_,
	const gui::Position& viewport_position_)
	: x(x_), y(y_), id(id_counter++),
	  window(window_),
	  color(color_),
	  viewport_position(viewport_position_),
	  color_menu(nullptr)
{
	SetClickCallback();
	SetMoveCallbacks();
	SetDeleteKeyCallback();
}

Vertex::~Vertex()
{
	if (moving_vertex == id) moving_vertex = -1;
	window->RemoveMousedownCallback(mousedown_callback_id);
	window->RemoveKeydownCallback(m_keydown_callback_id, GDK_KEY_m);
	window->RemoveKeyupCallback(m_keyup_callback_id, GDK_KEY_m);
	window->RemoveKeyupCallback(x_keyup_callback_id, GDK_KEY_x);
}

void Vertex::Lock()
{
	is_locked = true;
	moving_vertex = -1;
}

void Vertex::Unlock()
{
	is_locked = false;
}

void Vertex::SetClickCallback()
{
	mousedown_callback_id =
		window->SetMousedownCallback([this] (gui::Window*, int x, int y) {
			MouseCallback(x, y);
		}, GDK_BUTTON_PRIMARY);

}

void Vertex::SetMoveCallbacks()
{
	m_keydown_callback_id =
		window->SetKeydownCallback([this] (gui::Window*) {
			if (moving_vertex == -1 && hovering)
			{
				moving_vertex = id;
			}
		}, GDK_KEY_m);
	m_keyup_callback_id =
		window->SetKeyupCallback([this] (gui::Window*) {
			moving_vertex = -1;
		}, GDK_KEY_m);
}

void Vertex::SetDeleteKeyCallback()
{
	x_keyup_callback_id =
		window->SetKeyupCallback([this] (gui::Window*) {
			if (hovering && !is_delete_protected && !is_locked)
			{
				if (delete_callback)
				{
					delete_callback();
				}
			}
		}, GDK_KEY_x);
}

void Vertex::SetDeleteCallback(std::function<void()> delete_callback_)
{
	delete_callback = delete_callback_;
}

bool Vertex::operator==(const Vertex other) const
{
	return id == other.id;
}

gui::Color Vertex::Color() const
{
	return color;
}

bool Vertex::ChangeColor(gui::Color new_color)
{
	if (is_color_protected) return false;
	color = new_color;
	return true;
}

int Vertex::RenderX() const
{
	return x - viewport_position.X();
}

int Vertex::RenderY() const
{
	return y - viewport_position.Y();
}


void Vertex::Render(int degree, bool filled, bool is_in_path,
	bool is_last_vertex)
{
	int rx = RenderX(), ry = RenderY(); // Coordinates of circle

	CheckIfMoving();

	window->SetDrawColor(is_in_path ? gui::colors::WHITE : color);
	if (is_last_vertex)
		window->SetDrawColor(0x888888FF);
	window->DrawCircle(rx, ry, VERTEX_RADIUS, filled);
	window->SetTextSize(VERTEX_RADIUS*gui::Button::CIRCLE_TEXT_SIZE_FACTOR);

	if (filled)
		window->SetDrawColor(GraphColoring::BACKGROUND_COLOR);

	hovering = utils::geometry::InCircle(
		window->GetMouseX(), window->GetMouseY(), rx, ry, VERTEX_RADIUS);
	if (hovering)
	{
		window->DrawText(std::to_string(degree), gui::Position(rx,ry),
			gui::Alignment::CENTER, gui::Alignment::CENTER);
	}
}

void Vertex::CheckIfMoving()
{
	if (moving_vertex == id)
	{
		x = window->GetMouseX() + viewport_position.X();
		y = window->GetMouseY() + viewport_position.Y();
	}
}

void Vertex::RenderColorMenu()
{
	if (color_menu != nullptr)
		color_menu->Render();
}


void Vertex::MouseCallback(int mouse_x, int mouse_y)
{
	if (is_color_protected || is_locked) return;
	if (hovering && color_menu == nullptr && !is_color_protected)
	{
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
}

} // namespace graphcoloring
