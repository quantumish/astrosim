//
//  matter.hpp
//  AstroSim
//
//  Created by David Freifeld on 4/18/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#ifndef matter_hpp
#define matter_hpp

#include <stdio.h>
#include <array>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include "force.hpp"

struct Trajectory {
    double A;
    double B;
    double C;
    double D;
    double E;
    double F;
};

class Matter
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
    std::vector<std::array<double, 2>> history;
    Trajectory orbit;
    
    sf::CircleShape shape;
    
    Matter(double massParam, double radiusParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam);
    Matter(const Matter &src);
    void updatePosition();
    Matter();
};


#endif /* matter_hpp */
