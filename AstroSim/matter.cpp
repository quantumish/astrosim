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
    warn = false;
    netForce.target = this;
}

Matter::Matter()
{
    mass = 10;
    radius = 1;
    position = {0,0};
    velocity = {0,0};
    netForce.target = this;
}


void Matter::updatePosition()
{
    warn = false;
    prevPosition = position;
    PLOG_ERROR_IF(mass < 0) << "Mass of planet is NEGATIVE. This doesn't really make sense. This will cause errors.";
    PLOG_ERROR_IF(radius < 0) << "Radius of planet is NEGATIVE. This isn't even explanable. This will cause errors.";

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
