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

#ifndef GRAPHCOLORING_LEVELS_COLORLOADER_H_
#define GRAPHCOLORING_LEVELS_COLORLOADER_H_

#include <map>
#include <string>

#include "gui/window.hpp"
#include "pugi/pugixml.hpp"

namespace graphcoloring {

class ColorLoader {
public:
	ColorLoader();
	virtual ~ColorLoader(){}
	void LoadDocument(const pugi::xml_document& document);
	gui::Color GetColorByName(std::string name) const;
	gui::Color GetColorFromAttribute(pugi::xml_attribute attr) const;
	std::string GetColorName(gui::Color color) const;
	int GetVertexPoints(gui::Color color) const;
	int GetEdgePoints(gui::Color color) const;
	void RenderColorPoints(gui::Window* window) const;
	std::map<std::string, gui::Color> color_names;
	std::map<gui::Color, int> vertex_color_points;
	std::map<gui::Color, int> edge_color_points;
private:
	static constexpr int COLOR_POINTS_COLUMN_WIDTH = 200;
	void RenderVertexColorPoints(gui::Window* window, gui::Color color,
		int x, int y) const;
	void RenderEdgeColorPoints(gui::Window* window, gui::Color color,
		int x, int y) const;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_COLORLOADER_H_
