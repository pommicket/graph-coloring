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

#ifndef GRAPHCOLORING_GUI_WINDOW_EVENTS_MOUSE_H_
#define GRAPHCOLORING_GUI_WINDOW_EVENTS_MOUSE_H_

#include <gtk/gtk.h>

namespace gui {

extern void GtkMousedownCallback(GtkWidget*, GdkEventButton* event,
	gpointer data);
extern void GtkMouseupCallback(GtkWidget*, GdkEventButton* event,
	gpointer data);
extern void GtkMousemotionCallback(GtkWidget*, GdkEventMotion* event,
	gpointer data);
extern void GtkScrollCallback(GtkWidget*, GdkEventScroll* event, gpointer data);

}


#endif /* GRAPHCOLORING_GUI_WINDOW_EVENTS_MOUSE_H_ */
