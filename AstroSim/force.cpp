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
    double universalGravitationalConstant = 6.7408 * pow(10, -11);
    std::array<double, 2> distanceComponents = {source->position[0]-target->position[0], source->position[1]-target->position[1]};
    double distance = sqrt(pow(distanceComponents[0],2)+pow(distanceComponents[1],2));
    double magnitude = (source->mass * target->mass * universalGravitationalConstant)/pow(distance,2);
    std::cout << "( " << source->mass << " * " << target->mass << " * " << universalGravitationalConstant << " ) / " << pow(distance,2) << " = " << magnitude << std::endl;
    double pi = 3.14159265359;
    double direction;
    if (distanceComponents[0] == 0)
    {
        if (distanceComponents[1] > 0)
        {
            direction = pi/2;
        }
        else
        {
            direction = (3*pi)/2;
        }
    }
    else if (distanceComponents[1] == 0)
    {
        if (distanceComponents[0] > 0)
        {
            direction = 0;
        }
        else
        {
            direction = pi;
        }
    }
    else
    {
        direction = atan(abs(distanceComponents[1]) / abs(distanceComponents[0]));
        if (distanceComponents[0] < 0 && distanceComponents[1] < 0)
        {
            direction += pi;
        }
        else if (distanceComponents[0] < 0 && distanceComponents[1] > 0)
        {
            direction += pi/2;
        }
        else if (distanceComponents[0] > 0 && distanceComponents[1] < 0)
        {
            direction -= pi/2;
        }
    }
    std::cout << "UPDATED GRAV FOR " << target->mass << " TO GET " << magnitude << " " << direction << " CUZ " << distanceComponents[0] << " and " << distanceComponents[1] << "\n";
    components[0] = cos(direction)*magnitude;
    components[1] = sin(direction)*magnitude;
}

void Force::applyForce()
{
    target->acceleration[0] += components[0]/target->mass;
    std::cout << "APPLY " << components[0] << " / " << target->mass << " = " << target->acceleration[0] << "\n";
    target->acceleration[1] += components[1]/target->mass;
}
