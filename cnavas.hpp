//
// Created by dunka on 24.11.2020.
//




#include "graphic_library.cpp"
#include "input_box.cpp"
#include <dlfcn.h>
#include <memory>
#include <cstdio>
#include "helper.cpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Canvas   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Canvas: public Image, public Clickable, public Abstract_window {
private:
    ImageMemory memory;
public:
    Canvas& operator= (Canvas&& new_canvas)  noexcept;

    Canvas (int x_screen, int y_screen, const std::string& file_name);
    Canvas (int init_x, int init_y, double width, double height);


    void set_pixel (int x, int y, Color color, int thickness = 1);
    void operator() (int x, int y, Color color, int thickness = 1);

    void set_pixel (int i, Color color, int thickness = 1);
    void operator() (int i, Color color, int thickness = 1);

    Color get_pixel (int x, int y) const;

    Point get_canvas_size () const;

    void update ();

    virtual bool contains_point (Point mouse) const override;
    virtual void hover  () override;
    virtual void render () override ;
    virtual bool process_event (Event* event) override;
    virtual bool clicked (Point mouse) override ;
    virtual ~Canvas () override;

    Point get_coordinates (Point x_y) const;

    uint8_t* get_data ();

    void memset (Color color);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Abstract_tool   //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Abstract_tool {
private:
    int thickness = 11;
    Color color = RED;
public:
    virtual ~Abstract_tool () = 0;
    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) = 0;

    void set_thickness (int init_thickness);
    void set_color     (Color init_color);

    int   get_thickness () const;
    Color get_color () const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class ToolManager   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "palette.cpp"
#include "plugins/api.hpp"

class ToolManager: public Abstract_window, public Clickable {
private:

    static const int number_of_tools = 9;
    static const int number_of_buttons = 9;

    Point tool_size = {32, 32};
    Point space = {10, 5};

    Point start_point = {0, 0};
    Color manger_color = GREEN;

    Point tool_manager_size  = {tool_size.x*number_of_tools + space.x*(number_of_tools + 1),
                                tool_size.y + 2*space.y};

    Abstract_window* buttons[number_of_buttons];
    Abstract_tool* tools[number_of_tools];

    std::vector<Image*> plugin_buttons;
    std::vector<PluginAPI::Plugin*> plugins;

    PluginAPI::Plugin* cur_plugin = nullptr;

    const Canvas_event::tools& cur_tool;
    Text thickness_text;

    const int init_thickness = 11;
    const Color init_color = RED;
public:
    ToolManager (const Canvas_event::tools& init_thickness, Palitra::Palitra_settings settings);

    Abstract_tool* operator [] (Canvas_event::tools i) {
        return tools[i];
    }
    
    void load_plugins (const std::vector<std::string>& path);

    PluginAPI::Plugin* get_cur_plugin ();

    virtual void render ();
    virtual bool process_event (Event* event);
    virtual bool contains_point (Point mouse) const {return false;};
    virtual void hover () { };
    virtual bool clicked (Point mouse) {return false;};
    virtual ~ToolManager ();
};

PluginAPI::Plugin* load_plugin (const char* str);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Tool Classes   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class Pencil: public Abstract_tool {
private:
    friend class ToolManager;

    struct Pencil_action {
        void operator () () {
            add_event (new Canvas_event(Canvas_event::PENCIL));
        }
    };

    Point last_pos = {-1, -1};

public:
    Pencil (Button<Pencil_action>* init_button);

    Button<Pencil_action>* button;

    virtual ~Pencil () = default;
    
    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) override;
};

class Eraser: public Abstract_tool {
private:
    friend class ToolManager;

    struct Eraser_action {
        void operator () () {
            add_event (new Canvas_event(Canvas_event::ERASER));
        }
    };

    Point last_pos = {-1, -1};

public:
    Eraser (Button<Eraser_action>* init_button);

    Button<Eraser_action>* button;

    virtual ~Eraser () = default;

    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) override;
};

class Change_thickness: public Abstract_tool {
private:
    friend class ToolManager;

    struct Change_thickness_action {
    public:
        enum direction {
            UP = true,
            DOWN = false
        };
    private:
        direction up;
    public:
        Change_thickness_action (direction dir):
                up (dir)
        { }

        void operator () () {
            add_event (new Thickness_event (up == UP ? 1 : -1));
        }
    };

public:
    Change_thickness (Button<Change_thickness_action>* init_button);

    Button<Change_thickness_action>* button;

    virtual ~Change_thickness () = default;

    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness)
    { }
};

class Zoom: public Abstract_tool {
private:
    friend class ToolManager;

    struct Zoom_action {
        void operator () () {
            add_event (new Canvas_event(Canvas_event::ZOOM));
        }
    };
public:
    Zoom (Button<Zoom_action>* init_button);

    Button<Zoom_action>* button;

    virtual ~Zoom () = default;

    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) override;
};

class Save: public Abstract_tool {
private:
    friend class ToolManager;

    struct Save_action {
        void operator () () {
            add_event (new Canvas_event (Canvas_event::SAVE));
        }
    };
public:
    Save (Button<Save_action>* init_button);

    Button<Save_action>* button;

    virtual ~Save () = default;

    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) override {};
};

class Trash: public Abstract_tool {
private:
    friend class ToolManager;

    struct Trash_action {
        void operator () () {
            add_event (new Canvas_event (Canvas_event::TRASH));
        }
    };
public:
    Trash (Button<Trash_action>* init_button);

    Button<Trash_action>* button;

    virtual ~Trash () = default;

    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) override {};
};

class Load_picture: Abstract_tool {
    friend class ToolManager;

    struct Load_action {
        void operator () () {
            add_event (new Canvas_event (Canvas_event::LOAD));
        }
    };
public:
    Load_picture (Button<Load_action>* init_button);

    Button<Load_action>* button;

    virtual ~Load_picture () = default;

    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) override {};
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Painter   ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Painter: public Abstract_window // Rename фасад
{
private:

    Canvas_event::tools current_tool = Canvas_event::NO_TOOL;
    const int max_thickness = 50;

    Canvas canvas;
    ToolManager tools_manager;

    char plugin_used = false;

    Point screen_size;

    bool process_button_event       (Event* event);
    bool process_change_color_event (Event* event);
    bool process_thickness_event    (Event* event);
    bool process_canvas_event       (Event* event);

public:
    Painter (Point screen_size, const std::string& file_name, Palitra::Palitra_settings settings);
    Painter (int init_x, int init_y, double width, double height, Palitra::Palitra_settings settings, Point screen_size);

    Canvas& get_canvas ();
    
    void load_plugins (const std::vector<std::string>& path);

    virtual void render () override ;
    virtual bool process_event (Event* event) override;
    virtual ~Painter () override;
};





