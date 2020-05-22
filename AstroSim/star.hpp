//
//  star.hpp
//  AstroSim
//
//  Created by David Freifeld on 5/20/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#ifndef star_hpp
#define star_hpp

#include <stdio.h>

#include "matter.hpp"

class Star : Matter
{
public:
    double mass;
    double radius;
    Eigen::Vector2d position;
    Eigen::Vector2d screenPosition;
    Eigen::Vector2d velocity;
    Eigen::Vector2d acceleration;
    Eigen::Vector2d previousPosition;
    Force netForce{};
    Eigen::Vector2d prevPosition;
    std::vector<Eigen::Vector2d> history;
    Trajectory orbit;
    
    std::vector<std::array<Eigen::Vector2d,2>> rays;
    double luminosity;
    
    Star(double massParam, double radiusParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam);
};

#endif /* star_hpp */