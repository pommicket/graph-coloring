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

#ifndef GRAPHCOLORING_MAINMENU_H_
#define GRAPHCOLORING_MAINMENU_H_

#include "gui/gui.hpp"

#include <memory>

namespace graphcoloring {

class MainMenu {
public:
	MainMenu(gui::Window* window, gui::Button::callback_t onstart);
	MainMenu();
	virtual ~MainMenu() {}
private:
	void Render();
	gui::Window* const window;
	std::unique_ptr<gui::Menu> menu;
	gui::Size title_size;
	static constexpr double BUTTON_WIDTH  = 0.42; // 42% of window width
	static constexpr double BUTTON_HEIGHT = 0.18;
	static constexpr double TITLE_SIZE    = 0.15;
	static constexpr gui::Color TITLE_COLOR = 0xDDDDDDFF;

};

} // namespace graphcoloring

#endif // GRAPHCOLORING_MAINMENU_H_
