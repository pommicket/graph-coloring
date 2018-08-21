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
#include "window_events_keyboard.hpp"
#include "window_events_mouse.hpp"

namespace gui
{

template<typename F>
int Window::AddToCallbackMap(std::map<guint,std::vector<F>>& callback_map,
    	guint id, F callback)
{
	if (callback_map.count(id))
	{
		// There are already callbacks for this key.
		callback_map[id].push_back(callback);
	}
	else
	{
		std::vector<F> callbacks = {callback};
		callback_map[id] = callbacks;
	}
	return callback_map[id].size()-1;
}

template<typename F>
std::vector<F> Window::CheckCallbackMap(
	std::map<guint,std::vector<F>>& callback_map, guint id)
{
	// mouse_x and mouse_y will be passed to the function if it is a
	// mouse_callback_t. Otherwise, they will be set to -1, and will
	// not be passed.
	if (callback_map.count(id))
	{
		// There are callbacks for this key
		return callback_map[id];
	}
	else
	{
		return std::vector<F>();
	}
}

template<typename F>
void Window::RemoveFromCallbackList(
	std::vector<F>& callback_list, int id)
{
	if (id < 0 || id >= (int)callback_list.size())
		utils::errors::Die("Invalid callback ID.");
	callback_list[id] = nullptr;
}

template int Window::AddToCallbackMap<Window::mouse_callback_t>(
		mouse_callback_map_t&, guint, mouse_callback_t);
template int Window::AddToCallbackMap<Window::callback_t>(
		callback_map_t&, guint, callback_t);

template std::vector<Window::mouse_callback_t>
	Window::CheckCallbackMap<Window::mouse_callback_t>(
	mouse_callback_map_t&, guint);
template std::vector<Window::callback_t>
	Window::CheckCallbackMap<Window::callback_t>(callback_map_t&, guint);


template void Window::RemoveFromCallbackList<Window::mouse_callback_t>(
	std::vector<mouse_callback_t>&, int);
template void Window::RemoveFromCallbackList<Window::callback_t>(
	std::vector<callback_t>&, int);
template void Window::RemoveFromCallbackList<Window::scroll_callback_t>(
	std::vector<scroll_callback_t>&, int);

void Window::InitializeEvents()
{
	gtk_widget_set_events(window, GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK
			| GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK
			| GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(G_OBJECT(window), "key-press-event",
        G_CALLBACK(GtkKeydownCallback), this);
    g_signal_connect(G_OBJECT(window), "key-release-event",
        G_CALLBACK(GtkKeyupCallback), this);
    g_signal_connect(G_OBJECT(window), "button-press-event",
    	G_CALLBACK(GtkMousedownCallback), this);
    g_signal_connect(G_OBJECT(window), "button-release-event",
    	G_CALLBACK(GtkMouseupCallback), this);
    g_signal_connect(G_OBJECT(window), "motion-notify-event",
    	G_CALLBACK(GtkMousemotionCallback), this);
    g_signal_connect(G_OBJECT(window), "scroll-event",
    	G_CALLBACK(GtkScrollCallback), this);
}

} // namespace gui
