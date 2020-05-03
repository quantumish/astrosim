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

#include "matter.hpp"
#include "force.hpp"
#include "linked_list.hpp"

class Renderer
{
public:
    int speed;
    Node matter{};
    Node forces{};
//    Matter * matter = malloc(10*sizeof(Matter));
//    Force * force = malloc(90*sizeof(Matter));
//    Matter * matter = (Matter*)malloc(10*sizeof(Matter));
    //std::vector<Matter> matter = new(10*sizeof(Matter));
    
    //std::vector<Force> forces;
    
    Renderer(int speedParam);
    void listObjects();
    void addMatter(Node node);
    void checkCollisions();
    void updateScene();
};

#endif /* renderer_hpp */
