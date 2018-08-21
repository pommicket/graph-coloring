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

#include "globalloader.hpp"

namespace graphcoloring {

GlobalLoader::GlobalLoader()
{
}

void GlobalLoader::LoadDocument(const pugi::xml_document& document)
{
	vertex_protections =
	   document.child("global-vertex-protections").attribute("protect").value();
	edge_protections =
		document.child("global-edge-protections").attribute("protect").value();
}

bool GlobalLoader::IsVertexProtected(char protection) const
{
	return vertex_protections.find(protection) != std::string::npos;
}

bool GlobalLoader::IsEdgeProtected(char protection) const
{
	return edge_protections.find(protection) != std::string::npos;
}

} // namespace graphcoloring
