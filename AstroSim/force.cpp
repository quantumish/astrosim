//
//  force.cpp
//  AstroSim
//
//  Created by David Freifeld on 4/18/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "force.hpp"

Force::Force(std::array<double, 2> componentsParam, Matter &targetParam, Matter &sourceParam)
{
    components = componentsParam;
    target = &targetParam;
    source = &sourceParam;
}

Force::Force()
{
    components = {0,0};
    target = NULL;
    source = NULL;
}

void Force::updateGravity()
{
    warn = false;
    err = false;
    double universalGravitationalConstant = 6.7408 * pow(10, -11);
    std::array<double, 2> distanceComponents = {source->position[0]-target->position[0], source->position[1]-target->position[1]};
    double distance = sqrt(pow(distanceComponents[0],2)+pow(distanceComponents[1],2));
    double magnitude = (source->mass * target->mass * universalGravitationalConstant)/pow(distance,2);
    PLOG_VERBOSE << "Calculating gravity: ( " << source->mass << " * " << target->mass << " * " << universalGravitationalConstant << " ) / " << pow(distance,2) << " = " << magnitude;
    double pi = 3.14159265359;
    double direction;
    direction = atan2(distanceComponents[1], distanceComponents[0]);
    if (isinf(magnitude) == false && isnan(magnitude) == false)
    {
        components[0] = cos(direction)*magnitude;
        components[1] = sin(direction)*magnitude;
    }
    else
    {
        PLOG_WARNING << "Force magnitude is infinity/NaN.";
        warn = true;
    }
}

void Force::applyForce()
{
    target->acceleration[0] = components[0]/target->mass;
    PLOG_VERBOSE << "Applying force: " << components[0] << " / " << target->mass << " = " << target->acceleration[0];
    target->acceleration[1] = components[1]/target->mass;
}
