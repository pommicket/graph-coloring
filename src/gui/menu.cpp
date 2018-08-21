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

#include "menu.hpp"

#include <iostream>

namespace gui {

Menu::Menu(Window* window,
	const std::vector<std::shared_ptr<Button>>& buttons_,
	Position position_, Size size_)
	: window(window), buttons(buttons_), position(position_), size(size_)
{
	int total_button_height = 0;
	for (std::shared_ptr<Button>& button : buttons)
	{
		total_button_height += button->GetHeight();
	}

	int number_of_buttons = buttons.size();
	double spacing_rel = 1.0 / (number_of_buttons + 1);
	double spacing_abs = (-total_button_height) / (number_of_buttons + 1);
	if (spacing_rel * size.Y() + spacing_abs < 0) // Window is very short; just give up.
	{
		spacing_rel = 0;
		spacing_abs = 0;
	}
	double y_abs = spacing_abs;
	double y_rel = spacing_rel;

	for (std::shared_ptr<Button>& button : buttons)
	{
		button->SetPosition(Position(0, y_abs, 0.5, y_rel, &size, &position));
		button->SetAlignment(Alignment::CENTER, Alignment::TOP);
		y_abs += spacing_abs + button->GetHeight();
		y_rel += spacing_rel;
	}
}

void Menu::SetCommand(int button, Button::callback_t command)
{
	buttons[button]->SetCommand(command);
}

void Menu::SetCommands(std::vector<Button::callback_t> commands)
{
	for (unsigned i = 0; i < buttons.size(); i++)
		SetCommand((int)i, commands[i]);
}


void Menu::Render()
{
	for (std::shared_ptr<Button>& button : buttons)
		button->Render();
}

} // namespace gui
