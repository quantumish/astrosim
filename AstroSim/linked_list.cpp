//
//  linked_list.cpp
//  AstroSim
//
//  Created by David Freifeld on 5/2/20.
//  Copyright © 2020 David Freifeld. All rights reserved.
//

//#include "linked_list.hpp"
#include <stdio.h>
#include <iostream>

template <class Type>

class Node
{
public:
    Node<Type> * prev;
    Node<Type> * next;
    Type * value;
    

    Node()
    {
        prev = NULL;
        next = NULL;
        value = NULL;
    }

    Node(Type &valueParam, Node<Type>* prevParam, Node* nextParam)
    {
        value = &valueParam;
        prev = prevParam;
        next = nextParam;
    }

    void addToLinkedList(Node * addition)
    {
        if (value == NULL)
        {
            value = addition->value;
        }
        else
        {
            if (next != NULL)
            {
                next->addToLinkedList(addition);
            }
            else
            {
                addition->prev = this;
                next = addition;
            }
        }
        listObjects();
    }

    void listObjects()
    {
        Node * current = this;
        std::cout << " io;fzjoajks ";
        while (true)
        {
            std::cout << current->value << " ";
            if (current->next == NULL)
            {
                break;
            }
            current = current->next;
        }
        std::cout << "\n";
    }
};
