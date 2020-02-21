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


bool Actor::isOverlap(double x, double y)
{
    double distance = pow((getX()-x),2)+pow((getY()-y),2);
    return distance <= 4*SPRITE_RADIUS*SPRITE_RADIUS;
}
bool Actor::checkOverlap(Actor* actor_pt)
{
    double current_x = actor_pt -> getX();
    double current_y = actor_pt -> getY();
    return isOverlap(current_x, current_y);
}

 double Actor::calculate_distance(Actor* a, Actor*b)
{
    double a_x = a->getX();
    double a_y = a->getY();
    double b_x = b->getX();
    double b_y = b->getY();
    double distance = pow((a_x-b_x),2)+pow((a_y-b_y),2);
    return distance;
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
                   positional_angle += 5;
                   double next_x, next_y;
                    calculate_position(positional_angle,128,next_x,next_y,128,128);
                    moveTo(next_x, next_y);
                    setDirection(positional_angle+180);
                    break;
                case KEY_PRESS_RIGHT:
                    positional_angle -= 5;
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
                        get_student_world()->add_actor(new spray(spray_x, spray_y, get_student_world(), getDirection(), this));
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

//fungus constructor
fungus::fungus(double startX, double startY,StudentWorld* new_petri, bool damage, Direction dir, int depth):
Actor(IID_FUNGUS,startX,startY,new_petri, damage, dir, depth){
    life_time = max(rand()%(300-10*new_petri->getLevel()),50);
}

void fungus::doSomething()
{
    if(!check_alive()) return;
    else{
        
        if(get_student_world()->get_socrate()->check_alive())
        {
            get_student_world() -> increaseScore(-50);
            set_alive(-1);
            int current_point = get_student_world() -> get_socrate() ->get_hit_pt();
            get_student_world()->get_socrate()->set_alive(current_point-20);
            return;
        }
        if(life_time<=0) set_alive(-1);
        life_time--;
    }
}

bacteria::bacteria(int image_id, double x, double y, StudentWorld* new_petri, int initial_point,int hurt_pt, int movement_plan_distance,Direction dir, bool damage, int depth):Actor(image_id, x,y,new_petri, true, dir, depth)
{
    m_movement_distance = movement_plan_distance;
    set_alive(initial_point);
    food_count = 0;
    initial_pt = initial_point;
    my_id = image_id;
    hurt_point = hurt_pt;
}

//if the bacteria overlap with socrate, then it would hurt it by the hurt_pt
void bacteria::hurt_socrate()
{
    if(checkOverlap(get_student_world()->get_socrate()))
    {
        int current_hp = get_student_world()->get_socrate()->get_hit_pt();
               get_student_world()->get_socrate()->set_alive(current_hp-hurt_point);
}
    move_to_food();
}

void bacteria::generate_new()
{
       if(food_count ==3)
       {
           if(getX()<VIEW_WIDTH/2) moveTo(getX()+SPRITE_RADIUS,getY());
           else if(getX()>VIEW_WIDTH/2) moveTo(getX()-SPRITE_RADIUS,getY());
           if(getY()<VIEW_HEIGHT/2) moveTo(getX(), getY()+SPRITE_RADIUS);
           else if(getY()<VIEW_HEIGHT/2) moveTo(getX(), getY()-SPRITE_RADIUS);
               double new_x = getX();
           double new_y = getY();
           bacteria* new_bacteria = new bacteria(my_id,new_x, new_y, get_student_world(), m_movement_distance,initial_pt,getDirection());
           get_student_world()->add_actor(new_bacteria);
           food_count = 0;
       }
    move_to_food();
}

void bacteria::eat_food()
{
        if(get_student_world()->food_overlap(this)!=nullptr)
                  {
                      food_count++;
                      get_student_world()->food_overlap(this)->set_alive(-1);
                  }
    move_to_food();
}
  

void bacteria::move_to_food()
{
        if(m_movement_distance>=0)
                {
                    m_movement_distance--;
                    double potential_x, potential_y;
                    getPositionInThisDirection(getDirection(), 3, potential_x, potential_y);
                    double center_distance = pow((potential_x-VIEW_WIDTH/2),2)+pow((potential_y-VIEW_HEIGHT/2),2);
        
                    if(!get_student_world()->movement_overlap(potential_x, potential_y) && center_distance < VIEW_RADIUS*VIEW_RADIUS)
                    {
        
                            moveAngle(getDirection(),3);
        
                    }
                    else{
                        int new_direction = randInt(0, 359);
                        setDirection(new_direction);
                        m_movement_distance=10;
                        return;
                    }
        
                }else
                {
                    if(!get_student_world()->food_overlap(this))
                    {
                        int new_dir = randInt(0,359);
                        setDirection(new_dir);
                        m_movement_distance=10;
                        return;
                    }
                    else{
        
                        double potential_x, potential_y;
                        int new_direction = get_student_world() -> food_overlap(this)->getDirection();
                        setDirection(new_direction);
                        double new_distance = calculate_distance(this, get_student_world()->food_overlap(this));
                        getPositionInThisDirection(new_direction ,new_distance, potential_x, potential_y);
                        if(get_student_world()->movement_overlap(potential_x, potential_y))
                        {int new_dir = randInt(0,359);
                         setDirection(new_dir);
                            m_movement_distance=10;}
                    }
        
}
}

void bacteria::doSomething()
{
     if(!check_alive()) return;
}


//void bacteria::doSomething()
//{
//    //check whether the bacteria is still alive
//    if(!check_alive()) return;
//    else{
//        //check whether the bacteria overlaps with anotehr socrate, if it is, then hurt the socrate by 1 pt
//        if(checkOverlap(get_student_world()->get_socrate()))
//        {
//            int current_hp = get_student_world()->get_socrate()->get_hit_pt();
//                   get_student_world()->get_socrate()->set_alive(current_hp-hurt_point);
//        }
//        //else if see whether the food count of this bacteria is 3; if so, then add a new bacteria at the coordinate
//       else if(food_count==3)
//       {
//               if(getX()<VIEW_WIDTH/2) moveTo(getX()+SPRITE_RADIUS,getY());
//               else if(getX()>VIEW_WIDTH/2) moveTo(getX()-SPRITE_RADIUS,getY());
//               if(getY()<VIEW_HEIGHT/2) moveTo(getX(), getY()+SPRITE_RADIUS);
//               else if(getY()<VIEW_HEIGHT/2) moveTo(getX(), getY()-SPRITE_RADIUS);
//                   double new_x = getX();
//               double new_y = getY();
//               bacteria* new_bacteria = new bacteria(my_id,new_x, new_y, get_student_world(), m_movement_distance,initial_pt,getDirection());
//               get_student_world()->add_actor(new_bacteria);
//               food_count = 0;
//       }
//       else{
//           if(get_student_world()->food_overlap(this)!=nullptr)
//           {
//               food_count++;
//               get_student_world()->food_overlap(this)->set_alive(-1);
//           }
//       }
//        if(m_movement_distance>=0)
//        {
//            m_movement_distance--;
//            double potential_x, potential_y;
//            getPositionInThisDirection(getDirection(), 3, potential_x, potential_y);
//            double center_distance = pow((potential_x-VIEW_WIDTH/2),2)+pow((potential_y-VIEW_HEIGHT/2),2);
//
//            if(!get_student_world()->movement_overlap(potential_x, potential_y) && center_distance < VIEW_RADIUS*VIEW_RADIUS)
//            {
//
//                    moveAngle(getDirection(),3);
//
//            }
//            else{
//                int new_direction = randInt(0, 359);
//                setDirection(new_direction);
//                m_movement_distance=10;
//                return;
//            }
//
//        }else
//        {
//            if(!get_student_world()->food_overlap(this))
//            {
//                int new_dir = randInt(0,359);
//                setDirection(new_dir);
//                m_movement_distance=10;
//                return;
//            }
//            else{
//
//                double potential_x, potential_y;
//                int new_direction = get_student_world() -> food_overlap(this)->getDirection();
//                setDirection(new_direction);
//                double new_distance = calculate_distance(this, get_student_world()->food_overlap(this));
//                getPositionInThisDirection(new_direction ,new_distance, potential_x, potential_y);
//                if(get_student_world()->movement_overlap(potential_x, potential_y))
//                {int new_dir = randInt(0,359);
//                 setDirection(new_dir);
//                    m_movement_distance=10;}
//            }
//
//
//
//        }
//    }
//}

regular_salmonella::regular_salmonella(double x, double y, StudentWorld* new_petri, int initial_pt,int hurt_pt):bacteria(IID_SALMONELLA, x, y, new_petri,initial_pt,hurt_pt )
{
    doSomething();
    hurt_socrate();
    generate_new();
    eat_food();
}

aggressive_salmonella::aggressive_salmonella(double x, double y, StudentWorld* new_petri, int initial_pt,int hurt_pt):
bacteria(IID_SALMONELLA,x,y,new_petri, initial_pt,hurt_pt)
{
    doSomething();
    double socrate_distance = calculate_distance(this, get_student_world()->get_socrate());
    if(socrate_distance <=72*72) 
    {
        Direction new_dir = get_student_world()->get_socrate()->getDirection();
        if(!get_student_world()->movement_overlap(getX(), getY()))
        {
            setDirection(new_dir);
        }
    }
    hurt_socrate();
    generate_new();
    eat_food();
}


E_coli::E_coli(double x, double y, StudentWorld* new_petri, int initial_pt, int hurt_pt):
bacteria(IID_ECOLI,x, y, new_petri, initial_pt, hurt_pt)
{
    doSomething();
    hurt_socrate();
    generate_new();
    eat_food();
    double distance_to_socrate = calculate_distance(this, get_student_world()->get_socrate());
    if(distance_to_socrate<=256*256)
    {
        Direction theta = get_student_world()->get_socrate()->getDirection();
        
        
    }
    
}

pit::pit(double x, double y, StudentWorld* new_petri, int initial_r_s , int initial_a_s , int initial_Ecoli, Direction dir, int depth):Actor(IID_PIT,x,y,new_petri, true, dir,depth){
    r_s = initial_r_s;
    a_s = initial_a_s;
    Ecoli = initial_Ecoli;
}






