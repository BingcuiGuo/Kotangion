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
    virtual bool isdirt() const {return false; }
    StudentWorld* get_student_world() const;
    bool get_damagae_status() const;
    void calculate_position(int positional_angle, int units, double& new_x, double& new_y,double center_x, double center_y);
    void move_new_direction(double center_x, double center_y, int center_diretional_angle, int moving_units);
    bool isOverlap(double x, double y);
    bool checkOverlap(Actor* actor_pt);
    virtual bool is_food(){return false;}
    double calculate_distance(Actor* a, Actor*b);
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
    virtual bool isdirt() const {return true;}
};

class food: public Actor{
public:
    food(StudentWorld* new_petri, double x,double y);
    void doSomething();
    virtual bool is_food(){return true;}
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

class bacteria:public Actor{
public:
    bacteria(int image_id, double x, double y, StudentWorld* new_petri, int initial_point,int movement_plan_distance=0,
            Direction dir=90, bool damage=true,  int depth=0);
    void doSomething();

   
private:
    int m_movement_distance;
    int food_count;
    int initial_pt;
    int my_id; 
};

class fungus:public Actor{
public:
     fungus( double startX, double startY,StudentWorld* new_petri, bool damage=true, Direction dir = 0, int depth = 1);
    void doSomething(); 
private:
    int life_time;
};

class regular_salmonella:public bacteria{
public:
    regular_salmonella(double x, double y, StudentWorld* new_petri, int initial_pt=4);
};

class aggressive_salmonella:public bacteria{
public:
    aggressive_salmonella(double x, double y, StudentWorld* new_petri, int initial_pt=10);
    
};

class E_coli: public bacteria{
public:
    E_coli(double x, double y, StudentWorld* new_petri, int initial_pt=5, Direction dir = 90, int depth=0);
};


#endif // ACTOR_H_
