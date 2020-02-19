#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld; 

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY,StudentWorld* new_petri, bool damage=true, Direction dir = 0, int depth = 0);
    virtual void doSomething() = 0;
    bool check_alive() const;
    void set_alive(int status);
    int get_hit_pt() const; 
    StudentWorld* get_student_world() const;
    bool get_damagae_status() const;
    void calculate_position(int positional_angle, int units, double& new_x, double& new_y,double center_x, double center_y);
    void move_new_direction(double center_x, double center_y, int center_diretional_angle, int moving_units);
private:
    int hit_pt;
    bool isdamaneagble;
    StudentWorld* m_petri; 
};



                            
                            
class Socrates:public Actor{
public:
    Socrates(StudentWorld* new_petri);
    virtual void doSomething();
    int get_directional_angle() const; 
private:
    int positional_angle;
    int spray_charges;
    int flame_thrower;
};


class dirt:public Actor{
public:
     dirt(double startX, double startY,StudentWorld* new_petri, Direction dir = 0, int depth = 1);
     virtual void doSomething();
};

class food: public Actor{
public:
    food(StudentWorld* new_petri, double x,double y);
    void doSomething();
};

class weapon:public Actor{
public:
    weapon(int ImageID, double x, double y, StudentWorld* new_petri,  Direction dir, int max_distance, Socrates* new_socrates, int depth=1);
    void doSomething();
    void set_loss_pt(int n);
    
private:
    int max_travel_distance;
    int current_movement;
    Socrates* m_socrates;
    int loss_pt; 
};

class flame:public weapon{
public:
    flame(double x, double y, StudentWorld* new_petri,  Direction dir,  Socrates* new_socrates);

   
};

class spray:public weapon{
public:
    spray(double x, double y, StudentWorld* new_petri,  Direction dir,  Socrates* new_socrates);

  
};

#endif // ACTOR_H_
