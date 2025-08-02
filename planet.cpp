#include <iostream>

#include <SFML/Graphics.hpp>

#include "planet.h"

#define G 1
#define step 1.0e-3

using namespace std;

Planet Planet::makePlanet(double mass, double radius, double x, double y)
{
    Planet p;
    p.mass = mass;
    p.radius = radius;
    p.position = sf::Vector2<double>(x, y);
    p.visual = sf::CircleShape(radius);
    p.visual.setPosition(sf::Vector2f(x - radius, y - radius));

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

void Planet::setVelocity(double x, double y)
{
    this->velocity = sf::Vector2<double>(x, y);
}

void Planet::updateVisualPosition()
{
    this->visual.setPosition(sf::Vector2f(
        this->position.x - this->radius, this->position.y - this->radius
    ));
}

void Planet::addPlanetForce(Planet* p)
{
    // taken from newton
    sf::Vector2<double> r_21 = p->position - this->position;
    this->netForce += G * (this->mass * p->mass) / (r_21.lengthSquared()) * r_21.normalized();
}

void Planet::applyGravity()
{
    updateAcceleration();
    updateVelocity();
    updatePosition();
    this->netForce = sf::Vector2<double>(0, 0);
}

void Planet::updatePosition()
{
    this->position += this->velocity * step;
}

void Planet::updateVelocity()
{
    this->velocity += this->acceleration * step;
}

void Planet::updateAcceleration()
{
    // taken from newton
    this->acceleration = this->netForce / this->mass;
}


