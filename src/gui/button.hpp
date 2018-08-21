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

#ifndef GRAPHCOLORING_GUI_BUTTON_H_
#define GRAPHCOLORING_GUI_BUTTON_H_


#include <string>
#include <vector>
#include <functional>
#include <iostream>

#include "colors.hpp"
#include "position.hpp"
#include "window.hpp"

namespace gui {

class Button
{
public:
	typedef std::function<void()> callback_t;
	typedef std::map<guint, std::vector<callback_t>> callback_map_t;
	enum class Shape
	{
		RECTANGLE,
		CIRCLE // NOTE: If you select this, size.X() will be used for the radius of the circle.
	};
	Button(Window* window, const std::string& text, Position pos, Size size,
		   Color color,
		   Position::Alignment horizontal_align = Position::Alignment::LEFT,
		   Position::Alignment vertical_align = Position::Alignment::TOP,
		   Shape shape = Shape::RECTANGLE);
	virtual ~Button();
	void SetCommand(callback_t callback, guint button = GDK_BUTTON_PRIMARY);
	void SetHoverCallback(callback_t callback);
	void Render();
	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetRadius() const;
	void SetPosition(Position position);
	void SetAlignment(Alignment horizontal_align, Alignment vertical_align);
	static constexpr double CIRCLE_TEXT_SIZE_FACTOR = 0.9; // Allows for ~2 characters
private:
	void CheckHovering();
	void CheckClick(guint button);
	static constexpr int BORDER_WIDTH = 3;
	static constexpr double TEXT_SIZE_FACTOR = 0.7;
	static const std::vector<guint> MOUSE_BUTTONS;
	Window* const window;
	std::string text;
	Position position;
	Size size;
	Color color;
	Alignment horizontal_align, vertical_align;
	Shape shape;
	bool hovering;
	std::vector<int> mouseup_callback_ids;
	callback_map_t callbacks;
	std::vector<callback_t> hover_callbacks;
};

} // namespace gui


#endif // GRAPHCOLORING_GUI_BUTTON_H_
