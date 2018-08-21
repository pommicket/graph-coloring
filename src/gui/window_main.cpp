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

#include <iostream>

#include "utils/errors.hpp"
#include "window.hpp"

namespace gui {

void Window::InitializeWindow()
{
    // Initialize the Gtk Window
    window = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), GetWidth(), GetHeight());

    InitializeDrawingArea();
    InitializeEvents();

    gtk_widget_show_all(window);
    on_activate();
}

void Window::OnActivate(std::function<void()> callback)
{
	on_activate = callback;
}

void Window::RemoveAllCallbacks()
{
	keyup_callbacks.clear();
	keydown_callbacks.clear();
	mouseup_callbacks.clear();
	mousedown_callbacks.clear();
	mousemotion_callbacks.erase(mousemotion_callbacks.begin(),
		mousemotion_callbacks.end());
	scroll_callbacks.erase(scroll_callbacks.begin(), scroll_callbacks.end());
	render_callbacks.erase(render_callbacks.begin(), render_callbacks.end());
}

void Activate(GtkApplication*, gpointer data)
{
    // This will be called when the application activates.
    // It simply calls Window::InitializeWindow.
    Window* win = (Window*)data;
    win->InitializeWindow();
}

void Window::InitializeApplication(const char* t)
{
    title = t;
    application = gtk_application_new("com.pommicket.graphcoloring",
                                      G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application, "activate", G_CALLBACK(&Activate), this);
}

Window::Window(const char* title, int w, int h, int fps)
    : size(w,h), FPS(fps)
{
    InitializeApplication(title);
}

Window::~Window() {}

int Window::GetWidth()  const { return size.X();  }
int Window::GetHeight() const { return size.Y(); }

const Size* Window::GetSizePtr() const { return &size; }

Position Window::GetPosition(int x, int y, double xrel, double yrel)
{
	return Position(x, y, xrel, yrel, GetSizePtr());
}

void Window::Quit()
{
	g_application_quit(G_APPLICATION(application));
}

void Window::Mainloop()
{
    g_application_run(G_APPLICATION(application), 0, NULL);
    g_object_unref(application);
}

} // namespace gui
