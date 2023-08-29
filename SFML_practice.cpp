#include <SFML/Graphics.hpp> 
#include <SFML/System/Vector3.hpp>
// #include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
using namespace sf;

int scene = 0;

class Button{
public:
    void position(float width, float high, float x, float y);
    void text(String name);
    bool sensorarea(Vector2i mouse, int scene_num);
    RectangleShape button_shape;
    Text button_text;
    Font font;
private:
    float left, right, top, bottom;
};
void Button::position(float width, float high, float x, float y){
    button_shape.setSize(Vector2f(width, high));
    button_shape.setPosition(Vector2f(x, y));
    left = x;
    right = x + width;
    top = y;
    bottom = y + high;
}
void Button::text(String name){
    font.loadFromFile("arial.ttf");
    button_text.setFont(font);
    button_text.setString(name);
    button_text.setPosition(Vector2f(left, top));
    button_text.setFillColor(sf::Color::Black);
}
bool Button::sensorarea(Vector2i mouse, int scene_num){
    if (mouse.x >= left && mouse.y >= top && mouse.x <= right && mouse.y <= bottom && Mouse::isButtonPressed(Mouse::Left) && scene == scene_num) {
        button_shape.setFillColor(Color(255, 255, 255, 200));
        return true;
    } else {
        button_shape.setFillColor(Color(255, 255, 255, 255));
        return false;
    }
}

class Application{
public:
    Application();
    void work();
    void processInput_0();
    void processInput_1();
    void update_0();
    void update_1();
    void render_0();
    void render_1();
private:
    RenderWindow window;
    Clock clock;
    CircleShape circle0;
    Sprite sprite;
    Font font;
    Text text;
    Button play_button;
    Button end_button;

    float timer = 0, delay = 0.1;
    int x = 250, y = 250, i = 0;
    String sentance = "";
};
Application::Application(){
    srand(time(0));

    window.create(sf::VideoMode(500, 500), "SFML");
    window.setPosition(sf::Vector2i(10, 50));

    circle0.setRadius(3.f);
    circle0.setPosition(x, y);
    circle0.setScale(sf::Vector2f(1.f, 2.f));
    circle0.setFillColor(sf::Color::White);
    
    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setString(sentance);
    text.setFillColor(sf::Color(255, 255, 200, 255));

    play_button.position(100.f, 40.f, 250.f, 400.f);
    play_button.text("play");
    end_button.position(100.f, 40.f, 200.f, 400.f);
    end_button.text("leave");
}
void Application::work(){
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        if (scene == 0) {
            processInput_0();
            update_0();
            render_0();
        } else if (scene == 1) {
            processInput_1();
            update_1();
            render_1();
        }
    }
}
void Application::processInput_0(){
    Event e;
    while (window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            window.close();
        } else if (e.type == Event::TextEntered) {
            sentance += e.text.unicode;
        } else if (e.key.code == Keyboard::Key::Return) { 
            if (sentance == "exit") {
                window.close();
            }
            sentance.clear();
        } else if (e.type == Event::MouseWheelScrolled) {
            if (e.mouseWheelScroll.delta > 0) {
                y += 5;
            } else if (e.mouseWheelScroll.delta <= 0) {
                y -= 5;
            }
        } else if (e.type == Event::MouseMoved) {
            x = e.mouseMove.x;
            y = e.mouseMove.y;
        }
    }
}
void Application::processInput_1(){
    Event e;
    while (window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            window.close();
        }
    }
}
void Application::update_0(){
    circle0.setPosition(x, y);
    circle0.rotate(1.f);

    text.setString(sentance);

    if (play_button.sensorarea(Mouse::getPosition(window), 0)) {
        scene = 1;
    }else if (end_button.sensorarea(Mouse::getPosition(window), 1)) {
        sentance = "00000";
    }
    play_button.button_text.setString("play");
}
void Application::update_1(){
    if (end_button.sensorarea(Mouse::getPosition(window), 1)) {
        scene = 0;
    }
}
void Application::render_0(){
    window.clear();
    if (timer > delay) {
        timer = 0;
    }
    window.draw(circle0);
    window.draw(text);
    window.draw(play_button.button_shape);
    window.draw(play_button.button_text);
    window.display();
}
void Application::render_1(){
    window.clear();
    if (timer > delay) {
        timer = 0;
    }
    window.draw(end_button.button_shape);
    window.draw(end_button.button_text);
    window.display();
}

int main()
{
    Application app;
    app.work();

    return 0;
}