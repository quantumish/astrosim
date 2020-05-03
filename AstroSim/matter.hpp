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
    
    sf::CircleShape shape;
    
    Matter(double massParam, double radiusParam, std::array<double, 2> positionParam, std::array<double, 2> velocityParam);
    void updatePosition();
};

#endif /* matter_hpp */
