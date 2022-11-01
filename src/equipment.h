#pragma once
#include "entity.h"
#include <iostream>
using namespace std; 

class Equipment: public Entity
{
    public:

    float integrity;

    Equipment():Entity()
    {
        integrity = 100;
    }

    void getDull()
    {
        integrity -= 0.5;
    }

    void repair()
    {
        integrity = 100;
    }
};

class Weapon: public Equipment
{
    public:

    int power, range, accuracy;

    Weapon():Equipment()
    {
        power=1;
        range=0;
        accuracy=75;
    }

    int getID()
    {
        return id;
    }
};

class Tool: public Equipment
{
    public:

    int efficiency;
    Tool():Equipment()
    {
        efficiency=50;
    }

    int getID()
    {
        return id;
    }
};