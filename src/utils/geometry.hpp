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

#ifndef GRAPHCOLORING_UTILS_GEOMETRY_H_
#define GRAPHCOLORING_UTILS_GEOMETRY_H_

namespace utils {
namespace geometry {

extern bool InRectangle(int x, int y, int rx, int ry, int rw, int rh);
extern bool InCircle(int x, int y, int cx, int cy, int r);
extern double LineAngle(int x1, int y1, int x2, int y2); // Angle from (x1,y1) to (x2,y2)
extern int PointToLineSegmentDistance(int x, int y, int x1, int y1, int x2,
	int y2, int tolerance); // Returns -1 if point does not project to line segment
} // namespace geometry
} // namespace utils


#endif // GRAPHCOLORING_UTILS_GEOMETRY_H_
