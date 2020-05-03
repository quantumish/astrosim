//
//  force.hpp
//  AstroSim
//
//  Created by David Freifeld on 4/18/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#ifndef force_hpp
#define force_hpp

#include <stdio.h>
#include <array>
#include <iostream>

class Matter;

struct Force
{
public:
    std::array<double, 2> components;
    Matter * source;
    Matter * target;
    
    Force(std::array<double, 2> componentsParam, Matter &targetParam, Matter &sourceParam);
    Force();
    void updateGravity();
};

#include "matter.hpp"
#endif /* force_hpp */
