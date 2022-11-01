#include<iostream>
#include<cstdlib>
#include<ctime>
#include <unistd.h>
using namespace std;

#include"src/equipment.h"
#include"src/character.h"
#include"src/environment.h"

Map map=Map();

Player player=Player();
int n = 20;
Enemy* enemy;
int towNr = 8;
Tower* archerTower;
int brkNr = 4;
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

        for(int i=0;i<n;i++)
        {
            if(enemy[i].deployed)
                continue;
            else
            {
                enemy[i].deploy();
                break;
            }
        }

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
                            archerTower[i].ranger[k].hasShot=true;
                            archerTower[i].ranger[k].weapon.getDull();
                            if(archerTower[i].ranger[k].weapon.integrity<50)
                                archerTower[i].ranger[k].weapon.repair();
                            if(enemy[j].health<=0)
                            {    
                                enemy[j] = Enemy();
                                player.killCount += 1;
                            }
                        }    
                            
        for(int i=0;i<brkNr;i++)
            for(int j=0;j<n;j++)
                if(enemy[j].isAlive() && barrack[i].inRange(enemy[j]) && not enemy[j].inCombat)
                {
                    for(int k=0;k<3;k++)
                        if(not enemy[j].inCombat && barrack[i].fighter[k].isAlive() && not barrack[i].fighter[k].inCombat && not barrack[i].fighter[k].healing)
                        {
                            barrack[i].fighter[k].engage(j);
                            enemy[j].engage(k);
                            break;
                        }
                }
        

        for(int i=0;i<brkNr;i++)
            for(int k=0;k<3;k++)
                for(int j=0;j<n;j++)
                    if
                    (
                        enemy[j].isAlive() && enemy[j].inCombat 
                        && barrack[i].fighter[k].inCombat && 
                        barrack[i].fighter[k].isAlive() && 
                        not barrack[i].fighter[k].healing 
                    )
                    {
                        if(barrack[i].fighter[k].fighting == j && k == enemy[j].fighting)
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

                            if(enemy[j].health<=0)
                            {
                                enemy[j] = Enemy();
                                barrack[i].fighter[k].disengage();
                                barrack[i].knightResetPos(k);
                                player.killCount += 1;
                            }
                                
                            if(barrack[i].fighter[k].health<=0)
                            {
                                barrack[i].fighter[k].kill();
                                barrack[i].fighter[k].healing = true;
                                enemy[j].disengage();
                                barrack[i].fighter[k].disengage();
                                barrack[i].knightResetPos(k);
                                player.knightDefeatCount += 1;
                            }
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
            if(enemy[i].deployed && enemy[i].isAlive() && not enemy[i].inCombat)
            {
                enemy[i].moveRight();
                if(enemy[i].x==map.width)
                {
                    player.breachCount += 1;
                    enemy[i] = Enemy();
                }
            }
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
            enemy[i] = Enemy();

    int towSpacing = map.width/towNr;
    for(int i=0;i<towNr;i++)
    {
        int y;
        if(i%2==0) y=22; else y = 28;
        archerTower[i] = Tower(i*towSpacing,y);
    }

    towSpacing = map.width/brkNr;
    for(int i=0;i<brkNr;i++)
    {
        int y;
        if(i%2==0) y=22; else y = 28;
        barrack[i] = Barracks(i*towSpacing,y);
    }
    
    for(int i=0;i<100;i++)
    {
        turn(50);
        system("clear");
        cout<<"\nNr of defeated enemies: "<<player.killCount<<endl;
        cout<<"Nr of defeated knights: "<<player.knightDefeatCount<<endl;
        cout<<"Nr of escaped enemies: "<<player.breachCount<<endl;
        //cout<<"Nr of : "<<player.<<endl;
        usleep(250000);
    }
    endGame();

    return 0;
}