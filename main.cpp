#include <cmath>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>


using std::string;
using std::to_string;
using std::vector;

using sf::Font;
using sf::Text;
using sf::Time;
using sf::Mouse;
using sf::Clock;
using sf::Event;
using sf::Color;
using sf::Keyboard;
using sf::Vector2f;
using sf::VideoMode;
using sf::CircleShape;
using sf::microseconds;
using sf::RenderWindow;
using sf::RectangleShape;
using sf::ContextSettings;


typedef short int i16;
typedef unsigned short int u16;
typedef int i32;
typedef unsigned int u32;
typedef long int i64;
typedef unsigned long int u64;


const float PI = 3.14159265358979323846;


enum Mode {
    SPEED,
    DIRECTION,
    MASS
};


struct Object {
    float speed_x;
    float speed_y;
    float direction;
    float mass;
    CircleShape s;
    RectangleShape d;

    float speed() {
        return sqrt(pow(speed_x, 2) + pow(speed_y, 2));
    }

    float get_direction() {
        return atan2(speed_y, speed_x) * 180 / PI;
    }
};


void process_movement(vector<Object>& objects);

bool process_keys(Event::KeyEvent& key, vector<Object>& objects, Mode& mode);

void process_mouse_wheel(Event::MouseWheelScrollEvent& data, Mode mode, Object& object);

inline void process_mouse_move(Event::MouseMoveEvent& data, Object& object);

inline void process_mouse_button(Event::MouseButtonEvent& data, vector<Object>& objects, Object& object);

inline string get_mode(Mode& mode);


int main() {
    const u16 width = 1280;
    const u16 heigh = 720;
    const u16 FPS = 144;
    const float delay = (float)1000000 / (float)FPS;
    Mode mode = DIRECTION;

    vector<Object> objects;
    Object object{ .speed_x = 1.1, .speed_y = 0, .direction = 0, .mass = 100, .s = CircleShape(5, 100), .d = RectangleShape(Vector2f(25, 3))};
    object.d.setPosition(object.s.getPosition().x + 10, object.s.getPosition().y + 10);

    ContextSettings settings(0, 0, 16, 4, 6, ContextSettings::Attribute::Default, true);
    RenderWindow window(VideoMode(width, heigh), "Gravity model", sf::Style::Default, settings);

    Font font;
    if (!font.loadFromFile("resources/fonts/calibri.ttf")) return -1;
    Text fps_text("FPS: 0", font, 20);
    fps_text.setFillColor(Color::White);
    fps_text.setStyle(Text::Style::Regular);

    bool start_flag = false;

    Clock clock;
    Time time = microseconds(1);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyReleased:
                start_flag = process_keys(event.key, objects, mode);
                break;
            case Event::MouseWheelScrolled:
                if (!start_flag) process_mouse_wheel(event.mouseWheelScroll, mode, object);
                break;
            case Event::MouseMoved:
                if (!start_flag) process_mouse_move(event.mouseMove, object);
                break;
            case Event::MouseButtonReleased:
                if (!start_flag) process_mouse_button(event.mouseButton, objects, object);
                break;
            }
        }

        fps_text.setString(
            "FPS: " + to_string(1000000 / time.asMicroseconds()) +
            "\nMODE: " + get_mode(mode) +
            "\nSPEED: " + to_string(object.speed()) +
            "\nDIRECTION: " + to_string(object.direction) +
            "\nMASS: " + to_string(object.mass)
        );

        if (start_flag) {
            process_movement(objects);
            window.clear();
            window.draw(fps_text);
            for (Object& o : objects) {
                window.draw(o.s);
                window.draw(o.d);
            }
        }
        else {
            window.clear();
            window.draw(object.s);
            window.draw(object.d);
            window.draw(fps_text);
            for (Object& o : objects) {
                window.draw(o.s);
                window.draw(o.d);
            }
        }
        
        window.display();

        while (clock.getElapsedTime().asMicroseconds() < delay);
        time = clock.restart();
    }

    return 0;
}


void process_movement(vector<Object>& objects) {
    u64 size = objects.size();

    for (u64 i = 0; i + 1 < size; ++i) {
        for (u64 j = i + 1; j < size; ++j) {
            float r2 = pow(objects[i].s.getPosition().x - objects[j].s.getPosition().x, 2) + pow(objects[i].s.getPosition().y - objects[j].s.getPosition().y, 2);
            float r = sqrt(r2);

            float cos = (objects[i].s.getPosition().x - objects[j].s.getPosition().x) / r;
            float sin = (objects[i].s.getPosition().y - objects[j].s.getPosition().y) / r;

            objects[i].speed_x += objects[j].mass * -cos / r2 / 2;
            objects[j].speed_x += objects[i].mass * cos / r2 / 2;

            objects[i].speed_y += objects[j].mass * -sin / r2 / 2;
            objects[j].speed_y += objects[i].mass * sin / r2 / 2;
        }
    }

    for (Object& object : objects) {
        object.direction = object.get_direction();
        Vector2f speed(object.speed_x, object.speed_y);
        object.s.move(speed);
        object.d.move(speed);
        object.d.setRotation(object.direction);
    }
}


bool process_keys(Event::KeyEvent& key, vector<Object>& objects, Mode& mode) {
    switch (key.scancode) {
    case Keyboard::Scan::Escape:
        objects.clear();
        return false;
    case Keyboard::Scan::Enter:
        return true;
    case Keyboard::Scan::S:
        mode = SPEED;
        break;
    case Keyboard::Scan::D:
        mode = DIRECTION;
        break;
    case Keyboard::Scan::M:
        mode = MASS;
        break;
    }
    return false;
}


void process_mouse_wheel(Event::MouseWheelScrollEvent& data, Mode mode, Object& object) {
    switch (mode) {
    case SPEED:
        float radians;
        radians = object.direction * PI / 180;
        object.speed_x += data.delta / 4 * cos(radians);
        object.speed_y += data.delta / 4 * sin(radians);
        object.direction = object.get_direction();
        object.d.setSize(Vector2f(object.s.getRadius() + object.speed() * 20, object.d.getSize().y));
        break;
    case DIRECTION:
        object.direction += data.delta * 5;
        object.d.setRotation(object.direction);
        float speed;
        speed = object.speed();
        radians = object.direction * PI / 180;
        object.speed_x = speed * cos(radians);
        object.speed_y = speed * sin(radians);
        break;
    case MASS:
        object.mass += data.delta * 10;
        float r = object.mass / 20;
        i32 x = data.x;
        i32 y = data.y;
        object.s.setRadius(r);
        object.s.setPosition(Vector2f(x - r, y - r));
        object.d.setPosition(x, y);
        object.d.setSize(Vector2f(object.s.getRadius() + object.speed() * 20, object.d.getSize().y));
        break;
    }
}


inline void process_mouse_move(Event::MouseMoveEvent& data, Object& object) {
    float r = object.s.getRadius();
    i32 x = data.x;
    i32 y = data.y;
    object.s.setPosition(Vector2f(x - r, y - r));
    object.d.setPosition(x, y);
}


inline void process_mouse_button(Event::MouseButtonEvent& data, vector<Object>& objects, Object& object) {
    objects.push_back(object);
}


inline string get_mode(Mode& mode) {
    switch (mode) {
    case SPEED:
        return "SPEED";
    case DIRECTION:
        return "DIRECTION";
    case MASS:
        return "MASS";
    }
}