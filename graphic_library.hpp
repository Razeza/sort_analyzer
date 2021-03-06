#ifndef GRAPH_LIB_GRAPHIC_LIBRARY_H
#define GRAPH_LIB_GRAPHIC_LIBRARY_H

#include "engine.cpp"
#include "event.cpp"




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Background   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Background: public Abstract_window
{
private:
    Image back_img;
    Color color;

protected:

    int thickness;
    Color line_color;

    Point real_size;

    Point get_size () const;

public:
    Background () = default;
    Background (const char* name, Point size, Point start = {0, 0});
    Background (Color init_color, Point size, Point start = {0, 0}, Color line_color = NO_COLOR, int thickness = 0);
    Background (const char* name, Point size, Point start, Point real_size);
    Background (Color init_color, Point size, Point start, Point real_size, Color line_color = NO_COLOR, int thickness = 0);

    void change_coordinates (Point shift);
    Point get_start () const;

    Image* get_image ();

    virtual bool process_event (Event* event) override;
    virtual void render        ()             override;


    void change_size (Point new_size);
    void move (Point new_size);


    virtual ~Background () = default;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Hoverable   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Hoverable
{
public:
    virtual bool contains_point (Point mouse) const = 0;
    virtual void hover () = 0;
    virtual ~Hoverable () {};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Clickable   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Clickable: public Hoverable
{
public:
    virtual bool clicked (Point mouse) = 0;
    virtual ~Clickable () {};
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Button   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Action>
class Button: public Background, public Clickable
{
private:
    Action action;
    int thickness;
    Mouse_button_event::Mouse_button button_to_press;
public:

    Button() = default;

    Button (Action action_init, const char* name, Point size, Point start = {0, 0}, Mouse_button_event::Mouse_button init_button = Mouse_button_event::LEFT_BUTTON);
    Button (Action action_init, Color color     , Point size, Point start = {0, 0}, Color line_color = NO_COLOR, int thickness = 0, Mouse_button_event::Mouse_button init_button = Mouse_button_event::LEFT_BUTTON);

    virtual bool contains_point (Point mouse) const override;
    virtual void hover    () override;
    virtual bool clicked  (Point mouse) override;
    virtual void render   () override;
    virtual bool process_event (Event* event) override;

    virtual ~Button () { }

    void change_action (Action new_acton);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Key_functor   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Key_functor
{
    Key_functor () = default;
    Key_functor (keys init_key):
            key (init_key)
    { }

    keys key;
    void operator () ()
    {
        add_event (new Keybord_event (key));
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Window   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Window: public Background, public Clickable
{
private:
    struct Close_functor
    {
        Close_functor (Abstract_window* init):
                window (init)
        { }

        Abstract_window* window;
        void operator () ()
        {
            add_event (new Close_window_event(window));
        }
    };

    Button<Close_functor> close_window;

protected:


    Point window_size;
    Point window_start;
    Point frame_size;



public:
    Window (const char* name,
            Point window_size,
            Point window_start,
            Point frame_size,
            double button_width,
            Point real_size,
            Point image_size,
            Point image_start);

    Window (Color color,
            Point window_size,
            Point window_start,
            Point frame_size,
            double button_width,
            Point real_size);


    virtual bool contains_point (Point mouse) const override;
    virtual void hover    () override;
    virtual bool clicked  (Point mouse) override;
    virtual void render   () override;
    virtual bool process_event (Event* event) override;
    virtual ~Window ();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Window_manager   /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Window_manager: public Abstract_window
{
protected:
    std::vector<Abstract_window*> windows;
    bool top_manager;
public:

    enum codes {
        CLOSE = -1,
        OK    = 0
    };

    Window_manager (std::vector<Abstract_window*> init_windows, bool init_top_manager);

    codes manage_windows ();

    virtual bool process_event (Event* event) override;

    virtual void render () override ;
};

class Scrollbar: public Abstract_window, public Clickable
{
private:
    Point start;
    Point size;

    char what;

    bool pressed = false;

    Point real_size;
    Point shown_size;

    struct Scroller: Abstract_window
    {
        Point cur_place;
        Point start;
        Point size;

        Color color;

        bool contains_point (Point mouse) const;

        Scroller (Point size, Point start, Color init_color);

        virtual void render ();

        virtual bool process_event (Event* event);
    };



    Button<Key_functor> page_up;
    Button<Key_functor> page_down;
    Button<Key_functor> rect;
    Scroller            scroller;

    void set_up_down (double shift);
    void set_right_left (double shift);

public:

    void set_real_size (Point size);

    Scrollbar() = default;

    Scrollbar(Point start, Point size, Point scroller_size,
              Color init_scroller_color,
              const char* button_up, const char* button_down,
              Point init_real_size, Point shown_size, char kind, Color color = {160, 160, 160});

    virtual bool contains_point (Point mouse) const override;
    virtual bool clicked        (Point mouse)       override;

    virtual void hover  () override;
    virtual void render () override;

    virtual bool process_event (Event* event) override;

    virtual ~Scrollbar () = default;

    void up_down    (double shift);
    void right_left (double shift);
};

enum bar
{
    Y_BAR   = 0,
    X_BAR   = 1,
    X_Y_BAR = 2
};

void page_up    (Scrollable* image, double shift);
void page_left  (Scrollable* image, double shift);
void page_down  (Scrollable* image, double shift);
void page_right (Scrollable* image, double shift);

class View_port: public Abstract_window {
public:

    struct Scroller_settings {
        Point scroller_size;
        Color scroller_color;
    };

    struct Settings {
        Point button_size;
        Point left_button;
        Point right_button;
        Color color;
        Scroller_settings scrl_settings;
    };

private:
    Scrollable* scrollable_image;

    Scrollbar bar[2];
    Settings bar_setting[2];

    enum bar kind_of_bar = X_Y_BAR;



    bool pressed = false;


    bool process_scroll_event (Event* event);
    bool process_button_event (Event* event);
    bool process_key_event    (Event* event);
public:


    virtual bool process_event (Event* event) override;

    virtual ~View_port () = default;
    virtual void render () override;

    View_port (Scrollable* image, Settings settings[2]);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Exit_functor   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Exit_functor
{
    [[noreturn]] void operator () ();
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Draw Functions   ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_connected_line (std::vector<Point> points, std::size_t size, Color color = {-1, -1, -1}, int line_thickness = 10);

void draw_circled_rectangle (Point start, Point size, Color fill_color, Color line_color, int thickness);


#endif //GRAPH_LIB_GRAPHIC_LIBRARY_H
