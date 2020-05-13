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
#include <array>
#include <cmath>
#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>

#include "matter.hpp"
#include "force.hpp"

class Renderer
{
public:
    int speed;
    
    std::vector<Matter> matter;
    std::vector<Force> forces;
    
    Renderer(int speedParam);
    std::array<double, 2> fixPosition(std::array<double, 2> coordinates, sf::RenderWindow &window);
    void addMatter(double massParam, double radiusParam, std::array<double, 2> positionParam, std::array<double, 2> velocityParam);
    void findTrajectory(Matter matter);
//    void traceObjects(sf::RenderWindow window);
    void checkCollisions();
    void updateScene();
};

#endif /* renderer_hpp */
