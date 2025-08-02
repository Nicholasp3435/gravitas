#include <iostream>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>

#include "planet.h"

using namespace std;

void renderingThread(sf::RenderWindow* window, vector<Planet*>* planets, bool* run_thread)
{
    // activate the window's context
    window->setActive(true);

    // the rendering loop
    while (*run_thread)
    {
        // Clear screen
        window->clear();

        // draw planets
        for (Planet* p : *planets) 
        {
            p->updateVisualPosition();
            window->draw(p->getVisual());
        }

        // Update the window
        window->display();
    }

    for (Planet* p : *planets) delete p;

    window->close();
}

int main()
{
    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    window.setFramerateLimit(120);
        
    // deactivate its OpenGL context
    if (!window.setActive(false)) return EXIT_FAILURE;

    vector<Planet*> planets;

    Planet* p1 = new Planet(Planet::makePlanet(100, 10, 400, 300));
    Planet* p2 = new Planet(Planet::makePlanet(100, 10, 600, 300));

    p1->setVelocity(0, .5);
    p2->setVelocity(0, -.5);


    planets.push_back(p1);
    planets.push_back(p2);

    bool run_thread = true;

    // launch the rendering thread
    std::thread thread(&renderingThread, &window, &planets, &run_thread);

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
                thread.join();
            }
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) 
            {
                int x = mouseButtonPressed->position.x;
                int y = mouseButtonPressed->position.y;

                Planet* p = new Planet(Planet::makePlanet(1, 10, x, y));

                planets.push_back(p);
            }
        }

        // update positions and do other gravity stuff
        for (long unsigned int i = 0; i < planets.size(); i++)
        {
            for (long unsigned int j = 0; j < planets.size(); j++)
            {
                if (i == j) continue;

                planets[i]->addPlanetForce(planets[j]);
            }
            planets[i]->applyGravity();
        }
    }

    return EXIT_SUCCESS;
}