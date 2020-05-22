//
//  renderer.cpp
//  AstroSim
//
//  Created by David Freifeld on 4/18/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "renderer.hpp"

Renderer::Renderer(int speedParam, sf::RenderWindow * windowParam, int pixelParam)
{
    speed=speedParam;
    window=windowParam;
    pixelLength=pixelParam;
    sf::Vector2u size = {window->getSize().x, window->getSize().y};
    canvas.create(size.x, size.y, sf::Color(0, 0, 0));
//    addMatter(pow(10,0), 1, {10000000000000, 0}, {0,0});
}

Eigen::Vector2d Renderer::fixPosition(Eigen::Vector2d coordinates)
{
    std::array<unsigned int, 2> fixed_coords;
    fixed_coords[0] = round(coordinates[0]);
    fixed_coords[1] = round(coordinates[1]);
    double pixelLength = pow(10, 2);
    fixed_coords[0] /= pixelLength;
    fixed_coords[1] /= pixelLength;
    return Eigen::Vector2d {fixed_coords[0], window->getSize().y-fixed_coords[1]};
}

void Renderer::addMatter(double massParam, double radiusParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam)
{
    matter.emplace_back(massParam, radiusParam, positionParam, velocityParam);
}

// O(n^2)
void Renderer::initializeForces()
{
    for (int i = 0; i < matter.size(); i++)
    {
        for (int j = 0; j < matter.size(); j++)
        {
            if (&forces[j] == &forces[i]) continue;
            std::array<double, 2> blank = {0,0}; // Current explanation for why this exists is type declaration problems.
            PLOG_DEBUG << "Initializing force between " << &matter[j] << "(with mass " << matter[j].mass << ") and " << &matter[i] << "(with mass " << matter[i].mass << ")";
            forces.emplace_back(blank, matter[j], matter[i]);
            forces[forces.size()-1].updateGravity();
            if (forces[forces.size()-1].warn == true) warnCount += 1;
            forces.emplace_back(blank, matter[i], matter[j]);
            forces[forces.size()-1].updateGravity();
            if (forces[forces.size()-1].warn == true) warnCount += 1;
            forces[forces.size()-1].updateGravity();
        }
    }
}

void Renderer::removeMatter(int index)
{
    for (int i = 0; i < forces.size(); i++)
    {
        if (forces[i].target == &matter[index] || forces[i].source == &matter[index])
        {
            forces.erase(forces.begin() + i);
        }
    }
    matter.erase(matter.begin()+index);
}

//void Renderer::diagnoseForces()
//{
//    std::cout << "------BEGIN DIAGNOSIS FOR SIZE " << forces.size() << "------\n";
//    for (int i = 0; i < matter.size(); i++)
//    {
//        std::cout << &matter[i] << "(" << matter[i].mass << "): \n";
//        std::cout << "\tFORCES IN: \n";
//        for (int j = 0; j < forces.size(); j++)
//        {
//            if (&matter[i] == forces[j].target)
//            {
//                std::cout << "\t\t" << forces[j].source << "(" << forces[j].source->mass << ") " << forces[j].target << "(" << forces[j].target->mass << ") " << forces[j].components[0] << " " << forces[j].components[1] << std::endl;
//            }
//        }
//        std::cout << "\tFORCES OUT: \n";
//        for (int j = 0; j < forces.size(); j++)
//        {
//            if (&matter[i] == forces[j].source)
//            {
//                std::cout << "\t\t" << forces[j].source << "(" << forces[j].source->mass << ") " << forces[j].target << "(" << forces[j].target->mass << ") " << forces[j].components[0] << " " << forces[j].components[1] << std::endl;
//            }
//        }
//    }
//    std::cout << "------END DIAGNOSIS------\n";
//}


void Renderer::findOrbit(Matter matter)
{
}

void Renderer::traceObjects()
{
    for (int i = 0; i<matter.size(); i++)
    {
        Eigen::Vector2d loc = fixPosition(matter[i].position);
        if (loc[0] < window->getSize().x && loc[1] < window->getSize().y)
        {
            canvas.setPixel(loc[0], loc[1], sf::Color(255,255,255,100));
        }
    }
}

// Strange naming convention is a byproduct of an attempt to roll elastic collision and inelastic collision into the same function
// as they're mainly the same type of calculations. You may wonder why they're even in a separate function: it allows me to separately
// work on collision detection and collisions so one can be functional while the other one isn't!
void Renderer::momentumCollision(Matter a, Matter b, int aIndex, bool elastic)
{
    if (elastic == false)
    {
        b.velocity = (a.mass * a.velocity + b.mass * b.velocity)/(a.mass + b.mass);
        b.mass = a.mass + b.mass;
        removeMatter(aIndex);
    }
    else
    {
        // crap this needs some physics that i don't wanna do rn
    }
}

std::vector<double> Renderer::checkCollisions(Eigen::Vector2d origin, Eigen::Vector2d direction, Eigen::Vector2d circleCenter, double circleRadius)
{
    double a = 1;//direction.dot(direction);
    double b = 2 * origin.dot(direction) - 2 * direction.dot(circleCenter);
    double c = origin.dot(origin) - pow(circleRadius,2) - (2 * origin.dot(circleCenter)) + circleCenter.dot(circleCenter);
    double discriminant = pow(b,2) - 4*a*c;
    std::cout << discriminant << " is discriminant\n";
    std::vector<double> points;
    if (discriminant >= 0)
    {
        if (discriminant == 0)
        {
            points.push_back(-b/(2*a));
        }
        else
        {
            points.push_back((-b + sqrt(discriminant))/(2*a));
            points.push_back((b + sqrt(discriminant))/(2*a));
        }
    }
    if (points.size() > 1)
    {
        if (points[0] > points[1])
        {
            double old = points[0];
            points[0] = points[1];
            points[1] = old;
        }
    }
    return points;
}

void Renderer::rayTrace(int rayCount)
{
    for (int i = 0; i < stars.size(); i++)
    {
        stars[i].rays = {};
        std::cout << stars[i].rays.size() << " INIT SIZE\n";
        // |P-C|^2 - R^2 = 0 or |O+tD-C|^2 - R^2 = 0 which can be simplified to
        for (int rayNum = 0; rayNum < rayCount; rayNum++)
        {
            double pi = 3.14159265359;
            Eigen::Vector2d origin = stars[i].position;
            Eigen::Vector2d direction = {cos(rayNum * ((2 * pi)/(rayCount-1))),sin(rayNum * ((2 * pi)/(rayCount-1)))};
            std::cout << rayNum * ((2 * pi)/(rayCount-1)) << " angle\n";
            Eigen::Vector2d endpoint;
            for (int j = 0; j < matter.size(); j++)
            {
                std::vector<double> points = checkCollisions(origin, direction, matter[j].position, matter[j].radius);
                double offscreen = 5 * pow(10,4);
                if (points.size() < 1)
                {
                    endpoint = origin + (direction * offscreen);
                    std::cout << origin[0] << ", " << origin[1] << " + (" << direction[0] << ", "<< direction[1] << ") * pow(10,4) = (" << endpoint[0] << ", " << endpoint[1] << "\n";
                }
                else
                {
                    if (points.size() > 1)
                    {
                        if (points[0] > points[1]) std::swap(points[0], points[1]);
                        endpoint = origin + (direction * points[0]);
                        if (points[0] < 0) {
                            points[0] = points[1];
                            endpoint = origin + (direction * points[0]);
                            if (points[0] < 0) endpoint = origin + (direction * offscreen);
                        }
                    }
                    else
                    {
                        if (points[0] < 0)
                        {
                            endpoint = origin + (direction * offscreen);
                        }
                        endpoint = origin + (direction * points[0]);
                    }
                
                    std::cout << origin[0] << ", " << origin[1] << " + (" << direction[0] << ", "<< direction[1] << ") * " << points[0] << " = (" << endpoint[0] << ", " << endpoint[1] << "\n";
                }
            }
            std::array<Eigen::Vector2d,2> path = {origin, endpoint};
            stars[i].rays.push_back(path);
        }
        for (int k = 0; k < stars[i].rays.size(); k++)
        {
            Eigen::Vector2d start = fixPosition(stars[i].rays[k][0]);
            Eigen::Vector2d end = fixPosition(stars[i].rays[k][1]);
            std::cout << "drawing (in func) " << start[0] << " " << start[1] << " and " << end[0] << " " << end[1] << "\n";
        }
    }
}

void Renderer::updateScene()
{
    for (int i = 0; i < matter.size(); i++)
    {
        matter[i].acceleration = {0,0};
    }
    for (int i = 0; i < forces.size(); i++)
    {
        forces[i].updateGravity();
        if (forces[i].warn == true)
        {
            warnCount++;
        }
        PLOG_VERBOSE << "Calling method to apply force (" << forces[i].components[0] << ", " << forces[i].components[1] << ") between " << forces[i].target << " (with mass " << forces[i].target->mass << ") " << forces[i].source << " (with mass " << forces[i].source->mass << ")";
        forces[i].target->netForce.components[0] += forces[i].components[0];
        forces[i].target->netForce.components[1] += forces[i].components[1];
    }
    for (int i = 0; i < matter.size(); i++)
    {
        matter[i].updatePosition();
        if (matter[i].warn == true)
        {
            warnCount++;
        }
    }
}

void Renderer::drawScene()
{
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromImage(canvas);
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    window->draw(backgroundSprite);
    for (Matter object : matter)
    {
        object.screenPosition = fixPosition(object.position);
        sf::CircleShape shape (object.radius);
        shape.setPosition(object.screenPosition[0]-object.radius, object.screenPosition[1]-object.radius);
        window->draw(shape);
    }
    for (Star star : stars)
    {
        star.screenPosition = fixPosition(star.position);
        sf::CircleShape shape (star.radius);
        shape.setPosition(star.screenPosition[0]-star.radius, star.screenPosition[1]-star.radius);
        window->draw(shape);
        std::cout << star.rays.size() << " SIZE\n";
        for (int i = 0; i <= star.rays.size(); i++)
        {
            std::cout << "should draw " << star.rays[i][0][0] << " " << star.rays[i][0][1] << " and " << star.rays[i][1][0] << " " << star.rays[i][1][1] << "\n";
            Eigen::Vector2d start = fixPosition(star.rays[i][0]);
            Eigen::Vector2d end = fixPosition(star.rays[i][1]);
            sf::Vertex line[] = {sf::Vertex(sf::Vector2f((float) start[0], (float) start[1])), sf::Vertex(sf::Vector2f((float) end[0], (float) end[1]))};
            std::cout << "drawing " << start[0] << " " << start[1] << " and " << end[0] << " " << end[1] << "\n";
            window->draw(line, 2, sf::Lines);
        }
    }
}


void Renderer::nextFrame()
{
    updateScene();
    rayTrace(13);
    //     diagnoseForces();
    traceObjects();
    drawScene();
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
