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

void Renderer::addStar(double massParam, Eigen::Vector2d positionParam, Eigen::Vector2d velocityParam, double radiusParam, double luminosityParam)
{
    matter.emplace_back(massParam, radiusParam, positionParam, velocityParam);
    stars.emplace_back(massParam, positionParam, velocityParam, radiusParam, luminosityParam);
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
    Eigen::Vector2d L = (origin-circleCenter);
    double a = direction.dot(direction);
    double b = 2 * direction.dot(L); //origin.dot(direction) - 2 * direction.dot(circleCenter);
    double c = L.dot(L) - pow(circleRadius*pixelLength,2); //origin.dot(origin) - pow(circleRadius,2) - (2 * origin.dot(circleCenter)) + circleCenter.dot(circleCenter);
    double discriminant = pow(b,2) - 4*a*c;
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
    return points;
}

void Renderer::rayTrace(int rayCount)
{
    for (int i = 0; i < stars.size(); i++)
    {
        stars[i].rays = {};
        // |P-C|^2 - R^2 = 0 or |O+tD-C|^2 - R^2 = 0 which can be simplified to
        for (int rayNum = 0; rayNum < rayCount; rayNum++)
        {
            double pi = 3.14159265359;
            Eigen::Vector2d origin = stars[i].position;
            double angle = rayNum * ((2 * pi)/(rayCount-1));
            Eigen::Vector2d direction = {cos(angle),sin(angle)};
            Eigen::Vector2d endpoint;
            for (int j = 0; j < matter.size(); j++)
            {
                std::vector<double> points = checkCollisions(origin, direction, matter[j].position, matter[j].radius);
                double offscreen = 5 * pow(10,4);
                endpoint = origin + (direction * offscreen);
                if (points.size() >= 1 && matter[j].mass != stars[i].mass)
                {
                    endpoint = origin + (direction * points[0]);
                    if (points[0] < 0)
                    {
                        endpoint = origin + (direction * offscreen);
                    }
                }
                if (j > 0)
                {
                    double newMagnitudeSq = pow(endpoint[0]-origin[0],2)+pow(endpoint[1]-origin[1],2);
                    double currentMagnitudeSq = pow(stars[i].rays[rayNum][0]-stars[i].position[0],2)+pow(stars[i].rays[rayNum][1]-stars[i].position[1],2);
                    if (newMagnitudeSq < currentMagnitudeSq)
                    {
                        stars[i].rays[rayNum] = endpoint;
                    }
                }
                else
                {
                    stars[i].rays.push_back(endpoint);
                }
            }
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
        shape.setFillColor(sf::Color(135, 133, 132));
        window->draw(shape);
    }
    for (Star star : stars)
    {
        star.screenPosition = fixPosition(star.position);
        sf::CircleShape shape (star.radius);
        shape.setPosition(star.screenPosition[0]-star.radius, star.screenPosition[1]-star.radius);
        window->draw(shape);
    }
}


void Renderer::nextFrame()
{
    updateScene();
    rayTrace(1001);
    //traceObjects();
    drawScene();
    std::this_thread::sleep_for(std::chrono::milliseconds(speed));
}
