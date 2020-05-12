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
#include "force.hpp"

struct Trajectory {
    double semiMajorAxis;
    double semiMinorAxis;
    float r;
    float rPrime;
};

class Matter
{
public:
    double mass;
    double radius;
    std::array<double, 2> position;
    std::array<double, 2> velocity;
    std::array<double, 2> acceleration;
    std::vector<Force> forces;
    Force netForce{};
    Trajectory orbit;
    
    sf::CircleShape shape;
    
    Matter(double massParam, double radiusParam, std::array<double, 2> positionParam, std::array<double, 2> velocityParam);
    void updatePosition();
    Matter();
};


#endif /* matter_hpp */
