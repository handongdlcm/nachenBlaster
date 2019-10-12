#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameConstants.h"
#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iomanip>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void removeDeadObject();
    void updateDisplayText();
    //virtual void letItExplode();
    void newCabbage();
    void newTurnip(Actor* alien);
    void newSnagTorpedo(Actor* actor);
    void newNachenTorpedo();
    void newExtraLifeGoodie(Actor* alien);
    void newRepairGoodie(Actor* alien);
    void newTorpedoGoodie(Actor* alien);
    void CheckCabbageCollides(Cabbage* itCabbages);
    void CheckTorpedoCollides(Torpedo* itTorpedo);


private:
    int curLevel;
    int curAliens; 
    int totalAlien = 6+4*curLevel;
    int destroyedAlien;
    int maxOnScreenAlien = 4+0.5*curLevel;
    int s2 = 20+5*curLevel;
    int s3 = 5+10*curLevel;
    const int s1 = 60;
    int S = s1+s2+s3;
    
    list<Stars*> myStars;
    list<NachenBlaster*> myBlaster;
    list<Smallgon*> mySmallgons;
    list<Smoregon*> mySmoregons;
    list<Snagglegon*> mySnagglegons;
    list<Explosion*> myExplosion;
    list<Cabbage*> myCabbages;
    list<Turnip*> myTurnips;
    list<Torpedo*> myTorpedoes;
    list<ExtraLife*> myGoodies;
    
    string screenText;

};


#endif // STUDENTWORLD_H_
