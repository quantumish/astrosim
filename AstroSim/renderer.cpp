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

void Renderer::listObjects()
{
    Node * current = &matter;
    while (true)
    {
        std::cout << current->value->mass << " ";
        if (current->next == NULL)
        {
            break;
        }
        current = current->next;
    }
}

void Renderer::addMatter(Node node)
{
    if (matter.value == NULL)
    {
        matter.value = node.value;
    }
    else
    {
        Node * current = &matter;
        while (current->next != NULL)
        {
            current = current->next;
        }
        node.prev = current;
        current->next = &node;
//        Node newNode(matterObject, current, NULL);
//        current->next = &newNode;
    }
//    std::cout << matter.value;
//    listObjects();
//    matter.push_back(matterObject);
//    for (int i=0; i<matter.size()-1; ++i)
//    {
//        Matter m = matter[i];
//        Force gravitational_force1({0,0}, m, matter[matter.size()-1]);
//        std::cout << "MASSES 1 AFTER CREATION: " << gravitational_force1.target->mass << " " << gravitational_force1.source->mass << std::endl;
//        gravitational_force1.updateGravity();
//        std::cout << "MASSES 1 AFTER INITIAL UPDATE: " << gravitational_force1.target->mass << " " << gravitational_force1.source->mass << std::endl;
//        forces.push_back(gravitational_force1);
//        std::cout << "MASSES 1 PUSHED TO LIST: " << forces[forces.size()-1].target->mass << " " << forces[forces.size()-1].source->mass << std::endl;
//
//        Force gravitational_force2({0,0}, matter[matter.size()-1], m);
//        gravitational_force2.updateGravity();
//        forces.push_back(gravitational_force2);
////        std::cout << forces.size() << std::endl;
//    }
}

void Renderer::updateScene()
{
////    std::cout << "MASSES BEFORE SCENE UPDATE: " << forces[0].target->mass << " " << forces[0].source->mass << std::endl;
//    for (int i=0; i<forces.size(); ++i)
//    {
//        Force force = forces[i];
//        std::cout << "MASSES INSIDE SCENE UPDATE: " << force.target->mass << " " << force.source->mass << std::endl;
//        force.updateGravity();
//        force.target->acceleration[0] = force.components[0]/force.target->mass;
//        force.target->acceleration[1] = force.components[1]/force.target->mass;
//        std::cout << "ACCEL " << force.target->acceleration[0] << std::endl;
//    }
//    for (int i=0; i<matter.size(); ++i)
//    {
//    }
}
