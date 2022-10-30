#pragma once
#include <iostream>
using namespace std; 

class Entity
{
    public:

    int id;

    virtual int getID() = 0;

    Entity()
    {
        id = 0;
    }
    
    void setID(int i)
    {
        id = i;
    }
};