using namespace std;

#ifndef PLANET_H
#define PLANET_H

#include <SFML/System/Vector2.hpp>

/**
 * Class: Planet
 * ----------------
 * Represents a Planet object
 */
class Planet
{
    public:
        void makePlanet();

    private:
        int mass;
        int radius;
        double netForce;
        sf::Vector2<double> position;
        sf::Vector2<double> velocity;
        sf::Vector2<double> acceleration;
};

#endif /* PLANET_H */