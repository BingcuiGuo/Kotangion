#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include<list>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    pit_over = false; 
    player = nullptr;
}

//construct a new socrates and the number of dirt in the game
int StudentWorld::init()
{
    player = new Socrates(this);
    int L = getLevel();
   
    int food_num = min(5*L, 25);
    for (int i=0; i<food_num; i++)
    {
        int position_x, position_y;
        size_t current_size = all_actor.size();
        do{
         do{
            position_x = randInt(8,248);
            position_y = randInt(8,248);
               }while((position_x-128)*(position_x-128)+(position_y-128)*(position_y-128) > 120*120);
       
            food* new_food = new food(this, position_x, position_y);
                   if(check_other_overlap(new_food)==nullptr)
                   {
                        all_actor.push_back(new_food);
                   }
                   else delete new_food;
               
        }while(all_actor.size()==current_size);
        
    }
    int numdirt = max(180 - 20*L, 20);
       for(int i=0; i<numdirt; i++)
       {
           int position_x, position_y;
           do{
               position_x = randInt(8,248);
               position_y = randInt(8,248);
           }while((position_x-128)*(position_x-128)+(position_y-128)*(position_y-128) > 120*120);
           
           dirt* new_dirt = new dirt(position_x, position_y,this);
           all_actor.push_back(new_dirt);
       }
    
    return GWSTATUS_CONTINUE_GAME;
}


//enable everything in the all_actor to do something
int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
//    decLives();
    
//    return GWSTATUS_PLAYER_DIED;
    
    list<Actor*>::iterator i = all_actor.begin();
    while(i !=all_actor.end())
    {
        (*i)->doSomething();
        i++;
    }
    player->doSomething();
    list<Actor*>::iterator check_death = all_actor.begin();
    while(check_death!=all_actor.end())
    {
        if(!(*check_death)->check_alive())
        {
            list<Actor*>::iterator n = check_death;
            check_death++;
            delete *n;
            all_actor.erase(n);
        }
        check_death++;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete player;
    list<Actor*>::iterator i = all_actor.begin();
    while(i !=all_actor.end())
    {
        list<Actor*>::iterator n = i;
        i++;
        delete *n;
        all_actor.erase(n);
    }
    
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}


//check whether actor would overlap with another thing within 8 pixels 
Actor* StudentWorld::check_other_overlap(Actor* a)
{
    list<Actor*>::iterator it = all_actor.begin();
    while(it !=all_actor.end())
    {  double current_x = a->getX();
        double current_y = a->getY();
        double other_x = (*it)->getX();
        double other_y = (*it)-> getY();
        double distance = pow((current_x-other_x),2)+pow((current_y-other_y),2);
        if(distance <= SPRITE_RADIUS*2 )
        {
            return (*it);
        }
        it++;
    }
    return nullptr;
}

//iterate trhough all actor list and check whether actor a overlaps with a damageable thing within 64 pixels and if so, return the damanagebla thing's pointer, otherwise, return nullptr
Actor* StudentWorld::check_damage_overlap(Actor* a)
{
    list<Actor*>::iterator it = all_actor.begin();
    while(it !=all_actor.end())
    {
        double other_x = (*it)->getX();
        double other_y = (*it)-> getY();
        if(a->isOverlap(other_x, other_y) && (*it) -> get_damagae_status())
        {
            return (*it);
        }
        it++;
    }
    return nullptr;
}



//add flame to the petri-dish
void StudentWorld::add_flame(double x, double y)
{
   int current_angle = player -> getDirection();
    
    for (int i=0;i<16; i++)
    {
        double new_flame_x, new_flame_y;
        player -> getPositionInThisDirection(current_angle, 2*SPRITE_RADIUS, new_flame_x, new_flame_y);
        flame* new_flame = new flame(new_flame_x, new_flame_y,this, current_angle, player);
        current_angle += 22;
      
        all_actor.push_back(new_flame);
    }
}

//return socrate
Actor* StudentWorld::get_socrate() const
{
    return player;
}

//check whether it is bateria and the dirt overlaps if so return true
bool StudentWorld::movement_overlap(double x, double y) 
{
    list<Actor*>::iterator it = all_actor.begin();
       while(it !=all_actor.end())
       { if((*it)->isdirt())
       {
           double current_x = (*it)->getX();
           double current_y = (*it)->getY();
           double distance = pow((current_x-x),2)+pow((current_y-y),2);
           if(distance <=SPRITE_RADIUS*SPRITE_RADIUS) return true;
       }
       }
       return false;
}

//add an actor to the petri
void StudentWorld::add_actor(Actor* a)
{
    all_actor.push_back(a);
}

//check whether the actor could be overlap with a food within 128*128 and return the closest food
Actor* StudentWorld::food_overlap(Actor* a) 
{
    list<Actor*>::iterator it = all_actor.begin();
    int min_distance = 50000;
    Actor* initial_food = nullptr;
    double current_x = a->getX();
    double current_y = a->getY();
       while(it !=all_actor.end())
       {
           if((*it)->is_food())
           {
               double food_x = (*it)->getX();
               double food_y = (*it)->getY();
               double current_distance = pow((current_x - food_x),2)+pow((current_y-food_y),2);
               if(current_distance < 128*128 && current_distance < min_distance)
               {
                   min_distance = current_distance;
                   initial_food = (*it);
               }
           }
           it++;
       }
    return initial_food;
}

void StudentWorld::set_pit_over()
{
    pit_over=true; 
}


