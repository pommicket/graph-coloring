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

#ifndef GRAPHCOLORING_LEVELS_VALUELOADER_H_
#define GRAPHCOLORING_LEVELS_VALUELOADER_H_

#include "value.hpp"

#include "graphloader.hpp"

namespace graphcoloring {

class ValueLoader {
public:
	ValueLoader();
	virtual ~ValueLoader() {}
	void LoadDocument(const pugi::xml_document& document);
	void LoadGraph(const GraphLoader& graph_loader); // Loads edge & vertex IDs.
	void LoadColors(const ColorLoader& color_loader); // Loads color names.
	int VariableValue(const Graph& graph, const std::string& name) const;
	int Points(const Graph& graph) const;
	int ObjectivePoints(const Graph& graph) const;
private:
	void AddNode(pugi::xml_node node);
	std::map<std::string, Value> variables;
	int objective_points;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_VALUELOADER_H_
