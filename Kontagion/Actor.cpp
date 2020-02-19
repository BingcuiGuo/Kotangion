#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor:: Actor(int imageID, double startX, double startY,StudentWorld* new_petri, bool damage,Direction dir , int depth):GraphObject(imageID, startX, startY, dir, depth)
{
    isdamaneagble = damage;
    hit_pt=0 ;
    m_petri = new_petri;
}
//return true if hit point is larger than 0
bool Actor::check_alive() const
{
    return hit_pt>=0;
}

//return hit point
int Actor::get_hit_pt() const
{
    return hit_pt;
}

//set the hit point to status point
void Actor::set_alive(int status)
{
    hit_pt = status;
}

//return the student world
 StudentWorld* Actor::get_student_world() const
{return m_petri;}

///calculate the position that the thing will move to
void Actor::calculate_position(int positional_angle, int units, double& new_x, double& new_y,double center_x, double center_y)
{
    moveTo(center_x, center_y);
    moveAngle(positional_angle, units);
    new_x = getX();
    new_y = getY();
}

//return true if the object is damageable
bool Actor::get_damagae_status() const
{
    return isdamaneagble;
}

//constructor
Socrates::Socrates(StudentWorld* new_petri):
Actor(IID_PLAYER, 0, 128, new_petri, 0)
{
     positional_angle = 180;
     spray_charges = 20;
     flame_thrower =5;
     set_alive(100);
}


//check alive status first and if the user press left, it would move the socrates to the left; if the user press right,
//then the socrate will move to the right; if the user press enter, then the socrate would use the flame;
// if the user press space, the socrate would use the spray
void Socrates::doSomething()
{
    int ch;
    StudentWorld* my_petri = get_student_world();
    if( !check_alive()) return;
    else{
        if( my_petri -> getKey(ch))
        {
            switch(ch)
            {
                case KEY_PRESS_LEFT:
                   positional_angle -= 5;
                   double next_x, next_y;
                    calculate_position(positional_angle,128,next_x,next_y,128,128);
                    moveTo(next_x, next_y);
                    setDirection(positional_angle+180);
                    break;
                case KEY_PRESS_RIGHT:
                    positional_angle += 5;
                    double next_x_2, next_y_2;
                    calculate_position(positional_angle,128,next_x_2,next_y_2,128,128);
                    moveTo(next_x_2, next_y_2);
                    setDirection(positional_angle+180);
                    break;
                case KEY_PRESS_SPACE:
                    if(spray_charges>=1)
                    {
                        double spray_x, spray_y;
                        getPositionInThisDirection(getDirection(), 2*SPRITE_RADIUS, spray_x, spray_y);
                        get_student_world()->add_spray(spray_x, spray_y,getDirection());
                        spray_charges --;
                        get_student_world()->playSound(SOUND_PLAYER_SPRAY);
                    }
                    break;
                case KEY_PRESS_ENTER:
                    if(flame_thrower>=1)
                    {
                        get_student_world()->add_flame(getX(), getY());
                        flame_thrower--;
                        get_student_world()->playSound(SOUND_PLAYER_FIRE);
                    }
                    
                break;
            }
            
        }
        else return;
    }
}

//dirt construtcor
dirt:: dirt(double startX, double startY,StudentWorld* new_petri, Direction dir, int depth):
Actor(IID_DIRT, startX, startY, new_petri,true, dir, depth) {}


//dirt do nothing
void dirt::doSomething()
{
}

//return the socrates' directional angle
int Socrates::get_directional_angle() const
{
    return positional_angle;
}
//food constructor
food::food(StudentWorld* new_petri, double x, double y):Actor(IID_FOOD, x, y,new_petri, false,90, 1){}

//food do nothing
void food::doSomething()
{
}
//weapon constructor
weapon::weapon(int ImageID, double x, double y, StudentWorld* new_petri,  Direction dir, int max_distance, Socrates* new_socrates, int depth):
Actor(ImageID, x,y, new_petri, false, dir,depth), current_movement(0), loss_pt(0)
{
    m_socrates = new_socrates;
    max_travel_distance = max_distance;
}


//check whether the weapon is alive first, if not, return;
//else, then check whether this weapon overlap with other damageable thing, and cause them to lose point
//or they would travel to their maximum distance and die
void weapon::doSomething()
{
    if(!check_alive()) return;
    else{
  
        if(get_student_world()->check_damage_overlap(this)!=nullptr)
        {
            Actor* damage_obj = get_student_world()->check_damage_overlap(this);
            damage_obj -> set_alive(damage_obj->get_hit_pt()-loss_pt);
            set_alive(-1);
            return;
        }
        else
        {
            moveAngle(getDirection(), SPRITE_RADIUS*2);
            current_movement+=2*SPRITE_RADIUS;
            if (current_movement>=max_travel_distance)
                set_alive(-1);
        }
    }
 }

//set the weapon's point to n
void weapon::set_loss_pt(int n)
{
    loss_pt=n;
}

//flame constructor
flame::flame(double x, double y, StudentWorld* new_petri,  Direction dir, Socrates* new_socrates):
weapon(IID_FLAME, x,y, new_petri, dir, 32, new_socrates){set_loss_pt(5);}

//spray constructor
spray::spray(double x, double y, StudentWorld* new_petri,  Direction dir, Socrates* new_socrates):
weapon(IID_SPRAY, x,y, new_petri, dir, 112, new_socrates){set_loss_pt(2); }
