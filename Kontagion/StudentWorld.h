#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include"Actor.h"
#include <string>
#include<list>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Actor* check_damage_overlap(Actor* a);
    void add_flame(double x, double y);
    Actor* check_other_overlap(Actor* a);
    Socrates* get_socrate() const;
    bool movement_overlap(double x, double y); 
    ~StudentWorld();
    void add_actor(Actor* a);
    Actor* food_overlap(Actor*a) ;
    void set_pit_over(); 
    void increase_counter();
    void decrease_counter();
    void increase_pit_counter();
    void decrease_pit_counter(); 
private:
    std::list<Actor*> all_actor;
    Socrates* player;
    bool pit_over;
    int bac_counter;
    int pit_counter;
};

#endif // STUDENTWORLD_H_


