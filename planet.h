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

        int getMass();
        void setMass(int mass);

        int getRadius();
        void setRadius(int radius);
        
        sf::Vector2<double> getPosition();
        void setPosition(double x, double y);
        
        sf::Vector2<double> getVelocity();
        void setVelocity(double x, double y);
        
        sf::Vector2<double> getAcceleration();
        void setAcceleration(double x, double y);

    private:
        int mass;
        int radius;
        sf::Vector2<double> position;
        sf::Vector2<double> velocity;
        sf::Vector2<double> acceleration;
};

#endif /* PLANET_H */