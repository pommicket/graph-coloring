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

#include "colormenu.hpp"

#include "../level.hpp"
#include "utils/geometry.hpp"

namespace graphcoloring {

int ColorMenu::number_of_color_menus = 0;

ColorMenu::ColorMenu(gui::Window* window_, int x_, int y_,
	const gui::Position& viewport_position_,
	std::function<void(gui::Color)> color_click_callback_)
	: window(window_), x(x_), y(y_), viewport_position(viewport_position_),
	  color_click_callback(color_click_callback_)
{
	number_of_color_menus++;
	if (number_of_color_menus == 1)
	{
		MakeButtons();
		callback_id = window->SetMousedownCallback([this](gui::Window*,int x,int y){
			if (!IsInsideMenu(x,y) && close_callback)
				close_callback();

		});
	}
	else
	{
		callback_id = -1;
	}
}

ColorMenu::~ColorMenu()
{
	if (callback_id != -1)
		window->RemoveMousedownCallback(callback_id);
	number_of_color_menus--;
}

void ColorMenu::SetCloseCallback(std::function<void()> close_callback_)
{
	close_callback = close_callback_;
}

int ColorMenu::GetWidth() const
{
	int number_of_colors = Level::colors.size();
	return SPACING  + (SPACING + 2 * CIRCLE_RADIUS) * number_of_colors;
}

int ColorMenu::GetHeight() const
{
	return 2 * (CIRCLE_RADIUS + SPACING);
}

void ColorMenu::MakeButtons()
{
	int xpos = x + SPACING;
	for (gui::Color color : Level::colors)
	{
		gui::Position pos(xpos, y + SPACING, 0, 0, nullptr,
			&negative_viewport_position);
		gui::Size size(CIRCLE_RADIUS);
		std::unique_ptr<gui::Button> button(
			new gui::Button(window, "", pos, size,
				color, gui::Alignment::LEFT, gui::Alignment::TOP,
				gui::Button::Shape::CIRCLE));
		button->SetCommand([this,color] (){
			if (!color_click_callback) return;
			color_click_callback(color);
		}, GDK_BUTTON_PRIMARY);
		buttons.push_back(std::move(button));
		xpos += SPACING + 2 * CIRCLE_RADIUS;
	}
}

void ColorMenu::Render()
{
	if (callback_id == -1) // Failed to create ColorMenu
	{
		close_callback();
		return;
	}

	int view_x = viewport_position.x;
	int view_y = viewport_position.y;
	negative_viewport_position.SetPos(-view_x, -view_y);

	window->SetDrawColor(BACKGROUND_COLOR);
	window->DrawRectangle(x-view_x, y-view_y, GetWidth(), GetHeight(), true);
	for (std::unique_ptr<gui::Button>& button : buttons)
		button->Render();
}

bool ColorMenu::IsInsideMenu(int mx, int my) const
{
	return utils::geometry::InRectangle(mx, my,
		x-viewport_position.X(), y-viewport_position.Y(),
		GetWidth(), GetHeight());
}

} // namespace graphcoloring

