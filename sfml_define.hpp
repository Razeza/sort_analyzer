#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

enum keys
{
    PAGE_UP     = sf::Keyboard::PageUp,
    PAGE_DOWN   = sf::Keyboard::PageDown,
    ARROW_UP    = sf::Keyboard::Up,
    ARROW_DOWN  = sf::Keyboard::Down,
    ARROW_RIGHT = sf::Keyboard::Right,
    ARROW_LEFT  = sf::Keyboard::Left,
    ENTER       = sf::Keyboard::Enter
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Engine   /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Engine
{
public:

    Engine             ();
    ~Engine            ();

    Engine             (const Engine&) = delete;
    Engine& operator = (const Engine&) = delete;


private:
    sf::RenderWindow    window;
    sf::Font            cur_font;
    std::string         name_of_cur_font;

    std::queue<Event*> queue_of_events;


    // Почему-то я решил не делать статические методы, пожтому так,
    // хотел ,чтобы доступ к движку пользователь не имел
    friend void store_events   ();
    friend void set_fill_color (Color color);
    friend void set_line_color (Color color, int line_thickness);
    friend void create_window  (Point size);
    friend void render_window  ();
    friend bool load_font      (const char* name);
    friend void add_event      (Event* new_event);
    friend Event* get_event    ();
    friend bool empty_queue    ();
    friend void draw_line               (Point start, Point end, Color color, int line_thickness);
    friend void draw_rectangle          (Point start, Point end, Color color, int line_thickness);
    friend void draw_circle             (Point start, double r,  Color color, int line_thickness);
    friend Point get_mouse_coordinates  ();
    friend class Image;
    friend class Text;

    sf::Color cur_fill_color;
    sf::Color cur_line_color;
    double thickness;

    bool pressed = false;
    Mouse_button_event::Mouse_button pressed_button;
} xxx;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Events Functions   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Mouse_button_event::Mouse_button init_button_pressed (const sf::Event& event, Type_of_action action);
void init_key_pressed (const sf::Event& event, Type_of_action action);
void store_events     ();
void add_event        (Event* new_event);
Event* get_event      ();
bool empty_queue      ();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Window Functions   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_window (Point size);
void render_window ();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Shape Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_line_color          (Color color, int line_thickness = 10);
void set_fill_color          (Color color);
void set_line_and_fill_color (Color color, int line_thickness = 10);

void draw_line               (Point start, Point end, Color color = {-1, -1, -1}, int line_thickness = 10);
void draw_rectangle          (Point start, Point end, Color color = {-1, -1, -1}, int line_thickness = 10);
void draw_circle             (Point start, double r,  Color color = {-1, -1, -1}, int line_thickness = 10);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Text Functions   //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Text {
private:
    sf::Text str;
public:
    Text () = default;
    Text (const std::string& init_str);

    std::string get_str () const;

    void set_character_size (int size);
    void set_color (Color color);
    void set_position (Point pos);
    void set_font ();
    void set_str (const std::string& new_str);

    void render ();
};

bool load_font (const char* name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Scrollable   //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Scrollable {
public:
    virtual Point get_full_size () const = 0;
    virtual Point get_size      ()  const = 0;
    virtual Point get_cur_start () const = 0;

    virtual void shift_coordinates (Point) = 0;

    virtual Point get_scale () const = 0;
    virtual ~Scrollable () = 0;
};

Scrollable::~Scrollable ()
{ }




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class Image   //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ImageMemory;

struct Image: public Scrollable
{
public:
    friend class ImageMemory;

    Image ()                         = default;
    Image (const Image&)             = default;
    Image& operator = (const Image&) = default;

    virtual ~Image () {};

    Image (const char* name, Point size = {0, 0}, Point start = {0, 0});

    const sf::Uint8* data ();
private:

    sf::Image image;
    sf::Texture full_image;
    sf::Sprite drawable_image;

    Point size;
    Point shown_size;
    Point shift = {0, 0};

    Point start;

public:
    Image& operator= (Image&& new_image) noexcept;

    void update    (ImageMemory& memory);
    void draw      ();
    bool exist     () const;

    void set_pos (Point pos);

    virtual Point get_size      () const final;
    virtual Point get_full_size () const final;
    virtual Point get_cur_start () const final;

    Point get_start () const;
    Point get_pos   () const;

    bool contains_point (Point pos) const;

    virtual Point get_scale () const final;
    void set_scale (Point scale);

    virtual void shift_coordinates (Point shift) final;
    void set_shift (Point change);

    void change_size (Point new_size);

    void save_image (const std::string& name);
};


Image load_image (const char* name, Point size);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Class ImageMemory   ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ImageMemory {
private:
    friend class Image;

    sf::Uint8* memory;
    int width;
    int height;

public:
    ImageMemory& operator= (ImageMemory&& new_mem) noexcept;

    ImageMemory (int init_width, int init_height);
    void set_pixel (int i, Color color, int thickness = 1);
    void operator() (int i, Color color, int thickness = 1);
    void set_with_image (Image* img);

    Color get_pixel (int x, int y) const;

    void _memset (Color color);

    Point get_size () const;


    uint8_t* get_data ();

    ~ImageMemory ();

    void set_pixel (int x, int y, Color color, int thickness = 1);
    void operator() (int x, int y, Color color, int thickness = 1);
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Declaration of Button Functions   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Point get_mouse_coordinates ();

// returns 0 - if nothing pressed
//         1 - if left button pressed
//         2 - if right button pressed
Mouse_button_event::Mouse_button is_clicked ();
