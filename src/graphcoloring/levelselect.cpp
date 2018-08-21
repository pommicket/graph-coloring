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

#include "levelselect.hpp"

#include "utils/errors.hpp"
#include "graphcoloring.hpp"

namespace graphcoloring {

const char* const LevelSelect::LEVEL_LISTING_PATH = "saves/level-list.xml";

LevelSelect::LevelSelect(gui::Window* window_,
	level_click_callback_t level_click_callback_)
	: window(window_), level_click_callback(level_click_callback_)
{
	ReadCategories();
	window->SetRenderCallback([this](gui::Window*){Render();});
	window->SetScrollCallback([this](gui::Window*, GdkScrollDirection d) {
		if (d == GDK_SCROLL_UP)
			Scroll(-1);
		else if (d == GDK_SCROLL_DOWN)
			Scroll(+1);
	});
	MakeLevelButtons();
}

void LevelSelect::Scroll(int direction)
{
	int circle_radius =
		gui::Size(0,0,LEVEL_CIRCLE_RADIUS,0,&min_dimension_both).X();
	lowest_y = 0;
	for (auto& button : buttons)
		if (button->GetY() > lowest_y)
			lowest_y = button->GetY();
	if (direction == 1
	 && lowest_y < window->GetHeight()-10-2*circle_radius)
		return;
	scroll_position.y -= direction * 10;
	if (scroll_position.y > 0)
		scroll_position.y = 0;
}

pugi::xml_object_range<pugi::xml_named_node_iterator>
	LevelSelect::GetCategories()
{
	return category_listing.child("category-listing").children("category");
}

void LevelSelect::ButtonClicked(std::string category_id, std::string level_id)
{
	level_click_callback(category_id, level_id);
}

void LevelSelect::MakeLevelButtons()
{
	double y = LEVEL_CIRCLE_PADDING;
	total_points = 0;
	for (pugi::xml_node category : GetCategories())
	{
		std::string category_id(category.attribute("id").value());
		gui::Color color
			= gui::colors::FromAttribute(category.attribute("color"));

		categories_y_positions[category_id] = y; // Keep track of y position to write text to
		y += TEXT_SIZE + LEVEL_CIRCLE_PADDING; // Move past text
		double x_spacing = // This formula determines the amount of space between each button
			(1-LEVELS_PER_ROW*2*LEVEL_CIRCLE_RADIUS) / (LEVELS_PER_ROW+1)
			+ 2*LEVEL_CIRCLE_RADIUS;
		double x = x_spacing - 2*LEVEL_CIRCLE_RADIUS;

		int i = 1; // Index of level
		for (pugi::xml_node level : category.children("level"))
		{
			std::string level_id(level.attribute("id").value());
			total_points += level.attribute("points").as_int(0);

			if (x > 1.01-x_spacing) // Move down
			{
				x = x_spacing - 2*LEVEL_CIRCLE_RADIUS;
				y += 2 * LEVEL_CIRCLE_RADIUS + LEVEL_CIRCLE_PADDING;
			}
			bool completed = !level.attribute("completed").empty();
			auto button = std::make_unique<gui::Button>(window,
				std::to_string(i),
				gui::Position(0,0,x,y,&min_dimension_y,&scroll_position),
				gui::Size(0,0,LEVEL_CIRCLE_RADIUS,0,&min_dimension_both),
				completed ? gui::colors::Shade(color, COMPLETED_SHADE) : color,
				gui::Alignment::LEFT,
				gui::Alignment::TOP, gui::Button::Shape::CIRCLE);

			button->SetCommand([category_id, level_id, this]() {
				ButtonClicked(category_id, level_id);
			});

			buttons.push_back(std::move(button));
			x += x_spacing;

			i++;
		}
		y += 2 * LEVEL_CIRCLE_RADIUS + LEVEL_CIRCLE_PADDING;
	}
}

void LevelSelect::ReadCategories()
{
	if (!category_listing.load_file(LEVEL_LISTING_PATH))
		utils::errors::Die("Failed to load category listing file.");
}

void LevelSelect::Render()
{
	// Update coordinates
	int m = std::min(window->GetWidth(), window->GetHeight());
	min_dimension_both.SetPos(m,m);
	min_dimension_y.SetPos(window->GetWidth(),m);

	window->SetDrawColor(GraphColoring::BACKGROUND_COLOR);
	window->Clear();

	// Draw Buttons
	for (std::unique_ptr<gui::Button>& button : buttons)
		button->Render();

	RenderCategoryLabels();
	window->SetDrawColor(0xDDDDDDFF);
	window->DrawText(std::string("Points: ") + std::to_string(total_points),
		gui::Position(window->GetWidth()-10, 10),
		gui::Alignment::RIGHT, gui::Alignment::TOP);


}

void LevelSelect::RenderCategoryLabels()
{
	window->SetTextSize(TEXT_SIZE*min_dimension_both.X());
	for (pugi::xml_node category : GetCategories())
	{
		std::string id(category.attribute("id").value());
		std::string category_name = category.attribute("name").value();
		gui::Color color
			= gui::colors::FromAttribute(category.attribute("color"));
		window->SetDrawColor(color);
		double y_pos = categories_y_positions[id];
		window->DrawText(category_name,
			gui::Position(0,0, LEVEL_CIRCLE_PADDING,y_pos,&min_dimension_both,
				&scroll_position),
			gui::Alignment::LEFT, gui::Alignment::TOP);
	}
}

} // namespace graphcoloring
