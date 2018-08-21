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

#include "geometry.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "errors.hpp"

namespace utils {
namespace geometry {

bool InRectangle(int x, int y, int rx, int ry, int rw, int rh)
{
	return x >= rx && x <= rx+rw && y >= ry && y <= ry+rh;
}

bool InCircle(int x, int y, int cx, int cy, int r)
{
	return (x-cx)*(x-cx) + (y-cy)*(y-cy) <= r*r;
}

double LineAngle(int x1, int y1, int x2, int y2)
{
	if (x1 == x2)
	{
		if (y1 == y2)
			return 0; // Give up
		if (y1 < y2)
			return M_PI / 2; // Slope = Positive infinity
		if (y1 > y2)
			return 3 * M_PI / 2; // Negative infinity
	}
	double slope = (double)(y2-y1)/(x2-x1);
	return x2 > x1 ? atan(slope) : M_PI + atan(slope);

}

int PointToLineSegmentDistance(int x, int y, int x1, int y1, int x2, int y2,
		int tolerance)
{
	// See https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
	// Check if point is inside rectangle bounded by endpoints.
	int min_x = std::min(x1, x2);
	int min_y = std::min(y1, y2);
	int max_x = std::max(x1, x2);
	int max_y = std::max(y1, y2);
	if (x < min_x-tolerance || x > max_x+tolerance || y < min_y-tolerance
	 || y > max_y+tolerance)
		return -1;
	// Check if the two points are the same.
	if (x1 == x2 && y1 == y2)
		return sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
	// Find distance to line.
	int dx = x2-x1, dy = y2-y1;
	double length = sqrt(dx*dx+dy*dy);
	return abs(dy*x - dx*y + x2*y1 - y2*x1)/length;
}

} // namespace geometry
} // namespace utils
