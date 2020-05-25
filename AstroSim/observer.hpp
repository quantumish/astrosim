//
//  observer.hpp
//  AstroSim
//
//  Created by David Freifeld on 5/24/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#ifndef observer_hpp
#define observer_hpp

#include <stdio.h>
#include <vector>
#include <Eigen/Dense>

#include "matter.hpp"

class Observer
{
public:
    Matter * matterTarget;
    double * observedAttribute;
    double coneAngle;
    Eigen::Vector2d position;
    std::vector<double> x;
    std::vector<double> y;
    Observer(double xBegin, Matter planet, double attribute);
    Observer(double xBegin, Eigen::Vector2d position, double angle);
    void observePlanet();
    void collectLight();
};

#endif /* observer_hpp */

