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

#ifndef GRAPHCOLORING_GUI_WINDOW_H_
#define GRAPHCOLORING_GUI_WINDOW_H_

#include <cstdint>
#include <functional>
#include <vector>
#include <map>

#include <gtk/gtk.h>

#include "colors.hpp"
#include "position.hpp"

namespace gui {


class Window
{
public:
	typedef std::function<void(Window*)> callback_t; // General 0-parameter callbacks.
	typedef std::map<guint, std::vector<callback_t>> callback_map_t;
	typedef std::function<void(Window*,int,int)> mouse_callback_t; // Mouse callbacks take x and y coordinates.
	typedef std::map<guint, std::vector<mouse_callback_t>> mouse_callback_map_t;
	typedef std::function<void(Window*,GdkScrollDirection)> scroll_callback_t;

    Window(const char* title, int width, int height, int fps = 30);
    virtual ~Window();
    // window_main.cpp methods
    int GetWidth() const;
    int GetHeight() const;
    Position GetPosition(int x = 0,int y = 0, double xrel = 0, double yrel = 0);
    const Size* GetSizePtr() const;
    void RemoveAllCallbacks();
    void OnActivate(std::function<void()> callback);
    void Quit();
    void Mainloop(); // Start application.
    // window_rendering.cpp methods
    int SetRenderCallback(callback_t callback);
    void RemoveRenderCallback(int id);
    void SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void SetDrawColor(Color color); // Sets color to value. Format: 0xRRGGBBAA.
    void SetLineWidth(int line_width);
    void Clear();
    void DrawRectangle(int x, int y, int w, int h, bool filled = true);
    void DrawPoint(int x, int y);
    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawArc(int x, int y, int r, double startAngle, double endAngle,
        bool filled = true);
    void DrawCircle(int x, int y, int r, bool filled = true);
    void DrawPolygon(std::vector<std::pair<int,int>> points, bool filled=true);
    void SetTextSize(int size);
    void DrawText(const std::string& text, int x, int y);
    void DrawText(const std::string& text, Position pos,
    	Alignment horizontal_align = Alignment::LEFT,
		Alignment vertical_align = Alignment::TOP);
    void DrawImage(const std::string& filename, int x, int y);
    void DrawImage(const std::string& filename, Position pos,
    	Alignment horizontal_align = Alignment::LEFT,
		Alignment vertical_align = Alignment::TOP);
    void GetImageSize(const std::string& filename, int* w, int* h);
    void GetTextSize(const std::string& text, int* w, int* h);

    // window_events_keyboard.cpp methods
    int SetKeydownCallback(callback_t callback, guint key);
    void RemoveKeydownCallback(int id, guint key);
    int SetKeyupCallback(callback_t callback, guint key);
    void RemoveKeyupCallback(int id, guint key);
    bool IsKeyDown(guint key) const;
    bool IsControlDown() const;

    // window_events_mouse.cpp
    int SetMousedownCallback(mouse_callback_t callback,
    	guint button = GDK_BUTTON_PRIMARY);
    void RemoveMousedownCallback(int id, guint button = GDK_BUTTON_PRIMARY);
    int SetMouseupCallback(mouse_callback_t callback,
    	guint button = GDK_BUTTON_PRIMARY);
    void RemoveMouseupCallback(int id, guint button = GDK_BUTTON_PRIMARY);
    int SetMousemotionCallback(mouse_callback_t callback);
    void RemoveMousemotionCallback(int id);
    int SetScrollCallback(scroll_callback_t callback);
    void RemoveScrollCallback(int id);
    bool IsMouseDown(guint button = GDK_BUTTON_PRIMARY) const;
    int GetMouseX() const;
    int GetMouseY() const;

private:
    // window_main.cpp methods
    void InitializeWindow();
    void InitializeApplication(const char* title);
    friend void Activate(GtkApplication*, gpointer);

    // window_events.cpp methods
    void InitializeEvents();

    template<typename F>
    static int AddToCallbackMap(std::map<guint,std::vector<F>>& callback_map,
		guint id, F callback);
    template<typename F>
    static std::vector<F> CheckCallbackMap(
    	std::map<guint,std::vector<F>>& callback_map, guint id);
    template<typename F>
    static void RemoveFromCallbackList(
    	std::vector<F>& callback_list, int id);

    friend void GtkKeydownCallback(GtkWidget*, GdkEventKey*, gpointer);
	void ProcessKeydown(GdkEventKey* event);
    friend void GtkKeyupCallback(GtkWidget*, GdkEventKey*, gpointer);
	void ProcessKeyup(GdkEventKey* event);

	friend void GtkMousedownCallback(GtkWidget*, GdkEventButton*, gpointer);
	void ProcessMousedown(GdkEventButton* event);

	friend void GtkMouseupCallback(GtkWidget*, GdkEventButton*, gpointer);
	void ProcessMouseup(GdkEventButton* event);

	friend void GtkMousemotionCallback(GtkWidget*, GdkEventMotion*, gpointer);
	void ProcessMousemotion(GdkEventMotion* event);

	friend void GtkScrollCallback(GtkWidget*, GdkEventScroll*, gpointer);
	void ProcessScroll(GdkEventScroll* event);

    // window_rendering.cpp methods
    friend void GtkDrawCallback(GtkWidget*, cairo_t*, gpointer);
    void InitializeDrawingArea();
    void Render();
    cairo_surface_t* GetSurface(const std::string& filename);

    // window_main.cpp members
    Size size;
    const char* title;
    const int FPS;
    GtkApplication* application;
    GtkWidget* window;
    std::function<void()> on_activate;

    // window_rendering.cpp members
    cairo_t* cr;
    cairo_font_face_t* font;
    std::map<std::string, cairo_surface_t*> images; // Only load each image once.
    std::vector<callback_t> render_callbacks;
    bool is_render_callbacks_modified;

    // window_events.cpp members
    mouse_callback_map_t mousedown_callbacks;
    bool is_mousedown_callbacks_modified;
    mouse_callback_map_t mouseup_callbacks;
    bool is_mouseup_callbacks_modified;
    std::vector<mouse_callback_t> mousemotion_callbacks;
    bool is_mousemotion_callbacks_modified;
    std::vector<scroll_callback_t> scroll_callbacks;
    bool is_scroll_callbacks_modified;
    callback_map_t keyup_callbacks;
    bool is_keyup_callbacks_modified;
    callback_map_t keydown_callbacks;
    bool is_keydown_callbacks_modified;
    std::map<guint, bool> keymap; // Which keys are down?
    std::map<guint, bool> mousemap; // Which mouse buttons are down?
    int mouse_x, mouse_y;
};

} // namespace gui

#endif // GRAPHCOLORING_GUI_WINDOW_H_
