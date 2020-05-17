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
    sf::RenderWindow * window;
    
    sf::Image canvas;
    
    int speed;
    int pixelLength;
    
    int warnCount = 0;
    int errCount = 0;
    
    std::vector<Matter> matter;
    std::vector<Force> forces;
    
    Renderer(int speedParam, sf::RenderWindow * windowParam, int pixelParam);
    std::array<unsigned int, 2> fixPosition(std::array<double, 2> coordinates);
    void addMatter(double massParam, double radiusParam, std::array<double, 2> positionParam, std::array<double, 2> velocityParam);
    void removeMatter(int index);
    
    void findTrajectory(Matter matter);
    void checkCollisions();
    void traceObjects();
    void updateScene();
    void drawScene();
    
    void nextFrame();
};

#endif /* renderer_hpp */
