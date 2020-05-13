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
}

std::array<double, 2> Renderer::fixPosition(std::array<double, 2> coordinates)
{
    double pixelLength = pow(10, 2);
    coordinates[0] /= pixelLength;
    coordinates[1] /= pixelLength;
    return std::array<double, 2> {coordinates[0], window->getSize().y-coordinates[1]};
}

void Renderer::addMatter(double massParam, double radiusParam, std::array<double, 2> positionParam, std::array<double, 2> velocityParam)
{
    matter.emplace_back(massParam, radiusParam, positionParam, velocityParam);
    for (int i = 0; i < matter.size()-1; i++)
    {
        std::array<double, 2> blank = {0,0}; // for some reason it only works if i initialize it first... type issues?
        forces.emplace_back(blank, matter[i], matter[matter.size()-1]);
        forces[forces.size()-1].updateGravity();
        forces.emplace_back(blank, matter[matter.size()-1], matter[i]);
        forces[forces.size()-1].updateGravity();
    }
}

void Renderer::findTrajectory(Matter matter)
{
    if (matter.history.size() >= 5)
    {
        Eigen::MatrixXd points(6,6);
        Eigen::MatrixXd answers(6,1);
        for (int i = 0; i < 6; i++)
        {
            std::array<double,2> pos = matter.history[i];
            points.row(i) << pow(pos[0],2), pos[0]*pos[1], pow(pos[1],2), pos[0], pos[1], 1;
            answers.row(i) << pos[1];
        }
        std::cout << "COEFFICIENTS\n";
        std::cout << points << "\n";
        std::cout << "ANSWERS\n";
        std::cout << answers << "\n";
        Eigen::MatrixXd unknowns (6,1);
        unknowns = points.inverse() * answers;
        std::cout << "UNKNOWNS\n";
        std::cout << unknowns << "\n";
    }
}

void Renderer::traceObjects()
{
    sf::Vector2u size = {window->getSize().x, window->getSize().y};
    sf::Image canvas;
    canvas.create(size.x, size.y, sf::Color(0, 0, 0));
    for (int i = 0; i<matter.size(); i++)
    {
        int x = round(matter[i].position[0])/pixelLength;
        int y = round(matter[i].position[1])/pixelLength;
        std::cout << x << " " << y << "\n";
        canvas.setPixel(x, y, sf::Color(255, 255, 255));
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
        forces[i].applyForce();
        forces[i].target->netForce.components[0] += forces[i].components[0];
        forces[i].target->netForce.components[1] += forces[i].components[1];
    }
    for (int i = 0; i < matter.size(); i++)
    {
        matter[i].updatePosition();
    }
}

void Renderer::drawScene()
{
    for (Matter object : matter)
    {
        std::cout << object.position[0] << std::endl;
        std::array<double, 2> fixedPosition = fixPosition(object.position);
        sf::CircleShape shape (object.radius);
        shape.setPosition(fixedPosition[0], fixedPosition[1]);
        window->draw(shape);
    }
}

void Renderer::nextFrame()
{
    updateScene();
    drawScene();
}
