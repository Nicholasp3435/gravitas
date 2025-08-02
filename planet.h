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
        static Planet makePlanet(int mass, int radius, double x, double y);
        sf::Vector2<double> getPosition();
        sf::CircleShape getVisual();
        void updateVisualPosition();
        void updatePosition();
        void setVelocity(sf::Vector2<double> velocity);

    private:
        int mass;
        int radius;
        sf::Vector2<double> netForce = sf::Vector2<double>(0, 0);
        sf::Vector2<double> position;
        sf::Vector2<double> velocity = sf::Vector2<double>(0, 0);
        sf::Vector2<double> acceleration = sf::Vector2<double>(0, 0);
        
        sf::CircleShape visual;
};

#endif /* PLANET_H */