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

#ifndef GRAPHCOLORING_LEVELSELECT_H_
#define GRAPHCOLORING_LEVELSELECT_H_

#include <memory>

#include "gui/gui.hpp"
#include "pugi/pugixml.hpp"

namespace graphcoloring {

class LevelSelect
{
public:
	typedef std::function<void(std::string,std::string)> level_click_callback_t;
	LevelSelect(gui::Window* window,
		level_click_callback_t level_click_callback);
	virtual ~LevelSelect() {}
	static const char* const LEVEL_LISTING_PATH;
private:
	void Scroll(int direction); // -1 for up, +1 for down
	void ReadCategories();
	pugi::xml_object_range<pugi::xml_named_node_iterator> GetCategories();
	void ButtonClicked(std::string category_id, std::string level_id);
	void MakeLevelButtons();
	void Render();
	void RenderCategoryLabels();
	static constexpr double LEVEL_CIRCLE_RADIUS = 0.06; // As a percentage of min(width, height)
	static constexpr double LEVEL_CIRCLE_PADDING = 0.03;
	static constexpr double TEXT_SIZE = 0.06;
	static constexpr double COMPLETED_SHADE = 0.7; // How much to shade completed levels
	static constexpr int LEVELS_PER_ROW = 7;
	gui::Window* const window;
	pugi::xml_document category_listing;
	int total_points;
	std::map<std::string, double> categories_y_positions;
	level_click_callback_t level_click_callback;
	std::vector<std::unique_ptr<gui::Button>> buttons;
	gui::Size min_dimension_both; // min_dimension_both.x,y = min(window width, window height)
	gui::Size min_dimension_y; // min_dimension_y.x = window width, .y = min(window width, window height)
	gui::Position scroll_position;
	int lowest_y; // Lowest y position of bottom of level circle. Used for calculating maximum scroll down.
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELSELECT_H_
