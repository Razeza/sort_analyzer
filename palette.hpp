#include "lib_palitra.cpp"
#include "engine.cpp"

class Palette: public Abstract_window, public Abstract_tool {
private:

    bool open = false;

    friend class ToolManager;

    struct Palette_action {
        void operator () () {
            add_event (new Open_palette ());
        }
    };

    Point start = {0, 0};
    Point size = {1000, 835};

    Palitra palette;
    Canvas cur_color;
    Button<Palette_action>* button;

    Text text;

    bool contains_point (Point pos);
public:
    Palette (Button<Palette_action>* init_button, Palitra::Palitra_settings settings);

    Palitra* get_palette ();

    virtual void process (Canvas& img, Mouse_button_event* event, Color color, Point shift, int thickness) override {};

    void set_start (Point new_start);
    void set_size (Point new_size);

    virtual void render () override;
    virtual bool process_event (Event* event) override;
    virtual ~Palette () = default;
};



