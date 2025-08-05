#include <iostream>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>

#include "planet.h"


using namespace std;

struct HeldMouseInfo 
{
    bool left_held;
    bool right_held;
    bool middle_held;
    sf::Vector2f left;
    sf::Vector2f right;
    sf::Vector2f middle;
    sf::CircleShape cursor = sf::CircleShape(10); 
    vector<sf::Vertex> line;
};

template <typename From, typename To>
sf::Vector2<To> castVector(sf::Vector2<From> v) 
{
    return sf::Vector2<To>((To) v.x, (To) v.y);
}


void renderingThread(sf::RenderWindow* window, vector<Planet>* planets, bool* run_thread, struct HeldMouseInfo* heldMouseInfo)
{
    // activate the window's context
    window->setActive(true);

    // the rendering loop
    while (*run_thread)
    {
        // Clear screen
        window->clear();

        // draw planets
        for (Planet &p : *planets) 
        {
            p.updateVisualPosition();
            window->draw(p.getVisual());
        }

        if (heldMouseInfo->left_held)
        {
            window->draw(heldMouseInfo->line.data(), 2, sf::PrimitiveType::Lines);
            window->draw(heldMouseInfo->cursor);
        }

        // Update the window
        window->display();
    }

    window->close();
}

void physicsThread(sf::RenderWindow* window, vector<Planet>* planets, bool* run_thread) 
{
    while (*run_thread) 
    {
        this_thread::sleep_for(chrono::microseconds(100));

        // update positions and do other gravity stuff
        for (long unsigned int i = 0; i < planets->size(); i++)
        {
            for (long unsigned int j = i + 1; j < planets->size(); j++)
            {
                if (i == j) continue;
                (*planets)[i].addPlanetForce(&(*planets)[j]);
            }
            (*planets)[i].applyGravity();
        }
    }

}


int main()
{
    XInitThreads();

    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "Gravitas");

    window.setVerticalSyncEnabled(true);
        
    // deactivate its OpenGL context
    if (!window.setActive(false)) return EXIT_FAILURE;

    vector<Planet> planets;

    Planet earth = Planet::makePlanet(5.9722e24 / 10e18, 10, 512, 384);
    Planet moon = Planet::makePlanet(7.346e22 / 10e18, 2.7, 812, 384);

    earth.setVelocity(0, -0.5525);
    moon.setVelocity(0, 44.89);

    planets.push_back(earth);
    planets.push_back(moon);

    bool run_thread = true;

    // needs scope outside of whiles and cannot be reset
    struct HeldMouseInfo mouseHeldInfo;
    mouseHeldInfo.cursor.setPosition(sf::Vector2f(-10, -10));

    // launch the rendering and physics thread
    thread render = thread(&renderingThread, &window, &planets, &run_thread, &mouseHeldInfo);
    thread physics = thread(&physicsThread, &window, &planets, &run_thread);

    // the event/logic/whatever loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>()) 
            {
                run_thread = false;
                physics.join();
                render.join();
            }
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) 
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    mouseHeldInfo.left = castVector<int, float>(mouseButtonPressed->position);

                    mouseHeldInfo.cursor.setPosition(sf::Vector2f(mouseHeldInfo.left.x - 10, mouseHeldInfo.left.y - 10));
                    mouseHeldInfo.left_held = true;
                }
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    mouseHeldInfo.right = castVector<int, float>(mouseButtonPressed->position);
                    mouseHeldInfo.right_held = true;
                }
                else if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
                {
                    mouseHeldInfo.middle = castVector<int, float>(mouseButtonPressed->position);
                    mouseHeldInfo.middle_held = true;
                }
            }
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) 
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {

                    Planet p = Planet::makePlanet(7.346e22 / 10e18, 2.7, mouseHeldInfo.left.x, mouseHeldInfo.left.y);

                    sf::Vector2f speed = castVector<int, float>(sf::Mouse::getPosition(window));
                    speed -= mouseHeldInfo.left;
                    speed = 0.5f * speed;

                    p.setVelocity(speed.x, speed.y);

                    planets.push_back(p);

                    mouseHeldInfo.left_held = false;
                }
                else if (mouseButtonReleased->button == sf::Mouse::Button::Right)
                {
                    mouseHeldInfo.right_held = true;
                }
                else if (mouseButtonReleased->button == sf::Mouse::Button::Middle)
                {
                    mouseHeldInfo.middle_held = true;
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                sf::Vertex start = sf::Vertex();
                sf::Vertex end = sf::Vertex();

                start.position = mouseHeldInfo.left;
                end.position = castVector<int, float>(sf::Mouse::getPosition(window));

                mouseHeldInfo.line = {start, end};
            }
        }


    }

    return EXIT_SUCCESS;
}