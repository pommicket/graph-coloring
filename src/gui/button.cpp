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

#include "button.hpp"

#include <iostream>

#include "utils/geometry.hpp"

namespace gui {

const std::vector<guint> Button::MOUSE_BUTTONS = {
	GDK_BUTTON_PRIMARY, GDK_BUTTON_MIDDLE, GDK_BUTTON_SECONDARY};

Button::Button(Window* window_, const std::string& text_,
	Position position_, Size size_, Color color_,
	Position::Alignment horizontal_align_, Position::Alignment vertical_align_,
	Shape shape_)
	: window(window_),
	  text(text_),
	  position(position_),
	  size(size_),
	  color(color_),
	  horizontal_align(horizontal_align_),
	  vertical_align(vertical_align_),
	  shape(shape_),
	  hovering(false)
{
	// Given a mouse button, this lambda gives the appropriate
	// mouse_callback_t (which will call CheckClick with the button).
	std::function<Window::mouse_callback_t(guint)> callback =
		[this](guint button)->Window::mouse_callback_t {
			return [this,button](Window*,int,int) {
				CheckClick(button);
			};
		};



	for (guint button : MOUSE_BUTTONS)
	{
		// Initialize callbacks.
		callbacks[button] = std::vector<callback_t>();
		int callback_id = window->SetMouseupCallback(callback(button), button);
		mouseup_callback_ids.push_back(callback_id);
	}
}

Button::~Button()
{
	int i;
	for (i = 0; i < (int)MOUSE_BUTTONS.size(); i++)
		window->RemoveMouseupCallback(mouseup_callback_ids[i],MOUSE_BUTTONS[i]);
}



void Button::CheckHovering()
{
	int mx = window->GetMouseX(), my = window->GetMouseY();
	hovering = shape == Shape::RECTANGLE
		? utils::geometry::InRectangle(mx, my, GetX(), GetY(),
			GetWidth(), GetHeight())
		: utils::geometry::InCircle(mx, my,
			GetX()+GetRadius(), GetY()+GetRadius(), GetRadius());
	if (hovering)
		for (callback_t callback : hover_callbacks)
			callback();
}

void Button::CheckClick(guint button)
{
	CheckHovering();
	if (hovering)
	{
		for (callback_t callback : callbacks[button])
		{
			callback();
		}
	}
}

void Button::Render()
{
	CheckHovering();
	window->SetLineWidth(BORDER_WIDTH);
	window->SetDrawColor(colors::Shade(color, 0.3));
	if (shape == Shape::CIRCLE)
		window->SetTextSize(GetRadius()*CIRCLE_TEXT_SIZE_FACTOR);
	else
		window->SetTextSize(GetHeight()*TEXT_SIZE_FACTOR);

	if (hovering)
	{
		switch (shape)
		{
		case Shape::RECTANGLE:
			window->DrawRectangle(GetX()+BORDER_WIDTH/2, GetY()+BORDER_WIDTH/2,
				GetWidth()-BORDER_WIDTH, GetHeight()-BORDER_WIDTH, true);
			break;
		case Shape::CIRCLE:
			window->DrawCircle(GetX()+GetRadius(), GetY()+GetRadius(),
				GetRadius()-BORDER_WIDTH);
			break;
		}
	}

	window->SetDrawColor(color);
	switch (shape)
	{
	case Shape::RECTANGLE:
		window->DrawRectangle(GetX(), GetY(), GetWidth(), GetHeight(), false);
		break;
	case Shape::CIRCLE:
		window->DrawCircle(GetX()+GetRadius(), GetY()+GetRadius(), GetRadius(),
			false);
		break;
	}

	int text_x = 0, text_y = 0;
	switch (shape)
	{
	case Shape::RECTANGLE:
		{
			Position text_pos(GetX(), GetY(), 0.5, 0.5, &size);
			text_x = text_pos.X();
			text_y = text_pos.Y();
		}
		break;
	case Shape::CIRCLE:
		text_x = GetX() + GetRadius();
		text_y = GetY() + GetRadius();
		break;
	}

	Position text_pos(text_x, text_y);
	window->DrawText(text, text_pos, Alignment::CENTER, Alignment::CENTER);
}

void Button::SetCommand(callback_t callback, guint button)
{
	callbacks[button].push_back(callback);
}

void Button::SetHoverCallback(callback_t callback)
{
	hover_callbacks.push_back(callback);
}

int Button::GetWidth()  const { return size.X();  }
int Button::GetHeight() const { return size.Y(); }
int Button::GetRadius() const { return size.X(); }

int Button::GetX() const
{
	return position.AlignedX(horizontal_align, GetWidth());
}

int Button::GetY() const
{
	return position.AlignedY(vertical_align, GetHeight());
}

void Button::SetPosition(Position position_)
{
	position = position_;
}

void Button::SetAlignment(Alignment horizontal_align_,Alignment vertical_align_)
{
	horizontal_align = horizontal_align_;
	vertical_align   = vertical_align_;
}


} // namespace gui
