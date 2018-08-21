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

#include "position.hpp"

#include <iostream>

#include "utils/errors.hpp"

namespace gui {

Position::Position(int x_, int y_, double relx_, double rely_,
	const Size* parent_size_, const Position* parent_pos_)
{
	SetPos(x_, y_);
	SetRel(relx_, rely_);
	SetParent(parent_size_, parent_pos_);
}

int Position::X() const
{
	int xpos = x;
	if (parent_size != nullptr)
		xpos += relx * parent_size->X();
	if (parent_pos != nullptr)
		xpos += parent_pos->X();
	return xpos;
}

int Position::Y() const
{
	int ypos = y;
	if (parent_size != nullptr)
		ypos += rely * parent_size->Y();
	if (parent_pos != nullptr)
		ypos += parent_pos->Y();
	return ypos;
}

void Position::SetX(int x_) { x = x_; }
void Position::SetY(int y_) { y = y_; }


void Position::SetPos(int x_, int y_)
{
	SetX(x_);
	SetY(y_);
}

void Position::SetRel(double relx_, double rely_)
{
	relx = relx_;
	rely = rely_;
}

void Position::SetParent(const Size* parent_size_, const Position* parent_pos_)
{
	parent_size = parent_size_;
	parent_pos = parent_pos_;
}

int Position::AlignAxis(Alignment alignment, int val, int size)
{
	switch (alignment)
	{
	case Alignment::LEFT:
		return val;
	case Alignment::RIGHT:
		return val - size;
	case Alignment::CENTER:
		return val - size/2;
	}
	return -1;
}

int Position::AlignedX(Alignment horizontal_align, int width) const
{
	return AlignAxis(horizontal_align, X(), width);
}

int Position::AlignedY(Alignment vertical_align, int height) const
{
	return AlignAxis(vertical_align, Y(), height);
}

std::ostream& operator<<(std::ostream& os, Position pos)
{
	os << "(" << pos.X() << ", " << pos.Y() << ")";
	return os;
}

} // namespace gui
