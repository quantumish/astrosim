//
//  renderer.cpp
//  AstroSim
//
//  Created by David Freifeld on 4/18/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "renderer.hpp"

Renderer::Renderer(int speedParam)
{
    speed=speedParam;
}

std::array<double, 2> fixPosition(std::array<double, 2> coordinates, sf::RenderWindow &window)
{
    double pixelLength = pow(10, 2);
    coordinates[0] /= pixelLength;
    coordinates[1] /= pixelLength;
    return std::array<double, 2> {coordinates[0], window.getSize().y-coordinates[1]};
}

void Renderer::addMatter(Node<Matter> * node)
{
    matter.addToLinkedList(node);

    Node<Matter> * current = &matter;
    while (true)
    {
        if (current->value == node->value)
        {
            if (current->next == NULL)
            {
                break;
            }
            current = current->next;
            continue;
        }
        Force * gravitational_force1 = new Force({0,0}, *(current->value), *(node->value));
        gravitational_force1->updateGravity();
        Node<Force> * force1 = new Node<Force> (*gravitational_force1, NULL, NULL);
        forces.addToLinkedList(force1);
        
        Force * gravitational_force2 = new Force({0,0}, *(node->value), *(current->value));
        gravitational_force2->updateGravity();
        Node<Force> * force2 = new Node<Force> (*gravitational_force2, NULL, NULL);
        forces.addToLinkedList(force2);
        
        if (current->next == NULL)
        {
            break;
        }
        current = current->next;
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

void Renderer::traceObjects(sf::RenderWindow window)
{
//    sf::Vector2u size = {window.getSize().x, window.getSize().y};
//    sf::Image canvas;
//    canvas.create(size.x, size.y, sf::Color(0, 0, 0));
    Node<Matter> * node = &matter;
    while (true)
    {
        int x = round(node->value->position[0])/pixelLength;
        int y = round(node->value->position[1])/pixelLength;
        std::cout << x << " " << y << "\n";
        canvas.setPixel(x, y, sf::Color(255, 255, 255));
        if (node->next != NULL)
        {
            node = node->next;
        }
        else
        {
            break;
        }
    }
}

void Renderer::updateScene()
{
    Node<Matter> * object = &matter;
    while (true)
    {
        object->value->acceleration = {0,0};
        if (object->next == NULL)
        {
            break;
        }
        object = object->next;
    }
    Node<Force> * force = &forces;
    while (true)
    {
        force->value->updateGravity();
        force->value->applyForce();
        force->value->target->netForce.components[0] += force->value->components[0];
        force->value->target->netForce.components[1] += force->value->components[1];
//        force->value->target->acceleration[1] = force->value->components[1]/force->value->target->mass;
        if (force->next == NULL)
        {
            break;
        }
        force = force->next;
    }
    object = &matter;
    while (true)
    {
        object->value->updatePosition();
        if (object->next == NULL)
        {
            break;
        }
        object = object->next;
    }
//    findTrajectory(*matter.value);
}
