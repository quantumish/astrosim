//
//  rocket.hpp
//  AstroSim
//
//  Created by David Freifeld on 5/27/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#ifndef rocket_hpp
#define rocket_hpp

#include <stdio.h>
#include <cmath>
#include "matter.hpp"

class Rocket : Matter
{
public:
    double mass;
    double radius;
    Eigen::Vector2d position;
    Eigen::Vector2d screenPosition;
    Eigen::Vector2d velocity;
    Eigen::Vector2d acceleration;
    Eigen::Vector2d prevPosition;
    Force netForce{};
    std::vector<Eigen::Vector2d> history;
    Trajectory orbit;
    Eigen::Vector2d dimensions;
    double deltaM;
    double exhaustVelocity;

    Rocket(double massParam, Eigen::Vector2d dimensionsParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam, double deltaMParam, double exhaustVParam);
    void updatePosition();
};

#endif /* rocket_hpp */
