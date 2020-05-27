//
//  star.cpp
//  AstroSim
//
//  Created by David Freifeld on 5/20/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "star.hpp"

Star::Star(double massParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam, double radiusParam, double luminosityParam)
{
    mass = massParam;
    double solarMass = 1.989 * pow(10,30);
    if (radiusParam == 0)
    {
        radius = pow((mass/solarMass), 0.78) * (6.957 * pow(10,5));
    }
    else
    {
        radius = radiusParam;
    }
    position = positionParam;
    velocity = velocityParam;
    warn = false;
    //    shape.setFillColor(sf::Color::Red);
    netForce.target = this;
    if (luminosity == 0)
    {
        if (mass < 0.43*solarMass)
        {
            luminosity = 0.23 * pow((mass/solarMass),2.3) * (3.828 * pow(10,26));
        }
        if (0.43*solarMass <= mass && mass < 2*solarMass)
        {
            luminosity = pow((mass/solarMass),4) * (3.828 * pow(10,26));
        }
        if (2*solarMass <= mass && mass < 55*solarMass)
        {
            luminosity = 1.4 * pow((mass/solarMass),4) * (3.828 * pow(10,26));
        }
        if (55*solarMass <= mass )
        {
            luminosity = 32000 * (mass/solarMass) * (3.828 * pow(10,26));
        }
    }
    else
    {
        luminosity = luminosityParam;
    }
}
