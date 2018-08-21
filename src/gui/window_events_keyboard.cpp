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

#include "utils/errors.hpp"

namespace gui {

int Window::SetKeydownCallback(callback_t callback, guint key)
{
	is_keydown_callbacks_modified = true;
	return AddToCallbackMap(keydown_callbacks, key, callback);
}

void Window::RemoveKeydownCallback(int id, guint key)
{
	if (keydown_callbacks.count(key) == 0)
		utils::errors::Die("Invalid key passed to remove key callback.");
	RemoveFromCallbackList(keydown_callbacks[key], id);
	is_keydown_callbacks_modified = true;
}

void Window::ProcessKeydown(GdkEventKey* event)
{
	keymap[event->keyval] = true;
	is_keydown_callbacks_modified = false;
	for (callback_t callback
		: CheckCallbackMap<callback_t>(keydown_callbacks, event->keyval))
	{
		if (!callback) continue;
		callback(this);
		if (is_keydown_callbacks_modified)
			break;
	}
}

void GtkKeydownCallback(GtkWidget*, GdkEventKey* event, gpointer data)
{
    Window* win = (Window*)data;
    win->ProcessKeydown(event);
}


int Window::SetKeyupCallback(callback_t callback, guint key)
{
	is_keyup_callbacks_modified = true;
	return AddToCallbackMap(keyup_callbacks, key, callback);
}

void Window::RemoveKeyupCallback(int id, guint key)
{
	if (keyup_callbacks.count(key) == 0)
		utils::errors::Die("Invalid key passed to remove key callback.");
	RemoveFromCallbackList(keyup_callbacks[key], id);
	is_keyup_callbacks_modified = true;
}

void Window::ProcessKeyup(GdkEventKey* event)
{
	keymap[event->keyval] = false;
	is_keyup_callbacks_modified = false;
	for (callback_t callback : CheckCallbackMap(keyup_callbacks, event->keyval))
	{
		if (!callback) continue;
		callback(this);
		if (is_keyup_callbacks_modified)
			break;
	}
}

void GtkKeyupCallback(GtkWidget*, GdkEventKey* event, gpointer data)
{
    Window* win = (Window*)data;
    win->ProcessKeyup(event);
}

bool Window::IsKeyDown(guint key) const
{
	if (keymap.count(key))
		return keymap.at(key);
	else
		return false;
}

bool Window::IsControlDown() const
{
	return IsKeyDown(GDK_KEY_Control_L) || IsKeyDown(GDK_KEY_Control_R);
}

} // namespace gui

