//
//  star.cpp
//  AstroSim
//
//  Created by David Freifeld on 5/20/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "star.hpp"

Star::Star(double massParam, double radiusParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam)
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
