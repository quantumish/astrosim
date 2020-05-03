//
//  linked_list.cpp
//  AstroSim
//
//  Created by David Freifeld on 5/2/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#include "linked_list.hpp"

Node::Node()
{
    prev = NULL;
    next = NULL;
    value = NULL;
}

Node::Node(Matter valueParam, Node* prevParam, Node* nextParam)
{
    value = &valueParam;
    prev = prevParam;
    next = nextParam;
}
