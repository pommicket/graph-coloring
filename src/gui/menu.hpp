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

#ifndef GRAPHCOLORING_GUI_MENU_H_
#define GRAPHCOLORING_GUI_MENU_H_

// Menus are vertical lists of buttons.

#include <vector>
#include <string>
#include <memory>

#include "button.hpp"

namespace gui {

class Menu
{
public:
	Menu(Window* window,
		const std::vector<std::shared_ptr<Button>>& buttons,
		Position pos, Size size);
	virtual ~Menu() {}
	void SetCommand(int button, Button::callback_t command);
	void SetCommands(std::vector<Button::callback_t> commands);
	void Render();
private:
	Window* const window;
	std::vector<std::shared_ptr<Button>> buttons;
	Position position;
	Size size;
};

} // namespace gui



#endif /* GRAPHCOLORING_GUI_MENU_H_ */
