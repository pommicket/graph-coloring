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

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cairo/cairo-ft.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "utils/errors.hpp"
#include "window.hpp"

namespace gui {


void GtkDrawCallback(GtkWidget* widget, cairo_t* c, gpointer data)
{
    Window* win = (Window*) data;
    win->cr = c;
    win->Render();
}

gboolean QueueDraw(gpointer w)
{
	GtkWidget* window = (GtkWidget*) w;
	gtk_widget_queue_draw(window);
    return TRUE;
}

void Window::InitializeDrawingArea()
{
    GtkWidget* drawing_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request (drawing_area, 100, 100);
    g_signal_connect(G_OBJECT(drawing_area), "draw",
                     G_CALLBACK(&GtkDrawCallback), this);
    g_timeout_add(1000/FPS, QueueDraw, drawing_area);

    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Also initialize font
    FT_Library library;
    FT_Face font_face;
    if (FT_Init_FreeType(&library))
    	utils::errors::Die("Failed to initialize freetype.");
    if (FT_New_Face(library, "assets/fonts/DejaVuSans-ExtraLight.ttf",
    		0, &font_face))
    	utils::errors::Die("Failed to open font.");


    font = cairo_ft_font_face_create_for_ft_face(font_face, 0);
	const cairo_user_data_key_t key = {0};
	auto status = cairo_font_face_set_user_data (font, &key,
	                               font_face, (cairo_destroy_func_t) FT_Done_Face);
	if (status)
		utils::errors::Die("Failed to load font.");

}

int Window::SetRenderCallback(callback_t callback)
{
    render_callbacks.push_back(callback);
	is_render_callbacks_modified = true;
    return render_callbacks.size()-1;
}

void Window::RemoveRenderCallback(int id)
{
	RemoveFromCallbackList(render_callbacks, id);
	is_render_callbacks_modified = true;
}

void Window::Render()
{
	int width, height;
	gtk_window_get_size(GTK_WINDOW(window), &width, &height);
	size.SetPos(width, height);
	is_render_callbacks_modified = false;
    for (callback_t callback : render_callbacks)
    {
    	if (!callback) continue;
    	callback(this);
    	// If a callback modifies the list of render callbacks, give up on
    	// trying to read the whole list.
        if (is_render_callbacks_modified)
        	break;
    }
}




void Window::SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    cairo_set_source_rgba(cr,
        (double)r/255, (double)g/255, (double)b/255, (double)a/255);
}

void Window::SetDrawColor(Color color)
{
    uint8_t r, g, b, a;
    colors::Unpack(color, &r, &g, &b, &a);
    SetDrawColor(r, g, b, a);
}

void Window::SetLineWidth(int line_width)
{
    cairo_set_line_width(cr, line_width);
}

void Window::Clear()
{
    DrawRectangle(0, 0, GetWidth(), GetHeight(), true);
}

void Window::DrawRectangle(int x, int y, int w, int h, bool filled)
{
    cairo_rectangle(cr, x, y, w, h);
    if (filled)
        cairo_fill(cr);
    else
        cairo_stroke(cr);
}

void Window::DrawPoint(int x, int y)
{
    DrawRectangle(x, y, 1, 1);
}

void Window::DrawLine(int x1, int y1, int x2, int y2)
{
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);
}

void Window::DrawArc(int x, int y, int r, double startAngle, double endAngle,
    bool filled)
{
    cairo_arc(cr, x, y, r, startAngle, endAngle);
    if (filled)
        cairo_fill(cr);
    else
        cairo_stroke(cr);
}

void Window::DrawCircle(int x, int y, int r, bool filled)
{
    DrawArc(x, y, r, 0, 2*M_PI, filled);
}

void Window::DrawPolygon(std::vector<std::pair<int,int>> points, bool filled)
{
    if (points.size() < 2)
        utils::errors::Die("Trying to create polygon with less than 2 points.");
    cairo_move_to(cr, points[0].first, points[0].second);
    for (unsigned i = 1; i < points.size(); i++)
        cairo_line_to(cr, points[i].first, points[i].second);
    if (filled)
        cairo_fill(cr);
    else
        cairo_stroke(cr);
}

void Window::SetTextSize(int size)
{
	cairo_set_font_size(cr, size);
}

void Window::DrawText(const std::string& text, int x, int y)
{
	cairo_set_font_face(cr, font);
	cairo_move_to(cr, x, y);
	cairo_show_text(cr, text.c_str());
	cairo_stroke(cr);
}

void Window::DrawText(const std::string& text, Position pos,
		Alignment horizontal_align, Alignment vertical_align)
{
	int w, h, x, y;
	GetTextSize(text, &w, &h);
	x = pos.X();
	y = pos.Y();
	switch (horizontal_align)
	{
	case Alignment::LEFT:
		break;
	case Alignment::CENTER:
		x -= w/2;
		break;
	case Alignment::RIGHT:
		x -= w;
		break;
	}
	switch (vertical_align)
	{
	case Alignment::TOP:
		y += h;
		break;
	case Alignment::CENTER:
		y += h/2;
		break;
	case Alignment::BOTTOM:
		break;
	}
	DrawText(text, x, y);
}

cairo_surface_t* Window::GetSurface(const std::string& filename)
{
	cairo_surface_t* image;
	std::string path = "assets/images/" + filename + ".png";
	if (images.count(path))
	{
		image = images[path];
	}
	else
	{
		image = cairo_image_surface_create_from_png(path.c_str());
		if (cairo_surface_status(image) != CAIRO_STATUS_SUCCESS)
			utils::errors::Die("Failed to load image: " + path);
		images[path] = image;
	}
	return image;
}


void Window::DrawImage(const std::string& filename, int x, int y)
{
	cairo_surface_t* image = GetSurface(filename);
	cairo_set_source_surface(cr, image, x, y);
	cairo_paint(cr);
}

void Window::DrawImage(const std::string& filename, Position pos,
	Alignment horizontal_align, Alignment vertical_align)
{
	int w, h;
	GetImageSize(filename, &w, &h);
	DrawImage(filename, pos.AlignedX(horizontal_align, w),
		pos.AlignedY(vertical_align, h));
}

void Window::GetImageSize(const std::string& filename, int* w, int* h)
{
	cairo_surface_t* image = GetSurface(filename);
	if (w != nullptr)
		*w = cairo_image_surface_get_width(image);
	if (h != nullptr)
		*h = cairo_image_surface_get_height(image);
}

void Window::GetTextSize(const std::string& text, int* w, int* h)
{
	cairo_set_font_face(cr, font);
	cairo_text_extents_t extents;
	cairo_text_extents(cr, text.c_str(), &extents);
	if (w != nullptr)
		*w = extents.width;
	if (h != nullptr)
		*h = extents.height;
}



} // namespace gui
