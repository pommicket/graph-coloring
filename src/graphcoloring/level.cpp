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

#include "level.hpp"

#include <sstream>

#include "graphcoloring.hpp"
#include "graphs/vertex.hpp"
#include "graphs/edge.hpp"
#include "levels/graphloader.hpp"

namespace graphcoloring {

std::vector<gui::Color> Level::colors;

pugi::xml_node Level::GetLevelNode(
		const pugi::xml_document& document, std::string category_id,
		std::string level_id)
{
	std::stringstream xpath;
	xpath << "/category-listing/category[@id='" << category_id << "']"
		  << "/level[@id='" + level_id + "']";

	pugi::xpath_node xnode = document.select_node(xpath.str().c_str());
	pugi::xml_node node = xnode.node();
	return node;
}

Level::Level(gui::Window* window_,
	std::string category_id_, std::string level_id_)
	: window(window_), category_id(category_id_), level_id(level_id_),
	  graph(window, viewport_position),
	  path(window, graph, rule_loader, color_loader),
	  point_calculator(value_loader, rule_loader, color_loader, path)
{

	window->SetRenderCallback([this] (gui::Window*){ Render(); });
	window->SetKeyupCallback([this] (gui::Window*){ ResetViewport(); },
		GDK_KEY_o);
	window->SetKeyupCallback([this] (gui::Window*) {
		if (window->IsControlDown())
			Reset();
	}, GDK_KEY_r);
	window->SetKeyupCallback([this] (gui::Window*) {
		if (window->IsControlDown())
			Load(SLOT_BEST);
	}, GDK_KEY_b);

	for (int digit = 0; digit <= 9; digit++)
	{
		window->SetKeyupCallback([this,digit](gui::Window*) {
			if (window->IsControlDown())
				Load(digit);
			else
				Save(digit);
		}, GDK_KEY_0 + digit);
	}
	LoadLevelDocument();
}

Level::~Level()
{
	Save();
}

std::pair<std::string,std::string> Level::NextLevel() const
{
	pugi::xml_document document;
	document.load_file(LevelSelect::LEVEL_LISTING_PATH);
	pugi::xml_node this_level_node =
		GetLevelNode(document, category_id, level_id);
	pugi::xml_node next_level_node = this_level_node.next_sibling();
	if (next_level_node)
	{
		std::string next_level_id = next_level_node.attribute("id").value();
		return std::make_pair(category_id, next_level_id);
	}
	else
	{
		pugi::xml_node category_node = this_level_node.parent();
		pugi::xml_node next_category_node = category_node.next_sibling();
		if (next_category_node)
		{
			std::string next_category_id =
				next_category_node.attribute("id").value();
			next_level_node = next_category_node.first_child();

			std::string next_level_id =
				next_level_node.attribute("id").value();
			return std::make_pair(next_category_id, next_level_id);
		}
		else
		{
			return std::make_pair("", ""); // No next level.
		}
	}
}

void Level::LoadLevelDocument()
{
	pugi::xml_document document;
	std::string filename
		= "assets/levels/" + category_id + "/" + level_id + ".xml";
	document.load_file(filename.c_str());

	pugi::xml_node level_node = document.child("level");
	title = level_node.attribute("title").value();
	description = level_node.attribute("description").value();
	objective = level_node.attribute("objective").value();

	color_loader.LoadDocument(document);
	global_loader.LoadDocument(document);

	graph.can_add_new_vertices = !global_loader.IsVertexProtected(PROTECT_ADD);
	graph.can_add_new_edges =    !global_loader.IsEdgeProtected(PROTECT_ADD);
	GraphLoader graph_loader(color_loader, global_loader);
	graph_loader.LoadDocument(document, graph);
	value_loader.LoadGraph(graph_loader);
	value_loader.LoadColors(color_loader);
	value_loader.LoadDocument(document);
	rule_loader.LoadDocument(document, color_loader);
	path.LoadFromDocument(document);

	Load(); // Check for save file

	ResetViewport();
}

std::string Level::GetFile()
{
	return "assets/levels/" + category_id + "/" + level_id + ".xml";
}

void Level::Reset()
{
	pugi::xml_document document;
	std::string filename = GetFile();
	document.load_file(filename.c_str());
	graph.Clear();
	GraphLoader graph_loader(color_loader, global_loader);
	graph_loader.LoadDocument(document, graph);
}

int Level::GetPoints(bool check_if_invalid) const
{
	return point_calculator.Points(graph, check_if_invalid);
}

void Level::GetBestPoints()
{
	if (has_loaded_best)
		return;
	pugi::xml_document document;
	if (!document.load_file(SaveFilename(SLOT_BEST).c_str()))
	{
		has_loaded_best = false;
		return;
	}
	pugi::xml_node graph_node = document.child("graph");
	has_loaded_best = true;
	best_points = graph_node.attribute("points").as_int();
}

void Level::Render()
{
	MoveViewport();

	window->SetDrawColor(GraphColoring::BACKGROUND_COLOR);
	window->Clear();
	graph.Render(path.PathEdgeSet(), path.PathVertexSet(), path.LastVertex());

	window->SetDrawColor(TEXT_COLOR);

	int y = 50;
	window->SetTextSize(TITLE_SIZE);
	window->DrawText(title, gui::Position(window->GetWidth()/2, y),
			gui::Alignment::CENTER, gui::Alignment::TOP);

	y += TITLE_SIZE + 10;
	window->SetTextSize(DESCRIPTION_SIZE);
	std::string description_text = description;
	size_t semicolon_index;
	do { // Read description lines
		semicolon_index = description_text.find(';');
		std::string line = description_text.substr(0, semicolon_index);
		window->DrawText(line, gui::Position(window->GetWidth()/2, y),
				gui::Alignment::CENTER, gui::Alignment::TOP);
		description_text.erase(0, semicolon_index+1);
		y += DESCRIPTION_SIZE + 10;
	} while (semicolon_index != std::string::npos);

	window->SetTextSize(OBJECTIVE_SIZE);
	window->DrawText(objective, gui::Position(window->GetWidth()/2, y),
			gui::Alignment::CENTER, gui::Alignment::TOP);

	y += OBJECTIVE_SIZE + 10;
	RenderPoints(y);
	RenderRules();
}

void Level::RenderPoints(int y)
{
	if (window->IsKeyDown(GDK_KEY_p) && !window->IsControlDown())
	{
		color_loader.RenderColorPoints(window);
		return;
	}

	bool is_valid = rule_loader.IsValid(graph);
	int points = GetPoints();
	int objective = value_loader.ObjectivePoints(graph);

	GetBestPoints();
	if (!has_loaded_best || points > best_points)
		Save(SLOT_BEST);

	if (points >= objective && is_valid)
	{
		window->SetTextSize(PRESS_ESC_SIZE);
		window->SetDrawColor(SUCCESS_COLOR);
		window->DrawText("Press Control-N to go to the next level.",
				gui::Position(window->GetWidth()/2, y),
				gui::Alignment::CENTER, gui::Alignment::TOP);
		y += PRESS_ESC_SIZE + 10;
	}
	if (!is_valid)
		window->SetDrawColor(INVALID_COLOR);

	std::stringstream points_text;
	int invalid_points = GetPoints(false);
	points_text << "Points: " << invalid_points << "/" << objective;
	window->SetTextSize(POINTS_SIZE);
	window->DrawText(points_text.str(), gui::Position(window->GetWidth()/2, y),
			gui::Alignment::CENTER, gui::Alignment::TOP);
}

void Level::RenderRules()
{
	if (!window->IsKeyDown(GDK_KEY_r) || window->IsControlDown()) return;
	rule_loader.RenderRules(window);
}

void Level::MoveViewport()
{
	if (window->IsKeyDown(GDK_KEY_Up))
		viewport_position.y -= VIEW_MOVE_SPEED;
	if (window->IsKeyDown(GDK_KEY_Down))
		viewport_position.y += VIEW_MOVE_SPEED;

	if (window->IsKeyDown(GDK_KEY_Left))
		viewport_position.x -= VIEW_MOVE_SPEED;
	if (window->IsKeyDown(GDK_KEY_Right))
		viewport_position.x += VIEW_MOVE_SPEED;

}

void Level::ResetViewport()
{
	viewport_position.SetPos(0, 0);
}

std::string Level::SaveFilename(int slot) const
{

	std::string suffix;
	if (slot == SLOT_RECENT)
		suffix = "";
	else if (slot == SLOT_BEST)
		suffix = "_best";
	else
		suffix = std::string("_") + std::to_string(slot);

	return "saves/" + category_id + "/" + level_id + suffix + ".xml";
}

void Level::Save(int slot)
{
	pugi::xml_document document;
	pugi::xml_node graph_node = document.append_child("graph");
	graph_node.append_attribute("points") = GetPoints();
	GraphLoader graph_loader(color_loader, global_loader);
	graph_loader.WriteGraph(graph, graph_node);
	document.save_file(SaveFilename(slot).c_str());

	if (slot == SLOT_BEST)
	{
		has_loaded_best = false;
		UpdateLevelList();
	}
}

int Level::Load(int slot)
{
	pugi::xml_document document;
	if (document.load_file(SaveFilename(slot).c_str()))
	{
		graph.Clear();
		GraphLoader graph_loader(color_loader, global_loader);
		graph_loader.LoadDocument(document, graph);
		return 1;
	}
	else
	{
		return 0;
	}
}

void Level::UpdateLevelList()
{
	pugi::xml_document category_listing;
	category_listing.load_file(LevelSelect::LEVEL_LISTING_PATH);

	int points = GetPoints();
	int objective = value_loader.ObjectivePoints(graph);

	pugi::xml_node node = GetLevelNode(category_listing, category_id, level_id);

	if (node.attribute("points").empty())
		node.append_attribute("points") = points;
	else
		node.attribute("points") = points;

	if (points >= objective)
	{

		if (node.attribute("completed").empty())
			node.append_attribute("completed") = "t";
		else
			node.attribute("completed") = "t";
	}
	category_listing.save_file(LevelSelect::LEVEL_LISTING_PATH);
}

} // namespace graphcoloring
