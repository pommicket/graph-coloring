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


// This position class can be used for relative positions, like
// (30% of parent width + 5 pixels, 20% of parent height - 20 pixels)

#ifndef GRAPHCOLORING_GUI_POSITION_H_
#define GRAPHCOLORING_GUI_POSITION_H_

#include <string>

namespace gui {

class Position {
public:
	typedef Position Size; // "Size" alias for Positions that really refer to sizes.
	enum class Alignment
	{
		LEFT,
		CENTER,
		RIGHT,
		TOP = LEFT,
		BOTTOM = RIGHT
	};
	Position(int x = 0, int y = 0, double relx = 0, double rely = 0,
		const Size* parent_size = nullptr, const Position* parent_pos
		= nullptr);
	virtual ~Position() {}
	int X() const;
	int Y() const;
	void SetX(int x);
	void SetY(int y);
	void SetPos(int x, int y);
	void SetRel(double relx, double rely);
	void SetParent(const Size* parent_size = nullptr,
		const Position* parent_pos = nullptr);
	int AlignedX(Alignment horizontal_align, int width)  const;
	int AlignedY(Alignment vertical_align,   int height) const;
	int x;
	int y;
	double relx;
	double rely;
	const Size* parent_size;
	const Position* parent_pos;
private:
	static int AlignAxis(Alignment alignment, int val, int size);
	friend std::ostream& operator<<(std::ostream& os, Position pos);
};

extern std::ostream& operator<<(std::ostream& os, Position pos);

typedef Position::Alignment Alignment;
typedef Position::Size Size;

} // namespace gui

#endif // GRAPHCOLORING_GUI_POSITION_H_
