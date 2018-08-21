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

#ifndef GRAPHCOLORING_GRAPHS_COLORMENU_H_
#define GRAPHCOLORING_GRAPHS_COLORMENU_H_

#include "gui/window.hpp"
#include "gui/button.hpp"

#include <memory>

namespace graphcoloring {

class ColorMenu {
public:
	ColorMenu(gui::Window* window, int x, int y,
		const gui::Position& viewport_position,
		std::function<void(gui::Color)> color_click_callback);
	virtual ~ColorMenu();
	void Render();
	void SetCloseCallback(std::function<void()> close_callback);
	int GetWidth() const;
	int GetHeight() const;
	bool IsInsideMenu(int x, int y) const; // Is this mouse position inside the menu?
private:
	void MakeButtons();
	static constexpr int CIRCLE_RADIUS = 20;
	static constexpr int SPACING = 10;
	static constexpr gui::Color BACKGROUND_COLOR = 0x222222FF;
	static int number_of_color_menus; // Number of color menus open.
	int callback_id;
	gui::Window* const window;
	const int x, y;
	const gui::Position& viewport_position;
	gui::Position negative_viewport_position;
	std::vector<std::unique_ptr<gui::Button>> buttons;
	std::function<void(gui::Color)> color_click_callback;
	std::function<void()> close_callback;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_GRAPHS_COLORMENU_H_
