#ifndef TX_DEFINE_HPP
#define TX_DEFINE_HPP


#include "D:\\TX\TXlib.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Engine   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Engine
{
public:

    Engine             ();
    ~Engine            ();

    Engine             (const Engine&) = delete;
    Engine& operator = (const Engine&) = delete;


private:
    std::vector<HDC>    images;
    std::vector<HWND>   windows;
    HFONT               cur_font;
    std::string         name_of_cur_font;

    std::queue<Event*> queue_of_events;
    friend void create_window (double      size_x, double size_y);
    friend bool load_font     (const char* name,   int    y_size);
    friend bool set_font      (double      size_y, double size_x);
    friend void add_event     (Event* new_event);
    friend Event* get_event   ();
    friend bool empty_queue   ();


    bool load_and_set_font (double size_y, double size_x);
    bool check_font        ();
} xxx;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Events Functions   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void store_events ();
void add_event    (Event* new_event);
Event* get_event   ();
bool empty_queue ();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Window Functions   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_window (double size_x, double size_y);
void render_window ();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Shape Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_line_color          (Color color, int line_thickness = 10);
void set_fill_color          (Color color);
void set_line_and_fill_color (Color color, int line_thickness = 10);

void draw_line               (double x0, double y0, double x1, double y1,                       Color color = {-1, -1, -1}, int line_thickness = 10);
void draw_rectangle          (double x0, double y0, double x1, double y1,                       Color color = {-1, -1, -1}, int line_thickness = 10);
void draw_triangle           (double x0, double y0, double x1, double y1, double x2, double y2, Color color = {-1, -1, -1}, int line_thickness = 10);
void draw_circle             (double x0, double y0, double r,                                   Color color = {-1, -1, -1}, int line_thickness = 10);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Text Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool load_font (const char* name,   double  y_size = 10);
bool set_font  (double      size_y, double  size_x);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Image   //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Image
{
public:
    Image ()                         = default;
    Image (const Image&)             = default;
    Image& operator = (const Image&) = default;


    Image (HDC img, double init_width, double init_height);
    ~Image ();

private:

    HDC image;
    double width;
    double height;

public:

    void draw      (double x = 0, double y = 0, double x_shift = 0, double y_shift = 0);
    bool exist     ();
    Point get_size ();
};

Image load_image (const char* name, double width, double height);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Button Functions   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Point get_mouse_coordinates ();

// returns 0 - if nothing pressed
//         1 - if left button pressed
//         2 - if right button pressed
Mouse_button_event::Mouse_button is_clicked ();

#endif