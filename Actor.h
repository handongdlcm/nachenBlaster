#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


//////////////////////////Actor:///////////////////////////////////
class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* myWorld, int hitPoints = 1)
    : GraphObject(imageID, startX, startY, dir, size, depth), m_hitPoints(hitPoints), m_alive(true), m_World(myWorld)
    {}
    ~Actor(){}
    StudentWorld* getMyWorld() const;
    virtual void doSomething(){};
    virtual bool isAlive();
    int currentHitpoints() const;
    virtual void setHitpoints(int num){
        m_hitPoints = num*m_hitPoints;
    };
    virtual bool isCollide(Actor* actor);
    virtual void setDead(){m_alive = false;}
    
protected:
    bool m_alive;
    int m_hitPoints;
private:
    StudentWorld* m_World;
};

//////////////////////////Star:///////////////////////////////////
class Stars: public Actor{
public:
    //Star Concstructor:
    Stars(double startX, double startY, StudentWorld* myWorld)
    : Actor(IID_STAR, startX, startY, 0, 0.01*randInt(5, 50), 3, myWorld)
    {}
    ~Stars(){}
    virtual void doSomething();
    //void getRidOfOneStar();
};


//////////////////////////Nachenblaster:///////////////////////////////
class NachenBlaster: public Actor{
public:
    NachenBlaster(double startX, double startY,StudentWorld* myWorld)
    :Actor(IID_NACHENBLASTER, startX, startY, 0, 1.0, 0, myWorld, 50), m_cabbageEnergy(30),m_numTorpedo(0) //m_World(myWorld)
    {}
    ~NachenBlaster(){}
    virtual void doSomething();
    virtual void setHitpoints(int num);
    void increaseCabbage(){m_cabbageEnergy++;}
    int getCabbageEnergy(){return m_cabbageEnergy;}
    int getNumTorpedo(){return m_numTorpedo;}
    void increaseTorpedo(){m_numTorpedo = m_numTorpedo+5;}
private:
    int m_cabbageEnergy;
    int m_numTorpedo;
};


//////////////////////////Smallgon:///////////////////////////////
class Smallgon: public Actor{
public:
    Smallgon(double startX, double startY,StudentWorld* myWorld,int curLev, int picID, NachenBlaster* myBlaster)
    :Actor(picID, startX, startY, 0, 1.5, 1, myWorld,5*(1+(curLev*0.1))),m_speed(2.0),m_distance(0),dirIndex(0), m_Blaster(myBlaster), currentLev(curLev)
    {}
    ~Smallgon();
    virtual void doSomething();
    virtual bool isAlive();
    virtual void letItMove();
    virtual bool FireorSprint();
    
    virtual void setHitpoints(int num){
        m_hitPoints = num+m_hitPoints;
    };
    
    NachenBlaster* getMyBlaster() const;
    int getDirIndex() const;
    void setDirIndex(int dir);
    int getDistance() const;
    void setDistance(int dis);
    int getCurLevel() const;
    double getSpeed() const;
    void setSpeed(double speed);
    
protected:
    NachenBlaster* m_Blaster;
    double m_speed;
    int m_distance;
    int dirIndex;
    int currentLev;

};


//////////////////////////Smoregan:///////////////////////////////

class Smoregon: public Smallgon{
public:
    Smoregon(double startX, double startY,StudentWorld* myWorld,int curLev, int picID, NachenBlaster* myBlaster)
    :Smallgon(startX, startY, myWorld, curLev, IID_SMOREGON,myBlaster)
    {}
    ~Smoregon(){}
    virtual void doSomething();
    virtual bool FireorSprint();
    virtual void Sprint();
private:
    bool isSprint = false;
    
};


//////////////////////////Snagglegon:///////////////////////////////
class Snagglegon: public Smallgon{
public:
    Snagglegon(double startX, double startY,StudentWorld* myWorld,int curLev, NachenBlaster* myBlaster)
    :Smallgon(startX, startY, myWorld, curLev, IID_SNAGGLEGON,myBlaster)
    {
        setHitpoints(2);
        setSpeed(1.75);
        setDirIndex(3);
    }
    virtual void letItMove();
    virtual void doSomething();
    virtual bool FireorSprint();
private:
    
};

//////////////////////////Explosion:///////////////////////////////
class Explosion:public Actor{
public:
    Explosion(double startX, double startY, StudentWorld* myWorld)
    : Actor(IID_EXPLOSION, startX, startY, 0, 1, 0,myWorld),m_ticks(3)
    {};
    virtual void doSomething(){
        setSize(1.5*getSize());
        m_ticks--;
    }
    
    int getTicks() const{ return m_ticks; }
private:
    int m_ticks;

};

//////////////////////////Projectiles & Goodies///////////////////////////
class Projectile: public Actor{
public:
    Projectile(double startX, double startY,StudentWorld* myWorld, int picID, NachenBlaster* myBlaster, int fightPoints, double speed)
    : Actor(picID, startX, startY, 0, 0.5, 1,myWorld),  m_Blaster(myBlaster),m_fightPoints(fightPoints),m_speed(speed)
    {}
    ~Projectile(){}
    virtual bool isAlive();
    virtual void doSomething();
    virtual void letItMove();
    double getSpeed() const {return m_speed;}
    NachenBlaster* getMyBlaster() const;
    
private:
    NachenBlaster* m_Blaster;
    //Smallgon* m_Smallgons;
    //Smoregon* m_Smoregons;
    int m_fightPoints;
    
protected:
    
    double m_speed;
    
};

//////////////////////////Cabbage///////////////////////////////
class Cabbage: public Projectile{
public:
    Cabbage(double startX, double startY,StudentWorld* myWorld, NachenBlaster* myBlaster, int fightPoints, double speed)
    :Projectile(startX, startY, myWorld, IID_CABBAGE, myBlaster, fightPoints, speed)
    {}

private:
    int m_energyPoints = 5;
};

//////////////////////////Turnip///////////////////////////////
class Turnip: public Projectile{
public:
    Turnip(double startX, double startY,StudentWorld* myWorld, NachenBlaster* myBlaster, int fightPoints, double speed)
    :Projectile(startX, startY, myWorld, IID_TURNIP, myBlaster, fightPoints, speed)
    {}
    bool isAlive();
private:
    
};

////////////////////////////Torpedo/////////////////////////////////
class Torpedo: public Projectile{
public://speed>0 is NachenBlaster Torpedo, speed<0 is Snagglegon Torpedo
    Torpedo(double startX, double startY,StudentWorld* myWorld, NachenBlaster* myBlaster, int fightPoints, double speed)
    :Projectile(startX, startY, myWorld, IID_TORPEDO, myBlaster, fightPoints, speed)
    {}
    ~Torpedo(){}
    bool isAlive();
    virtual void letItMove();
    
private:
    
};

//////////////////////////Extralife Goodie//////////////////////////////
class ExtraLife: public Projectile{
public:
    ExtraLife(double startX,double startY,StudentWorld* myWorld, int picID, NachenBlaster* myBlaster)
    :Projectile(startX,startY,myWorld,picID,myBlaster,0,0.75)
    {}
    ~ExtraLife(){}
    virtual bool isAlive();
    virtual void doSomething();
    virtual void letItMove();
    virtual int benifit(){return 1;}
};


//////////////////////////Repair Goodie//////////////////////////////
class RepairGoodie: public ExtraLife{
public:
    RepairGoodie(double startX,double startY,StudentWorld* myWorld, NachenBlaster* myBlaster)
    :ExtraLife(startX,startY,myWorld,IID_REPAIR_GOODIE,myBlaster)
    {}
    virtual int benifit(){return 2;}

};

//////////////////////////FlatulanTor//////////////////////////////

class FlatulanTor: public ExtraLife{
public:
    
    FlatulanTor(double startX,double startY,StudentWorld* myWorld, NachenBlaster* myBlaster)
    :ExtraLife(startX,startY,myWorld,IID_TORPEDO_GOODIE,myBlaster)
    {}
    virtual int benifit(){return 3;}

    
};






#endif // ACTOR_H_
