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

void Renderer::findTrajectory(Node<Matter> objectParam)
{
    // this is just a garbage solution. linked lists were a mistake
    Matter object = *objectParam.value;
    Node<Matter> * node = &matter;
    Matter other{};// = *matter.next->value;
    while (true)
    {
        if (node->value == &object)
        {
            continue;
        }
        Matter other = *node->value;
    }

    std::array<double, 2> barycenter = {(1/(object.mass*other.mass))*(object.mass*object.position[0] + other.mass*other.position[0]), (1/(object.mass*other.mass))*(object.mass*object.position[1] + other.mass*other.position[1])};
    std::array<double, 2> relativeDistance = {object.position[0]-other.position[0], object.position[1]-other.position[1]};
};

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
}
