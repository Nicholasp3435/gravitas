#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "planet.h"

#define G 1.0
#define STEP 1.0e-3

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

bool Planet::isColliding(Planet* p)
{
    sf::Vector2<double> r_21 = p->position - this->position;
    if (r_21.length() <= p->radius + this->radius)
    {
        return true;
    }
    return false;
}

Planet* Planet::collide(Planet* p)
{
    sf::Vector2<double> mergedPosition = 0.5 * (this->position + p->position);
    double merged_radius = sqrt((this->radius * this->radius) + (p->radius * p->radius));
    double merged_mass = this->mass + p->mass;
    sf::Vector2<double> mergeVelocity = 1 / (merged_mass) * (this->mass * this->velocity + p->mass * p->velocity);

    Planet* merged = new Planet(Planet::makePlanet(
        merged_mass, merged_radius, mergedPosition.x, mergedPosition.y
    ));
    merged->velocity = mergeVelocity;
    return merged;
}

void Planet::addPlanetForce(Planet* p)
{
    // taken from newton
    sf::Vector2<double> r_21 = p->position - this->position;
    sf::Vector2<double> force = G * (this->mass * p->mass) / (r_21.lengthSquared()) * r_21.normalized();
    this->netForce += force;
    p->netForce -= force;

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
    this->position += this->velocity * STEP;
}

void Planet::updateVelocity()
{
    this->velocity += this->acceleration * STEP;
}

void Planet::updateAcceleration()
{
    // taken from newton
    this->acceleration = this->netForce / this->mass;
}


