#ifndef PLANET_H
#define PLANET_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

/**
 * Class: Planet
 * ----------------
 * Represents a Planet object
 */
class Planet
{
    public:
        static Planet makePlanet(double mass, double radius, double x, double y);
        sf::Vector2<double> getPosition();
        sf::CircleShape getVisual();
        void setVelocity(double x, double y);
        void updateVisualPosition();
        void addPlanetForce(Planet* p);
        void applyGravity();

    private:
        double mass;
        int radius;
        sf::Vector2<double> netForce = sf::Vector2<double>(0, 0);
        sf::Vector2<double> position;
        sf::Vector2<double> velocity = sf::Vector2<double>(0, 0);
        sf::Vector2<double> acceleration = sf::Vector2<double>(0, 0);
        void updatePosition();
        void updateVelocity();
        void updateAcceleration();

        sf::CircleShape visual;
};

#endif /* PLANET_H */