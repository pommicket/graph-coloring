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


#ifndef GRAPHCOLORING_LEVEL_H_
#define GRAPHCOLORING_LEVEL_H_

#include <string>
#include <memory>

#include "gui/window.hpp"
#include "graphs/graph.hpp"
#include "levels/globalloader.hpp"
#include "levels/valueloader.hpp"
#include "levels/rules/ruleloader.hpp"
#include "levels/pointcalculator.hpp"

namespace graphcoloring {

class Level {
public:
	static pugi::xml_node GetLevelNode(
		const pugi::xml_document& document, std::string category_id,
		std::string level_id);
	Level(gui::Window* window, std::string category_id, std::string level_id);
	virtual ~Level();
	std::pair<std::string,std::string> NextLevel() const; // Returns <"", ""> on success.
	void Render();
	void Save(int slot = SLOT_RECENT);
	int Load(int slot = SLOT_RECENT); // Returns 1 on success, 0 on failure
	static std::vector<gui::Color> colors;
private:
	void LoadLevelDocument();
	std::string GetFile();
	void Reset();
	void MoveViewport();
	void ResetViewport();
	int GetPoints(bool check_if_invalid = true) const;
	void GetBestPoints();
	void RenderPoints(int y);
	void RenderRules();
	std::string SaveFilename(int slot = SLOT_RECENT) const;
	void UpdateLevelList();
	static constexpr int VIEW_MOVE_SPEED = 5;
	static constexpr int TITLE_SIZE = 48;
	static constexpr int DESCRIPTION_SIZE = 18;
	static constexpr int OBJECTIVE_SIZE = 32;
	static constexpr int POINTS_SIZE = 36;
	static constexpr int PRESS_ESC_SIZE = 18;
	static constexpr gui::Color TEXT_COLOR = 0xDDDDDDFF;
	static constexpr gui::Color SUCCESS_COLOR = gui::colors::GREEN;
	static constexpr gui::Color INVALID_COLOR = gui::colors::RED;
	static constexpr int SLOT_RECENT = -2;
	static constexpr int SLOT_BEST   = -1;

	gui::Window* const window;
	int best_points = 0;
	bool has_loaded_best = false;
	gui::Position viewport_position;
	std::string category_id;
	std::string level_id;
	std::string title;
	std::string description;
	std::string objective;
	Graph graph;
	ColorLoader color_loader;
	GlobalLoader global_loader;
	ValueLoader value_loader;
	RuleLoader rule_loader;
	Path path;
	PointCalculator point_calculator;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVEL_H_
