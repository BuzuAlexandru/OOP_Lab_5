#pragma once
#include "entity.h"
#include "equipment.h"
#include <iostream>
using namespace std; 

class Character: public Entity
{
    public:

    int maxHealth,health;

    Character():Entity()
    {
        maxHealth = 100;
        health = maxHealth;
    }

    void takeDmg(int dmg)
    {
        health -= dmg;
    }
};

class Player: public Character
{
    public:

    int coins;

    Player():Character()
    {
        maxHealth = 10;
        health = maxHealth;
        coins = 100;
    }

    void coinSpend(int cost)
    {
        coins -= cost;
    }

    void coinGet(int gold)
    {
        coins += gold;
    }

    int getID()
    {
        return id;
    }
};

class Worker: public Character
{
    public:

    int efficiency;
    Tool pickaxe;

    Worker():Character()
    {
        efficiency = 50;
        pickaxe = Tool();
    }

    int getID()
    {
        return id;
    }

    void getHeal(int heal)
    {
        health += heal;
    }
};

class Unit: public Character
{
    public:

    int armor, dexterity, strength;
    Weapon weapon;

    Unit():Character()
    {
        armor=1;
        dexterity=1;
        strength=rand()%1+1;
        weapon = Weapon();
    }

    bool isAlive()
    {
        if(health>0)
            return true;
        else    
            return false;
    }

    void kill()
    {
        health = 0;
    }

    int hit()
    {
        return (int)(strength + weapon.power * weapon.integrity / 100);
    }
};

class Soldier: public Unit
{
    public:

    Soldier():Unit()
    {}

    void getHeal(int heal)
    {
        health += heal;
    }
};

class Knight: public Soldier
{
    public:

    int x,y;

    bool inCombat, healing;

    Knight():Soldier()
    {
        x = -1;
        y = -1;
        strength = 3;
        inCombat = false;
        healing = false;
    }

    int getID()
    {
        return id;
    }
};

class Archer: public Soldier
{
    public:

    bool hasShot;

    Archer():Soldier()
    {
        weapon.range = rand()%5+3;
        hasShot = false;
    }

    int getID()
    {
        return id;
    }
};

class Enemy: public Unit
{
    public:

    int x,y,mvSpeed, goldValue, dmgValue;
    bool inCombat;

    Enemy():Unit()
    {
        x=0;
        y=0;
        maxHealth = rand()%201 + 100;
        health = maxHealth;
        strength = 2;
        mvSpeed=1;
        goldValue=10;
        dmgValue=1;
        inCombat = false;
    }

    void moveUp()
    {
        y -= mvSpeed;
    }

    void moveDown()
    {
        y += mvSpeed;
    }

    void moveRight()
    {
        x += mvSpeed;
    }

    void moveLeft()
    {
        x -= mvSpeed;
    }

    int getID()
    {
        return id;
    }
};