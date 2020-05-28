//
//  rocket.cpp
//  AstroSim
//
//  Created by David Freifeld on 5/27/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "rocket.hpp"

Rocket::Rocket(double massParam, Eigen::Vector2d dimensionsParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam, double deltaMParam, double exhaustVParam)
{
    mass = massParam;
    dimensions = dimensionsParam;
    position = positionParam;
    velocity = velocityParam;
    deltaM = deltaMParam;
    exhaustVelocity = exhaustVParam;
    warn = false;
    netForce.target = this;
}

void Rocket::updatePosition()
{
    warn = false;

    PLOG_ERROR_IF(mass < 0) << "Mass of rocket is NEGATIVE. This will cause errors.";
    PLOG_ERROR_IF(dimensions[0] < 0 || dimensions[1] < 0) << "Length/width of rocket is NEGATIVE. This will cause errors.";
    
    if (netForce.target != this)
    {
        PLOG_WARNING << "Net force belonging to matter does not have matter as its target.";
        netForce.target = this;
        warn = true;
    }
    netForce.applyForce();
    double deltaV = exhaustVelocity * log((mass+deltaM)/mass);
    double direction = atan2(velocity[1], velocity[0]);
    Eigen::Vector2d deltaVComponents = {cos(direction) * deltaV, sin(direction) * deltaV};
    velocity[0] += deltaVComponents[0];
    velocity[1] += deltaVComponents[1];
    velocity[0] += acceleration[0];
    velocity[1] += acceleration[1];
    position[0] += velocity[0];
    position[1] += velocity[1];
    netForce.components = {0,0};
}
