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

#include "graphcoloring.hpp"

#include "utils/windows.hpp"
#include "utils/filesystem.hpp"

#include <sstream>

namespace graphcoloring {

namespace filesystem = utils::filesystem;

const std::string GraphColoring::TITLE = "Graph Coloring";

GraphColoring::GraphColoring()
	: state(State::MAIN_MENU)
{
	window = new gui::Window("Graph Coloring", 800, 600);
	CheckSaves();
	window->OnActivate([this] (){ SwitchToMainMenu(); });
}

GraphColoring::~GraphColoring()
{
	DeleteAll();
	delete window;
}

void GraphColoring::CheckSaves()
{

	if (filesystem::directory_exists("saves"))
	{
		// Check for version mismatch
		pugi::xml_document saves_document;
		saves_document.load_file(LevelSelect::LEVEL_LISTING_PATH);
		pugi::xml_document assets_document;
		assets_document.load_file("assets/levels/level-list.xml");
		std::string saves_version(
			saves_document.child("category-listing")
						  .attribute("version").value());
		std::string assets_version(
				assets_document.child("category-listing")
							   .attribute("version").value());

		if (saves_version != assets_version)
		{
			// Copy new level list.
			filesystem::remove_file(LevelSelect::LEVEL_LISTING_PATH);
			filesystem::copy_file(
				"assets/levels/level-list.xml",
				LevelSelect::LEVEL_LISTING_PATH);
		}
		return;
	}

	// Create saves directory
	filesystem::create_directory("saves");
	filesystem::copy_file(
		"assets/levels/level-list.xml", LevelSelect::LEVEL_LISTING_PATH);

	pugi::xml_document document;
	document.load_file(LevelSelect::LEVEL_LISTING_PATH);
	for (pugi::xml_node category
		: document.child("category-listing").children("category"))
	{
		std::stringstream filename;
		filename << "saves/" << category.attribute("id").value();
		filesystem::create_directory(filename.str());
	}
}

void GraphColoring::DeleteAll()
{
	mainMenu = nullptr;
	levelSelect = nullptr;
	level = nullptr;
	window->RemoveAllCallbacks();
}

void GraphColoring::SwitchToMainMenu()
{
	DeleteAll();
	mainMenu = std::make_unique<MainMenu>(window,
		[this] (){ SwitchToLevelSelect(); });

	window->SetKeyupCallback([this] (gui::Window*){ // Press Escape to quit
		window->Quit();
	}, GDK_KEY_Escape);
}

void GraphColoring::SwitchToLevelSelect()
{
	DeleteAll();
	auto callback = [this](std::string category_id,std::string level_id) {
		SwitchToLevel(category_id, level_id);
	};
	levelSelect = std::make_unique<LevelSelect>(window, callback);
	window->SetKeyupCallback([this] (gui::Window*){ // Press Escape to go back to menu
		SwitchToMainMenu();
	}, GDK_KEY_Escape);
}

void GraphColoring::SwitchToLevel(std::string category_id, std::string level_id)
{
	DeleteAll();
	level = std::make_unique<Level>(window, category_id, level_id);
	window->SetKeyupCallback([this] (gui::Window*) { // Press Escape to go back to level selection
		level->Save();
		SwitchToLevelSelect();
	}, GDK_KEY_Escape);
	window->SetKeyupCallback([this] (gui::Window*) { // Press Control-N to go to the next level
		if (!window->IsControlDown()) return;
		level->Save();
		std::pair<std::string,std::string> next_level = level->NextLevel();
		if (next_level.first.empty()) // No next level
			SwitchToLevelSelect();
		else
			SwitchToLevel(next_level.first, next_level.second);
	}, GDK_KEY_n);
}

void GraphColoring::Start()
{
	window->Mainloop();
}


} // namespace graphcoloring
