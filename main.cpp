#include <iostream>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>

#include "planet.h"


using namespace std;

void renderingThread(sf::RenderWindow* window, vector<Planet>* planets, bool* run_thread)
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
    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "Gravitas");

    window.setVerticalSyncEnabled(true);
        
    // deactivate its OpenGL context
    if (!window.setActive(false)) return EXIT_FAILURE;

    vector<Planet> planets;

    Planet earth = Planet::makePlanet(5.9722e24 / 10e18, 10, 512, 384);
    Planet moon = Planet::makePlanet(7.346e22 / 10e18, 1.67, 812, 384);

    earth.setVelocity(0, -0.5525);
    moon.setVelocity(0, 44.89);

    planets.push_back(earth);
    planets.push_back(moon);

    bool run_thread = true;

    // launch the rendering and physics thread
    thread render = thread(&renderingThread, &window, &planets, &run_thread);
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
                int x = mouseButtonPressed->position.x;
                int y = mouseButtonPressed->position.y;

                Planet p = Planet::makePlanet(.1, 10, x, y);

                planets.push_back(p);
            }

        }

    }

    return EXIT_SUCCESS;
}