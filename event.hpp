//
// Created by dunka on 15.11.2020.
//

#ifndef GRAPH_LIB_EVENT_HPP
#define GRAPH_LIB_EVENT_HPP


#include "basics.cpp"
#include "abstract_classes.cpp"





enum event_type
{
    KEY_CLICKED     = 69,
    BUTTON_CLICKED  = 96,
    CLOSE_WINDOW    = 59,
    PROGRAM_CLOSE   = 89,
    SCROLL_EVENT    = 84,
    CANVAS_EVENT    = 70,
    THICKNESS_EVENT = 71,
    HUE_CHANGED     = 72,
    OPEN_PALETTE    = 73,
    CHANGED_COLOR   = 74,
    CHAR_ENTERED    = 75,
    NO_EVENT        = 0
};

enum Type_of_action {
    PRESSED,
    RELEASED
};




class Close_window_event: public Event
{
private:
    Abstract_window* to_close;
public:
    Abstract_window* get_window ();
    Close_window_event (Abstract_window* init_to_close);
    virtual ~Close_window_event () = default;
};

class Mouse_button_event: public Event
{
public:
    enum Mouse_button {
        NOTHING = 0,
        LEFT_BUTTON = 1,
        RIGHT_BUTTON = 2,
        MIDDLE_BUTTON
    };

    Point pos;
    Mouse_button button;
    Type_of_action action;

    Mouse_button_event (Point init_pos, Mouse_button init_button, Type_of_action init_action);
    virtual ~Mouse_button_event () = default;
};


class Keybord_event: public Event
{
public:
    int key;
    Type_of_action action;

    Keybord_event (int key, Type_of_action action = PRESSED);
    virtual ~Keybord_event () = default;
};



class Program_close: public Event
{
public:
    Program_close ();
    virtual ~Program_close () = default;
};

class Scroll_event: public Event
{
public:
    enum type {
        UP,
        DOWN,
        RIGHT,
        LEFT
    };

    type direction;
    double shift;
    Scroll_event (type init_direction, double init_shift);
    virtual ~Scroll_event () = default;
};

class Canvas_event: public Event {
public:
    enum tools {
        NO_TOOL = -1,
        PENCIL = 0,
        ERASER = 1,
        THICKNESS = 2,
        SHOW_THICKNESS = 3,
        ZOOM = 4,
        PALETTE = 5,
        SAVE = 6,
        TRASH = 7
    };

    tools tool;

    Canvas_event (tools init_tool);
    virtual ~Canvas_event () = default;
};

class Thickness_event: public Event {
public:

    int plus;
    Thickness_event (int plus);
    virtual ~Thickness_event () = default;
};

class Hue_event: public Event {
public:
    float hue;
    Hue_event (float init_hue);
    virtual ~Hue_event () = default;
};

class Open_palette: public Event {
public:
    Open_palette ();
    virtual ~Open_palette () = default;
};

class Changed_color: public Event {
public:
    Color color;

    Changed_color (Color init_color);
    virtual ~Changed_color () = default;
};

class Save_event: public Event {
public:
    Save_event ();
    virtual ~Save_event () = default;
};

class Text_event: public Event {
public:
    char symbol;

    Text_event (char init_char);
    virtual ~Text_event () = default;
};

class Trash_event: public Event {
public:
    Trash_event ();
    virtual ~Trash_event () = default;
};

#endif //GRAPH_LIB_EVENT_HPP
