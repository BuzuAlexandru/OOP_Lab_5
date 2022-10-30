#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;

#include"src/equipment.h"
#include"src/character.h"
#include"src/environment.h"

Map map=Map();

Player player=Player();
int n = rand()%5 + 10;
Enemy* enemy;
int towNr = rand()%3 + 1;
Tower* archerTower;
int brkNr = rand()%2+1;
Barracks* barrack;

void endGame()
{
    free(enemy);
    free(archerTower);
    free(barrack);
    for(int i=0;i<map.height;i++)
        free(map.tile[i]);
    free(map.tile);
    exit(0);
}

void turn(int t)
{
    while(t > 0)
    {
        t--;

        for(int i=0;i<towNr;i++)
            for(int k=0;k<3;k++)
                archerTower[i].ranger[k].hasShot=false;

        int tdmg;

        for(int i=0;i<towNr;i++)
            for(int k=0;k<3;k++)
                for(int j=0;j<n;j++)
                    if(enemy[j].isAlive() && archerTower[i].inRange(enemy[j],k))
                        {
                            tdmg = archerTower[i].ranger[k].hit();
                            enemy[j].takeDmg(tdmg); 
                            archerTower[i].ranger[k].weapon.getDull();
                            if(archerTower[i].ranger[k].weapon.integrity<50)
                                archerTower[i].ranger[k].weapon.repair();
                            if(enemy[j].health<0)
                                enemy[j].kill();
                            cout<<"\nEnemy "<<j+1<<", HP: "<<enemy[j].health<<"/"<<enemy[j].maxHealth<<" pos: "<<enemy[j].x<<", took "<<tdmg<<" dmg from archer "<<k+1<<" of tower "<<i+1<<". Turn "<<t;
                        }    
                            
        for(int i=0;i<brkNr;i++)
            for(int j=0;j<n;j++)
                if(enemy[j].isAlive() && barrack[i].inRange(enemy[j]) && not enemy[j].inCombat)
                {
                    for(int k=0;k<3;k++)
                        if(barrack[i].fighter[k].isAlive() && not barrack[i].fighter[k].inCombat && not barrack[i].fighter[k].healing)
                        {
                            barrack[i].fighter[k].x = enemy[j].x;
                            barrack[i].fighter[k].y = enemy[j].y;
                            barrack[i].fighter[k].inCombat = true;
                            enemy[j].inCombat = true;
                            break;
                        }
                }

        for(int i=0;i<brkNr;i++)
            for(int k=0;k<3;k++)
                for(int j=0;j<n;j++)
                    if(enemy[j].isAlive() && enemy[j].inCombat && barrack[i].fighter[k].inCombat && barrack[i].fighter[k].isAlive() && not barrack[i].fighter[k].healing)
                    {
                        if(barrack[i].fighter[k].x == enemy[j].x && barrack[i].fighter[k].y == enemy[j].y)
                        {
                            tdmg = barrack[i].fighter[k].hit();
                            enemy[j].takeDmg(tdmg);
                            barrack[i].fighter[k].weapon.getDull();
                            if(barrack[i].fighter[k].weapon.integrity<50)
                                barrack[i].fighter[k].weapon.repair();
                            barrack[i].fighter[k].takeDmg(enemy[j].hit());
                            enemy[j].weapon.getDull();
                            if(enemy[j].weapon.integrity<50)
                                enemy[j].weapon.repair();
                            if(enemy[j].health<0)
                            {
                                enemy[j].kill();
                                barrack[i].fighter[k].inCombat = false;
                            }
                                
                            if(barrack[i].fighter[k].health<0)
                            {
                                barrack[i].fighter[k].kill();
                                barrack[i].fighter[k].healing = true;
                                enemy[j].inCombat = false;
                            }
                                
                            cout<<"\nEnemy "<<j+1<<", HP: "<<enemy[j].health<<"/"<<enemy[j].maxHealth<<" pos: "<<enemy[j].x<<", took "<<tdmg<<" dmg from knight "<<k+1<<" of barrack "<<i+1<<". Turn "<<t;
                            cout<<"\nKnight "<<k+1<<" of barrack "<<i+1<<", HP: "<<barrack[i].fighter[k].health<<"/"<<barrack[i].fighter[k].maxHealth<<" pos: "<<barrack[i].fighter[k].x<<", took "<<enemy[j].hit()<<" dmg from enemy "<<j+1<<". Turn "<<t;
                        }
                    }

        for(int i=0;i<brkNr;i++)
            for(int k=0;k<3;k++)
            {
                if(barrack[i].fighter[k].healing)
                {
                    barrack[i].fighter[k].getHeal(10);
                    if(barrack[i].fighter[k].health >= barrack[i].fighter[k].maxHealth)
                        {
                            barrack[i].fighter[k].health = barrack[i].fighter[k].maxHealth;
                            barrack[i].fighter[k].healing = false;
                        }
                }
            }

        for(int i=0;i<n;i++)
        {
            if(enemy[i].isAlive() && not enemy[i].inCombat)
            {
                enemy[i].moveRight();
                if(enemy[i].x==map.width)
                {
                    player.takeDmg(enemy[i].dmgValue);
                    enemy[i].kill();
                    cout<<"\nEnemy "<<i+1<<" passed through you defense.";
                }
                if(player.health<=0)
                        {
                            cout<<"\nEnemies have breached the walls of the castle. Game ended.";
                            endGame();
                        }
            }
        }
        int deathCount=0;

        for(int i=0;i<n;i++)
        {
            if(not enemy[i].isAlive())
                deathCount++;
        }

        if(n==deathCount)
        {
            cout<<"\nNo more enemies on the map. Player health "<<player.health<<"/"<<player.maxHealth;
            endGame();
        }  
    }
}



int main()
{
    system("clear");

    srand(time(NULL));
    
    map.tile=(int**)malloc(map.height*sizeof(int*));
    for(int i=0;i<map.height;i++)
        map.tile[i]=(int*)malloc(map.width*sizeof(int));

    enemy = (Enemy*)malloc(n*sizeof(Enemy));

    archerTower = (Tower*)malloc(towNr*sizeof(Tower));

    barrack = (Barracks*)malloc(brkNr*sizeof(Barracks));

    for(int i=0;i<n;i++)
        {
            enemy[i] = Enemy();
            enemy[i].y = rand()%map.height;
            cout<<"\nEnemy "<<i+1<<", HP: "<< enemy[i].health;
        }

    for(int i=0;i<towNr;i++)
    {
        archerTower[i] = Tower(rand()%map.width,rand()%map.height);
        cout<<"\nCreated archer tower at x: "<<archerTower[i].x<<" y: "<<archerTower[i].y;
    }

    for(int i=0;i<brkNr;i++)
    {
        barrack[i] = Barracks(rand()%map.width,rand()%map.height);
        cout<<"\nCreated barrack at x: "<<barrack[i].x<<" y: "<<barrack[i].y;
    }

    turn(10000);
    cout<<"\nPlayer health "<<player.health<<"/"<<player.maxHealth;
    endGame();

    return 0;
}