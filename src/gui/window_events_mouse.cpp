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

#include "window.hpp"

#include <iostream>

#include "utils/errors.hpp"

namespace gui {


void GtkMousedownCallback(GtkWidget*, GdkEventButton* event, gpointer data)
{
	Window* win = (Window*)data;
	win->ProcessMousedown(event);
}

void Window::ProcessMousedown(GdkEventButton* event)
{
	mousemap[event->button] = true;
	is_mousedown_callbacks_modified = false;
	for (mouse_callback_t callback :
		CheckCallbackMap(mousedown_callbacks, event->button))
	{
		if (!callback) continue;
		callback(this, event->x, event->y);
		if (is_mousedown_callbacks_modified)
			break;
	}
}

int Window::SetMousedownCallback(mouse_callback_t callback, guint button)
{
	is_mousedown_callbacks_modified = true;
	return AddToCallbackMap(mousedown_callbacks, button, callback);
}

void Window::RemoveMousedownCallback(int id, guint button)
{
	is_mousedown_callbacks_modified = true;
	if (mousedown_callbacks.count(button) == 0)
		utils::errors::Die("Invalid button passed to remove mouse callback.");
	RemoveFromCallbackList(mousedown_callbacks[button], id);
}

void GtkMouseupCallback(GtkWidget*, GdkEventButton* event, gpointer data)
{
	Window* win = (Window*)data;
	win->ProcessMouseup(event);
}

void Window::ProcessMouseup(GdkEventButton* event)
{
	mousemap[event->button] = false;
	is_mouseup_callbacks_modified = false;
	for (mouse_callback_t callback : mouseup_callbacks[event->button])
	{
		if (!callback) continue;
		callback(this, event->x, event->y);
		if (is_mouseup_callbacks_modified)
			break;
	}

}

int Window::SetMouseupCallback(mouse_callback_t callback, guint button)
{
	is_mouseup_callbacks_modified = true;
	return AddToCallbackMap(mouseup_callbacks, button, callback);
}

void Window::RemoveMouseupCallback(int id, guint button)
{
	is_mouseup_callbacks_modified = true;
	if (mouseup_callbacks.count(button) == 0)
		utils::errors::Die("Invalid button passed to remove mouse callback.");
	RemoveFromCallbackList(mouseup_callbacks[button], id);
}

void GtkMousemotionCallback(GtkWidget*, GdkEventMotion* event, gpointer data)
{
	Window* win = (Window*)data;
	win->ProcessMousemotion(event);
}

void Window::ProcessMousemotion(GdkEventMotion* event)
{
	mouse_x = event->x;
	mouse_y = event->y;
	is_mousemotion_callbacks_modified = false;
	for (mouse_callback_t callback : mousemotion_callbacks)
	{
		if (!callback) continue;
		callback(this, event->x, event->y);
		if (is_mousemotion_callbacks_modified)
			break;
	}
}

int Window::SetMousemotionCallback(mouse_callback_t callback)
{
	is_mousemotion_callbacks_modified = true;
	mousemotion_callbacks.push_back(callback);
	return mousemotion_callbacks.size()-1;
}

void Window::RemoveMousemotionCallback(int id)
{
	is_mousemotion_callbacks_modified = true;
	RemoveFromCallbackList(mousemotion_callbacks, id);
}

void GtkScrollCallback(GtkWidget*, GdkEventScroll* event, gpointer data)
{
	Window* window = (Window*) data;
	window->ProcessScroll(event);
}

void Window::ProcessScroll(GdkEventScroll* event)
{
	is_scroll_callbacks_modified = false;
	for (scroll_callback_t callback : scroll_callbacks)
	{
		if (!callback) continue;
		callback(this, event->direction);
		if (is_scroll_callbacks_modified)
			break;
	}
}

int Window::SetScrollCallback(scroll_callback_t callback)
{
	is_scroll_callbacks_modified = true;
	scroll_callbacks.push_back(callback);
	return scroll_callbacks.size()-1;
}

void Window::RemoveScrollCallback(int id)
{
	is_scroll_callbacks_modified = true;
	RemoveFromCallbackList(scroll_callbacks, id);
}

bool Window::IsMouseDown(guint button) const { return mousemap.at(button); }

int Window::GetMouseX() const { return mouse_x; }
int Window::GetMouseY() const { return mouse_y; }

} // namespace gui

