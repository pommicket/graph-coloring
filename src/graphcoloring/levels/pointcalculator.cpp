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

#include "pointcalculator.hpp"

namespace graphcoloring {

PointCalculator::PointCalculator(const ValueLoader& value_loader_,
		const RuleLoader& rule_loader_, const ColorLoader& color_loader_,
		const Path& path_)
	: value_loader(value_loader_), rule_loader(rule_loader_),
	  color_loader(color_loader_), path(path_)
{}

int PointCalculator::Points(const Graph& graph, bool check_if_invalid) const
{
	if (check_if_invalid)
	{
		bool is_valid = rule_loader.IsValid(graph);
		if (!is_valid) return 0;
	}
	int points = value_loader.Points(graph);
	for (const Vertex* v : graph.vertices)
		points += color_loader.GetVertexPoints(v->Color());
	for (const Edge* e : graph.edges)
		points += color_loader.GetEdgePoints(e->Color());

	points += path.Points();

	return points;
}

} // namespace graphcoloring
