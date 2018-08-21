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


#ifndef GRAPHCOLORING_GRAPHCOLORING_H_
#define GRAPHCOLORING_GRAPHCOLORING_H_

#include "gui/window.hpp"
#include "mainmenu.hpp"
#include "levelselect.hpp"
#include "level.hpp"

namespace graphcoloring {

class GraphColoring
{
public:
	enum class State
	{
		MAIN_MENU,
		LEVEL_SELECT
	};

	GraphColoring();
	virtual ~GraphColoring();
	void Start();
	static constexpr gui::Color BACKGROUND_COLOR = 0x111111FF;

	static const std::string TITLE;
private:
	void CheckSaves(); // Makes saves directory if it does not exist.
	void DeleteAll();
	void SwitchToMainMenu();
	void SwitchToLevelSelect();
	void SwitchToLevel(std::string category_id, std::string level_id);
	State state;
	gui::Window* window;
	std::unique_ptr<MainMenu> mainMenu;
	std::unique_ptr<LevelSelect> levelSelect;
	std::unique_ptr<Level> level;
};

constexpr char PROTECT_ADD    = 'a';
constexpr char PROTECT_COLOR  = 'c';
constexpr char PROTECT_DELETE = 'd';
constexpr char PROTECT_EDGE   = 'e'; // Prevent user from adding edge to/from vertex

} // namespace graphcoloring

#endif /* GRAPHCOLORING_GRAPHCOLORING_H_ */
