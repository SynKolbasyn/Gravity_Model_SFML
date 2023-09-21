#include <cmath>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>


using std::to_string;
using std::vector;

using sf::Font;
using sf::Text;
using sf::Time;
using sf::Mouse;
using sf::Clock;
using sf::Event;
using sf::Color;
using sf::Vector2i;
using sf::Vector2f;
using sf::VideoMode;
using sf::CircleShape;
using sf::microseconds;
using sf::RenderWindow;
using sf::RectangleShape;
using sf::ContextSettings;


struct Object {
    double vx; // vx - speed x
    double vy; // vy - speed y
    double m; // m - mass
    CircleShape s;
};


void process_movement(vector<Object>& objects);

void reset_buttons_color(
    RectangleShape& b_speed_x_up,
    RectangleShape& b_speed_x_down,
    RectangleShape& b_speed_y_up,
    RectangleShape& b_speed_y_down,
    RectangleShape& b_mass_up,
    RectangleShape& b_mass_down,
    RectangleShape& b_start,
    RectangleShape& b_reset
);

void process_mouse(
    Vector2i position,
    bool& flag,
    const unsigned short int& width,
    const unsigned short int& heigh,
    vector<Object>& objects,
    double& speed_x,
    double& speed_y,
    double& mass,
    RectangleShape& b_speed_x_up,
    RectangleShape& b_speed_x_down,
    RectangleShape& b_speed_y_up,
    RectangleShape& b_speed_y_down,
    RectangleShape& b_mass_up,
    RectangleShape& b_mass_down,
    RectangleShape& b_start,
    RectangleShape& b_reset
);

void draw_ui(
    RenderWindow& window,
    RectangleShape& bg,
    RectangleShape& sep_line,
    RectangleShape& b_speed_x_up,
    RectangleShape& b_speed_x_down,
    RectangleShape& b_speed_y_up,
    RectangleShape& b_speed_y_down,
    RectangleShape& b_mass_up,
    RectangleShape& b_mass_down,
    RectangleShape& b_start,
    RectangleShape& b_reset,
    Text& t_speed_x_up,
    Text& t_speed_x_down,
    Text& t_speed_y_up,
    Text& t_speed_y_down,
    Text& t_mass_up,
    Text& t_mass_down,
    Text& t_speed_x,
    Text& t_speed_y,
    Text& t_mass,
    Text& t_start,
    Text& t_reset
);


int main() {
    const unsigned short int width = 1280;
    const unsigned short int heigh = 720;
    const unsigned short int FPS = 144;
    const double delay = (double)1000000 / (double)FPS;

    double speed_x = 0;
    double speed_y = 0;
    double mass = 0;

    vector<Object> objects;

    ContextSettings settings(0, 0, 16, 4, 6, ContextSettings::Attribute::Default, true);
    RenderWindow window(VideoMode(width, heigh), "Gravity model", sf::Style::Default, settings);

    Font font;
    if (!font.loadFromFile("resources/fonts/calibri.ttf")) return -1;
    Text fps_text("FPS: 0", font, 20);
    fps_text.setFillColor(Color::White);
    fps_text.setStyle(Text::Style::Regular);

    RectangleShape bg(Vector2f(280, heigh));
    RectangleShape sep_line(Vector2f(5, heigh));
    RectangleShape b_speed_x_up(Vector2f(30, 30));
    RectangleShape b_speed_x_down(Vector2f(30, 30));
    RectangleShape b_speed_y_up(Vector2f(30, 30));
    RectangleShape b_speed_y_down(Vector2f(30, 30));
    RectangleShape b_mass_up(Vector2f(30, 30));
    RectangleShape b_mass_down(Vector2f(30, 30));
    RectangleShape b_start(Vector2f(100, 30));
    RectangleShape b_reset(Vector2f(100, 30));
    Text t_speed_x_down("-", font, 40);
    Text t_speed_x_up("+", font, 40);
    Text t_speed_y_down("-", font, 40);
    Text t_speed_y_up("+", font, 40);
    Text t_mass_up("+", font, 40);
    Text t_mass_down("-", font, 40);
    Text t_speed_x("SPEED X: 0", font, 20);
    Text t_speed_y("SPEED Y: 0", font, 20);
    Text t_mass("MASS: 0", font, 20);
    Text t_start("START", font, 20);
    Text t_reset("RESET", font, 20);

    bg.setPosition(Vector2f(1000, 0));
    sep_line.setPosition(Vector2f(1000, 0));
    b_speed_x_down.setPosition(Vector2f(1010, 100));
    b_speed_x_up.setPosition(Vector2f(1050, 100));
    b_speed_y_down.setPosition(Vector2f(1010, 200));
    b_speed_y_up.setPosition(Vector2f(1050, 200));
    b_mass_down.setPosition(Vector2f(1010, 300));
    b_mass_up.setPosition(Vector2f(1050, 300));
    b_start.setPosition(Vector2f(1090, heigh - 100));
    b_reset.setPosition(Vector2f(1090, heigh - 200));
    t_speed_x_down.setPosition(Vector2f(1019, 86));
    t_speed_x_up.setPosition(Vector2f(1056, 86));
    t_speed_y_down.setPosition(Vector2f(1019, 186));
    t_speed_y_up.setPosition(Vector2f(1056, 186));
    t_mass_down.setPosition(Vector2f(1019, 286));
    t_mass_up.setPosition(Vector2f(1056, 286));
    t_speed_x.setPosition(Vector2f(1090, 100));
    t_speed_y.setPosition(Vector2f(1090, 200));
    t_mass.setPosition(Vector2f(1090, 300));
    t_start.setPosition(Vector2f(1115, heigh - 98));
    t_reset.setPosition(Vector2f(1115, heigh - 198));

    bg.setFillColor(Color::Black);
    t_speed_x_up.setFillColor(Color::Black);
    t_speed_x_down.setFillColor(Color::Black);
    t_speed_y_up.setFillColor(Color::Black);
    t_speed_y_down.setFillColor(Color::Black);
    t_mass_up.setFillColor(Color::Black);
    t_mass_down.setFillColor(Color::Black);
    t_start.setFillColor(Color::Black);
    t_reset.setFillColor(Color::Black);

    bool start_flag = false;

    Clock clock;
    Time time = microseconds(1);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }

        reset_buttons_color(b_speed_x_up, b_speed_x_down, b_speed_y_up, b_speed_y_down, b_mass_up, b_mass_down, b_start, b_reset);

        if (Mouse::isButtonPressed(Mouse::Left)) {
            process_mouse(Mouse::getPosition(window), start_flag, width, heigh, objects, speed_x, speed_y, mass, b_speed_x_up, b_speed_x_down, b_speed_y_up, b_speed_y_down, b_mass_up, b_mass_down, b_start, b_reset);
        }

        fps_text.setString("FPS: " + to_string(1000000 / time.asMicroseconds()));

        if (start_flag) process_movement(objects);

        t_speed_x.setString("SPEED X: " + to_string(speed_x));
        t_speed_y.setString("SPEED Y: " + to_string(speed_y));
        t_mass.setString("MASS: " + to_string(mass));

        window.clear();
        window.draw(fps_text);
        for (Object& o : objects) window.draw(o.s);
        draw_ui(
            window,
            bg, sep_line,
            b_speed_x_up, b_speed_x_down, b_speed_y_up, b_speed_y_down, b_mass_up, b_mass_down, b_start, b_reset,
            t_speed_x_up, t_speed_x_down, t_speed_y_up, t_speed_y_down, t_mass_up, t_mass_down, t_speed_x, t_speed_y, t_mass, t_start, t_reset
        );
        window.display();

        while (clock.getElapsedTime().asMicroseconds() < delay);
        time = clock.restart();
    }

    return 0;
}


void process_movement(vector<Object>& objects) {
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
}


void reset_buttons_color(
    RectangleShape& b_speed_x_up,
    RectangleShape& b_speed_x_down,
    RectangleShape& b_speed_y_up,
    RectangleShape& b_speed_y_down,
    RectangleShape& b_mass_up,
    RectangleShape& b_mass_down,
    RectangleShape& b_start,
    RectangleShape& b_reset
) {
    b_speed_x_up.setFillColor(Color::White);
    b_speed_x_down.setFillColor(Color::White);
    b_speed_y_up.setFillColor(Color::White);
    b_speed_y_down.setFillColor(Color::White);
    b_mass_up.setFillColor(Color::White);
    b_mass_down.setFillColor(Color::White);
    b_start.setFillColor(Color::White);
    b_reset.setFillColor(Color::White);
}


void process_mouse(
    Vector2i position,
    bool& flag,
    const unsigned short int& width,
    const unsigned short int& heigh,
    vector<Object>& objects,
    double& speed_x,
    double& speed_y,
    double& mass,
    RectangleShape& b_speed_x_up,
    RectangleShape& b_speed_x_down,
    RectangleShape& b_speed_y_up,
    RectangleShape& b_speed_y_down,
    RectangleShape& b_mass_up,
    RectangleShape& b_mass_down,
    RectangleShape& b_start,
    RectangleShape& b_reset
) {
    if (position.x >= 1010 && position.x <= 1040 && position.y >= 100 && position.y <= 130) {
        b_speed_x_down.setFillColor(Color::Black);
        speed_x -= 0.005;
    }
    if (position.x >= 1050 && position.x <= 1080 && position.y >= 100 && position.y <= 130) {
        b_speed_x_up.setFillColor(Color::Black);
        speed_x += 0.005;
    }
    if (position.x >= 1010 && position.x <= 1040 && position.y >= 200 && position.y <= 230) {
        b_speed_y_down.setFillColor(Color::Black);
        speed_y -= 0.005;
    }
    if (position.x >= 1050 && position.x <= 1080 && position.y >= 200 && position.y <= 230) {
        b_speed_y_up.setFillColor(Color::Black);
        speed_y += 0.005;
    }
    if (position.x >= 1010 && position.x <= 1040 && position.y >= 300 && position.y <= 330) {
        b_mass_down.setFillColor(Color::Black);
        mass -= 0.5;
    }
    if (position.x >= 1050 && position.x <= 1080 && position.y >= 300 && position.y <= 330) {
        b_mass_up.setFillColor(Color::Black);
        mass += 0.5;
    }
    if (position.x >= 1090 && position.x <= 1190 && position.y >= heigh - 100 && position.y <= heigh - 70 && objects.size() == 2) {
        b_start.setFillColor(Color::Black);
        flag = true;
    }
    if (position.x >= 1090 && position.x <= 1190 && position.y >= heigh - 200 && position.y <= heigh - 170) {
        b_reset.setFillColor(Color::Black);
        flag = false;
        speed_x = 0;
        speed_y = 0;
        mass = 0;
        objects.clear();
    }
    if (position.x < 1000 && objects.size() < 2 && !flag) {
        if (objects.size() == 1) {
            if (objects[0].s.getPosition().x == position.x && objects[0].s.getPosition().y == position.y) return;
        }
        Object o{ .vx = speed_x, .vy = speed_y, .m = mass, .s = CircleShape(10.f, 100) };
        o.s.setPosition(Vector2f(position.x, position.y));
        objects.push_back(o);
    }
}


void draw_ui(
    RenderWindow& window,
    RectangleShape& bg,
    RectangleShape& sep_line,
    RectangleShape& b_speed_x_up,
    RectangleShape& b_speed_x_down,
    RectangleShape& b_speed_y_up,
    RectangleShape& b_speed_y_down,
    RectangleShape& b_mass_up,
    RectangleShape& b_mass_down,
    RectangleShape& b_start,
    RectangleShape& b_reset,
    Text& t_speed_x_up,
    Text& t_speed_x_down,
    Text& t_speed_y_up,
    Text& t_speed_y_down,
    Text& t_mass_up,
    Text& t_mass_down,
    Text& t_speed_x,
    Text& t_speed_y,
    Text& t_mass,
    Text& t_start,
    Text& t_reset
) {
    window.draw(bg);
    window.draw(sep_line);
    window.draw(b_speed_x_up);
    window.draw(b_speed_y_up);
    window.draw(b_speed_x_down);
    window.draw(b_speed_y_down);
    window.draw(b_mass_up);
    window.draw(b_mass_down);
    window.draw(b_start);
    window.draw(b_reset);
    window.draw(t_speed_x_up);
    window.draw(t_speed_x_down);
    window.draw(t_speed_y_up);
    window.draw(t_speed_y_down);
    window.draw(t_mass_up);
    window.draw(t_mass_down);
    window.draw(t_speed_x);
    window.draw(t_speed_y);
    window.draw(t_mass);
    window.draw(t_start);
    window.draw(t_reset);
}
