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

#ifndef GRAPHCOLORING_LEVELS_POINTCALCULATOR_H_
#define GRAPHCOLORING_LEVELS_POINTCALCULATOR_H_

#include "valueloader.hpp"
#include "colorloader.hpp"
#include "rules/ruleloader.hpp"
#include "paths/path.hpp"

namespace graphcoloring {

class PointCalculator {
public:
	PointCalculator(const ValueLoader& value_loader,
		const RuleLoader& rule_loader, const ColorLoader& color_loader,
		const Path& path);
	int Points(const Graph& graph, bool check_if_invalid = true) const;
	virtual ~PointCalculator() {}
private:
	const ValueLoader& value_loader;
	const RuleLoader& rule_loader;
	const ColorLoader& color_loader;
	const Path& path;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_POINTCALCULATOR_H_
