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

#ifndef GRAPHCOLORING_LEVELS_PATHS_PATH_H_
#define GRAPHCOLORING_LEVELS_PATHS_PATH_H_

#include "gui/window.hpp"
#include "graphcoloring/levels/rules/ruleloader.hpp"

namespace graphcoloring {

class Path {
public:
	Path(gui::Window* window,
		Graph& graph, const RuleLoader& rule_loader,
		const ColorLoader& color_loader);
	virtual ~Path() {}
	void LoadFromDocument(const pugi::xml_document& document);
	void ResetPath();
	bool IsPath() const; // Is there a path in the document?
	std::vector<int> GetPath() const;
	std::set<int> PathEdgeSet() const;
	std::set<int> PathVertexSet() const;
	int LastVertex() const; // -1 if no last vertex
	bool IsMakingPath() const;
	int Points() const;
private:
	enum class Type
	{
		NO_PATH, // There is no path in the document.
		CYCLE,
		PATH
	};
	typedef std::function<int(int)> operation_t;
	static constexpr gui::Color ANY_COLOR = 0;
	void LoadFromNode(pugi::xml_node node);
	void RightClick();
	operation_t LoadOperation(pugi::xml_node node);
	gui::Window* window;
	Graph& graph;
	const RuleLoader& rule_loader;
	const ColorLoader& color_loader;
	Type type = Type::NO_PATH;
	bool is_making_path = false;
	int first_vertex = -1;
	int last_vertex = -1;
	std::vector<int> path;
	int points_starting_value = 0;
	std::map<gui::Color, operation_t> color_operations;

};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_PATHS_PATH_H_
