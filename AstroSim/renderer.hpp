//
//  renderer.hpp
//  AstroSim
//
//  Created by David Freifeld on 4/18/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#ifndef renderer_hpp
#define renderer_hpp

#include <stdio.h>
#include <vector>
#include <array>
#include <cmath>
#include <typeinfo>
#include <iostream>
#include <valarray>

#include "matter.hpp"
#include "force.hpp"
#include "linked_list.cpp"


class Renderer
{
public:
    int speed;
    Node<Matter> matter{};
    Node<Force> forces{};
    
    Renderer(int speedParam);
    void addMatter(Node<Matter> * node);
    void findTrajectory(Matter matter);
    void checkCollisions();
    void updateScene();
};

#endif /* renderer_hpp */
