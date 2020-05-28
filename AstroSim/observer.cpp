//
//  observer.cpp
//  AstroSim
//
//  Created by David Freifeld on 5/24/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "observer.hpp"

Observer::Observer(double xBegin, Matter planet, double attribute)
{
    x.push_back(xBegin);
    matterTarget = &planet;
    observedAttribute = &attribute;
}

Observer::Observer(double xBegin, Eigen::Vector2d positionParam, double angle)
{
    x.push_back(xBegin);
    position = positionParam;
    coneAngle = angle;
}

void Observer::observePlanet()
{
    y.push_back(*observedAttribute);
    if (x.size()>1)
    {
        x.push_back(x[x.size()-1]+1);
    }
}
