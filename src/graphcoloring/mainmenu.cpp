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

#include "mainmenu.hpp"

#include <iostream>

#include "gui/gui.hpp"
#include "graphcoloring.hpp"

namespace graphcoloring {

MainMenu::MainMenu(gui::Window* win, gui::Button::callback_t onstart)
	: window(win),
	  title_size(0, 0, TITLE_SIZE/1.5, TITLE_SIZE, win->GetSizePtr())
{
	gui::Button::callback_t quit = [this](){
		window->Quit();
	};

	gui::Size button_size = gui::Position(0,0,BUTTON_WIDTH,BUTTON_HEIGHT,
		win->GetSizePtr());

	std::shared_ptr<gui::Button> quitButton(new gui::Button(window, "Quit",
			gui::Position(), button_size, 0xFF0066FF));
	quitButton->SetCommand(quit);

	std::shared_ptr<gui::Button> startButton(new gui::Button(window, "Start",
			gui::Position(), button_size, 0x00FF66FF));
	startButton->SetCommand(onstart);

	std::vector<std::shared_ptr<gui::Button>> buttons = {
			startButton, quitButton
	};

	int title_height = title_size.Y();
	gui::Position menuPosition = window->GetPosition(0, title_height);
	gui::Size     menuSize     = window->GetPosition(0, -title_height, 1, 1);

	menu = std::make_unique<gui::Menu>(window, buttons, menuPosition, menuSize);

	window->SetRenderCallback([this](gui::Window*){Render();});
}

void MainMenu::Render()
{
	window->SetDrawColor(GraphColoring::BACKGROUND_COLOR);
	window->Clear();
	window->SetDrawColor(TITLE_COLOR);
	window->SetTextSize(std::min(title_size.X(), title_size.Y()));
	window->DrawText(GraphColoring::TITLE, window->GetPosition(0,10,0.5,0),
		gui::Alignment::CENTER, gui::Alignment::TOP);
	menu->Render();
}


} // namespace graphcoloring
