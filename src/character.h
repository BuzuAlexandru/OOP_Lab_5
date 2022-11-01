#pragma once
#include "entity.h"
#include "equipment.h"
#include <iostream>
using namespace std; 

class Character: public Entity
{
    public:

    float maxHealth,health;

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

    int coins, killCount, breachCount, knightDefeatCount;
    float TdefenseDmg, TenemyDmg;

    Player():Character()
    {
        killCount = 0;
        breachCount = 0;
        knightDefeatCount = 0; 
        TdefenseDmg = 0;
        TenemyDmg = 0;

        maxHealth = 0;
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
        dexterity=75;
        strength=1;
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

    void takeDmg(int dmg)
    {
        health -= dmg-dmg*armor/50;
    }

    int hit()
    {
        if(rand()%100<(dexterity+weapon.accuracy)/2)
            return strength + weapon.power * weapon.integrity / 100;
        else
            return 0;
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

    int x,y, fighting;

    bool inCombat, healing;

    Knight():Soldier()
    {
        x = -1;
        y = -1;
        strength = 3;
        inCombat = false;
        healing = false;
        fighting = -1;
    }

    void engage(int j)
    {
        fighting = j;
        inCombat = true;
    }

    void disengage()
    {
        fighting = -1;
        inCombat = false;
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
        weapon.range = 3;
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

    int x,y,mvSpeed, goldValue, dmgValue, fighting;
    bool inCombat, deployed;

    Enemy():Unit()
    {
        x=0;
        y=rand()%3+24;
        maxHealth = rand()%201 + 100;
        health = maxHealth;
        strength = 2;
        mvSpeed=1;
        goldValue=10;
        dmgValue=1;
        inCombat = false;
        deployed = false;
        fighting = -1;
    }

    void moveRight()
    {
        x += mvSpeed;
    }

    int getID()
    {
        return id;
    }

    void deploy()
    {
        deployed = true;
    }

    void engage(int j)
    {
        fighting = j;
        inCombat = true;
    }

    void disengage()
    {
        fighting = -1;
        inCombat = false;
    }
};