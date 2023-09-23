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
using sf::Vector2i;
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


enum Mode {
    SPEED,
    DIRECTION,
    MASS
};


struct Object {
    double speed;
    double direction;
    double mass;
    CircleShape s;
    RectangleShape d;
};


void process_movement(vector<Object>& objects);

bool process_keys(Event::KeyEvent& key, vector<Object>& objects, Mode& mode);

void process_mouse_wheel(Event::MouseWheelScrollEvent& data, Mode mode, Object& object);

void process_mouse_move(Event::MouseMoveEvent& data, Object& object);

void process_mouse_button(Event::MouseButtonEvent& data, vector<Object>& objects, Object& object);

string get_mode(Mode& mode);


int main() {
    const u16 width = 1280;
    const u16 heigh = 720;
    const u16 FPS = 144;
    const double delay = (double)1000000 / (double)FPS;
    Mode mode = DIRECTION;
    /*
    double speed = 0;
    double direction = 0;
    double mass = 0;
    */
    vector<Object> objects;
    Object object{ .speed = 10, .direction = 0, .mass = 10, .s = CircleShape(10, 100), .d = RectangleShape(Vector2f(10, 3))};
    object.d.setPosition(object.s.getPosition().x, object.s.getPosition().y);

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
            "\nSPEED: " + to_string(object.speed) +
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
    /*
    double r2 = pow(objects[0].s.getPosition().x - objects[1].s.getPosition().x, 2) + pow(objects[0].s.getPosition().y - objects[1].s.getPosition().y, 2);
    double r = sqrt(r2);
    
    double cos = (objects[0].s.getPosition().x - objects[1].s.getPosition().x) / r;
    double sin = (objects[0].s.getPosition().y - objects[1].s.getPosition().y) / r;
    
    objects[0].vx += objects[1].m * -cos / r2 / 2;
    objects[1].vx += objects[0].m * cos / r2 / 2;

    objects[0].vy += objects[1].m * -sin / r2 / 2;
    objects[1].vy += objects[0].m * sin / r2 / 2;

    objects[0].s.move(Vector2f(objects[0].vx, objects[0].vy));
    objects[1].s.move(Vector2f(objects[1].vx, objects[1].vy));
    */
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
        object.speed += data.delta;
        object.d.setSize(Vector2f(object.speed, object.d.getSize().y));
        break;
    case DIRECTION:
        object.direction += data.delta;
        object.d.setRotation(object.direction);
        break;
    case MASS:
        object.mass += data.delta;
        object.s.setRadius(object.mass);
        object.s.setPosition(Vector2f(data.x - object.mass, data.y - object.mass));
        object.d.setPosition(object.s.getPosition().x + object.s.getRadius(), object.s.getPosition().y + object.s.getRadius());
        break;
    }
}


void process_mouse_move(Event::MouseMoveEvent& data, Object& object) {
    float r = object.s.getRadius();
    object.s.setPosition(Vector2f(data.x - r, data.y - r));
    object.d.setPosition(object.s.getPosition().x + object.s.getRadius(), object.s.getPosition().y + object.s.getRadius());
}


void process_mouse_button(Event::MouseButtonEvent& data, vector<Object>& objects, Object& object) {
    objects.push_back(object);
}


string get_mode(Mode& mode) {
    switch (mode) {
    case SPEED:
        return "SPEED";
    case DIRECTION:
        return "DIRECTION";
    case MASS:
        return "MASS";
    }
}