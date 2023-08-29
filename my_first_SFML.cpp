#include <SFML/Graphics.hpp> 
#include <SFML/System/Vector3.hpp>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
using namespace sf;

int dir = 0;
int graph[8][3] = {{2, 4, 5}, {1, 3, 6}, {2, 4, 7}, {1, 3, 8}, {1, 6, 8}, {2, 5, 7}, {3, 6, 8}, {4, 5, 7}};

struct World
{
    float x, y, z, screen_x, screen_y;
} w[100];
struct Camera
{
    float x = 0, y = 0, z = 250, theta_x = 0, theta_y = 0, theta_z = 0;
}camera;

float Rotation1(float theta, float a, float b);
float Rotation2(float theta, float a, float b);
float Rotation3(float theta, float a, float b);
void SpawnObject()
{
    w[1].x = -50;
    w[1].y = 50;
    w[1].z = 50;
    w[2].x = 50;
    w[2].y = 50;
    w[2].z = 50;
    w[3].x = 50;
    w[3].y = -50;
    w[3].z = 50;
    w[4].x = -50;
    w[4].y = -50;
    w[4].z = 50;
    w[5].x = -50;
    w[5].y = 50;
    w[5].z = -50;
    w[6].x = 50;
    w[6].y = 50;
    w[6].z = -50;
    w[7].x = 50;
    w[7].y = -50;
    w[7].z = -50;
    w[8].x = -50;
    w[8].y = -50;
    w[8].z = -50;
}
void SetCamera(int order)
{
    float x = camera.x, y = camera.y, z = camera.z;
    if (order == 1) {
        camera.theta_x += (float)5 / 180 * M_PI;
    } else if (order == 2) {
        camera.theta_x -= (float)5 / 180 * M_PI;
    } else if (order == 3) {
        camera.theta_z += (float)5 / 180 * M_PI;
    } else if (order == 4) {
        camera.theta_z -= (float)5 / 180 * M_PI;
    }
    x = Rotation1(camera.theta_z, 0, 0);
    y = Rotation2(camera.theta_z, 0, 0);
    z = Rotation1(camera.theta_y, 250, x);
    camera.x = Rotation3(camera.theta_y, x, 250);
    camera.y = Rotation1(camera.theta_x, y, z);
    camera.z = Rotation2(camera.theta_x, y, z);
}
void Mapping()
{
    float t, x, y, z;
    sf::Vector3f vec(0, 0, 0);

    for (int i = 1; i <= 8; i++) {
        vec.x = w[i].x - camera.x;
        vec.y = w[i].y - camera.y;
        vec.z = w[i].z - camera.z;
        t = -(camera.x*camera.x+camera.y*camera.y+camera.z*camera.z) / (camera.x*vec.x + camera.y*vec.y + camera.z*vec.z);
        vec.x = vec.x*t + camera.x;
        vec.y = vec.y*t + camera.y;
        vec.z = vec.z*t + camera.z;
        y = Rotation1(-camera.theta_x, vec.y, vec.z);
        z = Rotation2(-camera.theta_x, vec.y, vec.z);
        x = Rotation3(-camera.theta_y, vec.x, z);
        z = Rotation1(-camera.theta_y, z, vec.x);
        w[i].screen_x = Rotation1(-camera.theta_z, x, y);
        w[i].screen_y = Rotation2(-camera.theta_z, x, y);
    }
}
float Rotation1(float theta, float a, float b){
    float output;
    output = (float)(cos(theta)*a - sin(theta)*b);
    return output;
}
float Rotation2(float theta, float a, float b){
    float output;
    output = (float)(sin(theta)*a + cos(theta)*b);
    return output;
}
float Rotation3(float theta, float a, float b){
    float output;
    output = (float)(cos(theta)*a + sin(theta)*b);
    return output;
}

int main()
{
    srand(time(0));

    RenderWindow window(sf::VideoMode(500, 500), "SFML");

    CircleShape circle0(3.f), circle1(3.f), dot(1.f);
    circle0.setFillColor(sf::Color::White);
    circle1.setFillColor(sf::Color::Green);

    Clock clock;
    float timer = 0, delay = 0.1;

    SpawnObject();
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            dir = 1;
        } else if (Keyboard::isKeyPressed(Keyboard::Up)) {
            dir = 2;
        } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
            dir = 3;
        } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
            dir = 4;
        } else {
            dir = 0;
        }
        if (timer > delay) {
            timer = 0;
            SetCamera(dir);
            dir = 0;
            Mapping();    
        }

        window.clear();
        // for (int i = 1; i <= 4; i++) {
        //     circle0.setPosition(w[i].screen_x + 250, -w[i].screen_y + 250);
        //     window.draw(circle0);
        // }
        float vec_x, vec_y, t = 2;
        for (int i = 1; i <= 8; i++) {
            circle1.setPosition(w[i].screen_x + 250, -w[i].screen_y + 250);
            window.draw(circle1);
            for (int j = 0; j < 3; j++) {
                vec_x = w[graph[i-1][j]].screen_x - w[i].screen_x;
                vec_y = (w[graph[i-1][j]].screen_y - w[i].screen_y) / (float)abs(vec_x);
                while (t < vec_x) {
                    dot.setPosition((w[i].screen_x + t) + 250, -(w[i].screen_y + vec_y * t) + 250);
                    window.draw(dot);
                    t += 2;
                }
                t= 2;             
            }
            for (int j = 0; j < 3; j++) {
                vec_y = w[graph[i-1][j]].screen_y - w[i].screen_y;
                vec_x = (w[graph[i-1][j]].screen_x - w[i].screen_x) / (float)abs(vec_y);
                while (t < vec_y) {
                    dot.setPosition((w[i].screen_x + vec_x * t) + 250, -(w[i].screen_y + t) + 250);
                    window.draw(dot);
                    t += 2;
                }
                t= 2;             
            }
        }
        
        // for (int i = 1; i <= 8; i++) {
        //     for (int j = 0; j < 3; j++) {
        //         vec_x = w[graph[i-1][j]].screen_x - w[i].screen_x;
        //         vec_y = (w[graph[i-1][j]].screen_y - w[i].screen_y) / (float)abs(vec_x);
        //         while (t < vec_x) {
        //             dot.setPosition((w[i].screen_x + t) + 250, -(w[i].screen_y + vec_y * t) + 250);
        //             window.draw(dot);
        //             t += 2;
        //         }
        //         t= 2;             
        //     }
        // }
        window.display();
    }

    return 0;
}