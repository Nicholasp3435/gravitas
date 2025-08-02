#include <iostream>

#include <SFML/Graphics.hpp>

#include "planet.h"

using namespace std;

Planet Planet::makePlanet(int mass, int radius, double x, double y)
{
    Planet p;
    p.mass = mass;
    p.radius = radius;
    p.position = sf::Vector2<double>(x, y);
    p.visual = sf::CircleShape(radius);
    p.visual.setPosition(sf::Vector2f(x - radius, y - radius));

    cout << "New planet made at (" << x << ", " << y << ")" << endl;

    return p;
}

sf::Vector2<double> Planet::getPosition()
{
    return this->position;
}

sf::CircleShape Planet::getVisual()
{
    return this->visual;
}

void Planet::updateVisualPosition()
{
    this->visual.setPosition(sf::Vector2f(
        this->position.x - this->radius, this->position.y - this->radius
    ));
}

void Planet::updatePosition()
{
    this->position += this->velocity;
}

void Planet::setVelocity(sf::Vector2<double> velocity)
{
    this->velocity = velocity;
}
