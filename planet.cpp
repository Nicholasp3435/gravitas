#include <iostream>

#include "planet.h"

using namespace std;

void Planet::makePlanet()
{
    cout << "New planet" << endl;
}

int Planet::getMass()
{
    return this->mass;
}

void Planet::setMass(int mass)
{
    this->mass = mass;
}

int Planet::getRadius()
{
    return this->radius;
}

void Planet::setRadius(int radius)
{
    this->radius = radius;
}

sf::Vector2<double> Planet::getPosition()
{
    return this->position;
}

void Planet::setPosition(double x, double y)
{
    this->position = sf::Vector2<double>(x, y);
}

sf::Vector2<double> Planet::getVelocity()
{
    return this->velocity;
}

void Planet::setVelocity(double x, double y)
{
    this->velocity = sf::Vector2<double>(x, y);
}

sf::Vector2<double> Planet::getAcceleration()
{
    return this->acceleration;
}

void Planet::setAcceleration(double x, double y)
{
    this->acceleration = sf::Vector2<double>(x, y);
}
