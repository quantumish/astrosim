//
//  linked_list.hpp
//  AstroSim
//
//  Created by David Freifeld on 5/2/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

#ifndef linked_list_hpp
#define linked_list_hpp

#include <stdio.h>
#include <iostream>
#include "matter.hpp"

#endif /* linked_list_hpp */

class Node
{
public:
    Node * prev;
    Node * next;
    Matter * value;
    Node();
    Node(Matter valueParam, Node* prevParam, Node* nextParam);
    Node(const Node &p2) {prev = p2.prev; next = p2.next; value = p2.value;} 
    void addMatter(Matter matter, Node head);
    void printList();
};
