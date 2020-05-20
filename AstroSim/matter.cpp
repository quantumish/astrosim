//
//  matter.cpp
//  AstroSim
//
//  Created by David Freifeld on 4/18/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "matter.hpp"
#include "force.hpp"

Matter::Matter(double massParam, double radiusParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam)
{
    mass = massParam;
    radius = radiusParam;
    position = positionParam;
    velocity = velocityParam;
    shape.setRadius(radius);
    warn = false;
//    shape.setFillColor(sf::Color::Red);
    netForce.target = this;
}

Matter::Matter()
{
    mass = 10;
    radius = 1;
    position = {0,0};
    velocity = {0,0};
    shape.setRadius(radius);
    netForce.target = this;
}

Matter::Matter(const Matter &src)
{
    mass = src.mass;
    radius = src.radius;
    position = src.position;
    velocity = src.velocity;
    acceleration = src.acceleration;
    netForce = src.netForce;
    history = src.history;
    orbit = src.orbit;
    shape = *(new sf::CircleShape (radius));
}

void Matter::updatePosition()
{
    warn = false;
    prevPosition = position;
    if (netForce.target != this)
    {
        PLOG_WARNING << "Net force belonging to matter does not have matter as its target.";
        netForce.target = this;
        warn = true;
    }
    netForce.applyForce();
    velocity[0] += acceleration[0];
    velocity[1] += acceleration[1];
    position[0] += velocity[0];
    position[1] += velocity[1];
    netForce.components = {0,0};
}
