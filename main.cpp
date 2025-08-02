#include <iostream>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>

#include "planet.h"

using namespace std;

void renderingThread(sf::RenderWindow* window, vector<Planet>* planets)
{
    // activate the window's context
    window->setActive(true);

    // the rendering loop
    while (window->isOpen())
    {
        // Clear screen
        window->clear();

        // draw planets
        for (Planet p : *planets) 
        {
            p.updateVisualPosition();
            window->draw(p.getVisual());
        }

        // Update the window
        window->display();
    }
}

int main()
{
    // create the window (remember: it's safer to create it in the main thread due to OS limitations)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    window.setFramerateLimit(60);
        
    // deactivate its OpenGL context
    if (!window.setActive(false)) return EXIT_FAILURE;

    vector<Planet> planets;

    // launch the rendering thread
    std::thread thread(&renderingThread, &window, &planets);

    // the event/logic/whatever loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) 
            {
                int x = mouseButtonPressed->position.x;
                int y = mouseButtonPressed->position.y;

                Planet p = Planet::makePlanet(10, 10, x, y);
                p.setVelocity(sf::Vector2<double>(0, 0.001));

                planets.push_back(p);
            }
        }

        // update positions and do other gravity stuff
        // the & makes sure p is a referance
        for (Planet& p : planets) 
        {
            p.updatePosition();
        }
    }

    thread.join();

    return EXIT_SUCCESS;
}