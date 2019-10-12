#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), curLevel(1), curAliens(0),destroyedAlien(0)
{
    
}

int StudentWorld::init()
{
    curAliens = 0;

    totalAlien = 6+4*curLevel;
    destroyedAlien = 0;
    s2 = 20+5*curLevel;
    s3 = 5+10*curLevel;
    S = s1+s2+s3;
    maxOnScreenAlien = 4+0.5*curLevel;
    //the NachenBlaster
    myBlaster.push_back(new NachenBlaster(0,128,this));
    //the Stars
    for (int i=0; i<30; i++)
        myStars.push_back(new Stars(randInt(0, 255),randInt(0, 255),this));
//    //the Smallgons
//    for(int i=0; i<2; i++){
//        mySmallgons.push_back(new Smallgon(255, randInt(0, 255),this, curLevel,IID_SMALLGON,*myBlaster.begin()));
//        curAliens++;
//    }
//    //the Smoregons
//    mySmoregons.push_back(new Smoregon(255, randInt(0, 255),this, curLevel,IID_SMOREGON,*myBlaster.begin()));
//    curAliens++;
//
//    //the Snagglegons
//    mySnagglegons.push_back(new Snagglegon(255, randInt(0, 255),this, curLevel,*myBlaster.begin()));
//    curAliens++;
    
    //the Onscreen Text
    ostringstream oss;
    oss<<"Lives: "<<setw(1)<<getLives()<<"  Health: "<<setw(4)<<2*(*myBlaster.begin())->currentHitpoints()<<"%  Score: "<<setw(3)<<getScore()<<"  Level: "<<curLevel<<"  Cabbages: "<<setw(3)<<(*myBlaster.begin())->getCabbageEnergy()*100/30<<"%  Torpedoes: "<<(*myBlaster.begin())->getNumTorpedo();
    screenText = oss.str();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    cerr<< "wtf" <<endl;
    
//////////////////////////Star:///////////////////////////////////
    if(randInt(1, 15) == 1){
        myStars.push_back(new Stars(255,randInt(0, 255),this));
    }
    list<Stars*>::iterator it;
    it=myStars.begin();
    while(it!=myStars.end())
    {
        //move each star
        (*it)->doSomething();
        //get rid of off-screen stars
        if((*it)->getX() < 0){
            delete (*it);    //memory leak!!!!!!
            it = myStars.erase(it);
        }
        it++;
    }
    
//////////////////////////Nachenblaster:///////////////////////////////
    list<NachenBlaster*>::iterator Nachen;
    Nachen = myBlaster.begin();
        (*Nachen)->doSomething();
    if(!(*Nachen)->isAlive()){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
//////////////////////////Smallgons///////////////////////////////////
    list<Smallgon*>::iterator itSmallgon;
    itSmallgon = mySmallgons.begin();
    while (itSmallgon!=mySmallgons.end()) {
        //If it collides with the NachenBlaster
        if((*itSmallgon)->isCollide(*Nachen)){
            myExplosion.push_back(new Explosion((*itSmallgon)->getX(),(*itSmallgon)->getY(),this));
            increaseScore(250); //increase the Score
            (*Nachen)->setHitpoints(-5); //damage
            destroyedAlien++;
            playSound(SOUND_DEATH);
            (*itSmallgon)->setDead();
            delete (*itSmallgon);
            itSmallgon =  mySmallgons.erase(itSmallgon);
            curAliens--;
            cerr << "deleted a Smallgon!11111" <<endl;
            itSmallgon++;
            continue;
        }

        (*itSmallgon)->doSomething();
        //If it collides with the NachenBlaster
        if((*itSmallgon)->isCollide(*Nachen)){
            myExplosion.push_back(new Explosion((*itSmallgon)->getX(),(*itSmallgon)->getY(),this));
            increaseScore(250); //increase the Score
            (*Nachen)->setHitpoints(-5); //damage
            destroyedAlien++;
            playSound(SOUND_DEATH);
            (*itSmallgon)->setDead();
        }
        //Remove DEAD ones
        if(!(*itSmallgon)->isAlive()){
            delete (*itSmallgon);
            itSmallgon = mySmallgons.erase(itSmallgon);
            curAliens--;
            cerr << "deleted a Smallgon!222222" <<endl;
        }
        itSmallgon++;
    }

//////////////////////////Smoregons///////////////////////////////////
    list<Smoregon*>::iterator itSmoregon;
    itSmoregon = mySmoregons.begin();
    while (itSmoregon!=mySmoregons.end()) {
        if((*itSmoregon)->isCollide(*Nachen)){
            myExplosion.push_back(new Explosion((*itSmoregon)->getX(),(*itSmoregon)->getY(),this));
            increaseScore(250); //increase the Score
            (*Nachen)->setHitpoints(-5);//damage
            destroyedAlien++;
            playSound(SOUND_DEATH);
            (*itSmoregon)->setDead();
            //chance for a goodie
            if(randInt(1, 3)==1){
                if(randInt(1, 2)==1) newRepairGoodie(*itSmoregon);
                else newTorpedoGoodie(*itSmoregon);
            }
            delete (*itSmoregon);
            itSmoregon = mySmoregons.erase(itSmoregon);
            curAliens--;
            cerr << "deleted a Smoregon11111!" <<endl;
            itSmoregon++;
            continue;
        }
        (*itSmoregon)->doSomething();
        
        //Remove DEAD ones
        if(!(*itSmoregon)->isAlive()){
            delete (*itSmoregon);
            itSmoregon = mySmoregons.erase(itSmoregon);
            curAliens--;
            cerr << "deleted a Smoregon!22222" <<endl;
        }
        itSmoregon++;
    }
    
//////////////////////////Snagglegons/////////////////////////////////
    list<Snagglegon*>::iterator itSnagglegon;
    itSnagglegon = mySnagglegons.begin();
    while (itSnagglegon!=mySnagglegons.end()) {
        if((*itSnagglegon)->isCollide(*Nachen)){
            myExplosion.push_back(new Explosion((*itSnagglegon)->getX(),(*itSnagglegon)->getY(),this));
            increaseScore(1000); //increaset the Score
            //myBlaster suffer collisoin
            (*Nachen)->setHitpoints(-15);
            playSound(SOUND_DEATH);
            (*itSnagglegon)->setDead();
            destroyedAlien++;
            // 1/6 chance for extralife goodie:
            if(randInt(1, 6) == 1)
                newExtraLifeGoodie(*itSnagglegon);
            delete (*itSnagglegon);
            itSnagglegon = mySnagglegons.erase(itSnagglegon);
            curAliens--;
            cerr << "deleted a Snagglegon!1111" <<endl;
            itSnagglegon++;
            continue;
        }
        (*itSnagglegon)->doSomething();
        
        
        //Remove DEAD ones
        if(!(*itSnagglegon)->isAlive()){
            delete (*itSnagglegon);
            itSnagglegon = mySnagglegons.erase(itSnagglegon);
            curAliens--;
            cerr << "deleted a Snagglegon!2222" <<endl;
        }
        itSnagglegon++;
    }
    
    
///////////////////////New Alien Ships//////////////////////////////
    if(curAliens<(totalAlien-destroyedAlien) && curAliens<maxOnScreenAlien){
        int temp = randInt(1, S);
        if(temp <= s1){
            mySmallgons.push_back(new Smallgon(255, randInt(0, 255),this, curLevel,IID_SMALLGON,*myBlaster.begin()));
            curAliens++;
        }
        if(temp>s1 && temp <= s1+s2){
            mySmoregons.push_back(new Smoregon(255, randInt(0, 255),this, curLevel,IID_SMOREGON,*myBlaster.begin()));
            curAliens++;
        }
        if(temp > s1+s2){
            mySnagglegons.push_back(new Snagglegon(255, randInt(0, 255),this, curLevel,*myBlaster.begin()));
            curAliens++;
        }
    }
///////////////////////////Cabbages//////////////////////////////
    if((*Nachen)->getCabbageEnergy()<30)
        (*Nachen)->increaseCabbage();
    list<Cabbage*>::iterator itCabbages;
    itCabbages = myCabbages.begin();
    while(itCabbages!= myCabbages.end()){
        CheckCabbageCollides(*itCabbages);
        (*itCabbages)->doSomething();

        //Remove DEAD ones
        if(!(*itCabbages)->isAlive()){
            delete *itCabbages;
            itCabbages = myCabbages.erase(itCabbages);
        }
        itCabbages++;
    }
    
    
///////////////////////////Turnips//////////////////////////////
    list<Turnip*>::iterator itTurnips;
    itTurnips = myTurnips.begin();
    while (itTurnips!=myTurnips.end()) {
        if((*itTurnips)->isAlive()){
        if((*itTurnips)->isCollide(*Nachen)){
            myExplosion.push_back(new Explosion((*itTurnips)->getX(),(*itTurnips)->getY(),this));
            (*Nachen)->setHitpoints(-2);//damage
            if(!(*Nachen)->isAlive())//check whether dead
                playSound(SOUND_DEATH);
            playSound(SOUND_BLAST);
            (*itTurnips)->setDead();
            delete *itTurnips;
            itTurnips = myTurnips.erase(itTurnips);
            itTurnips++;
            continue;
        }
        (*itTurnips)->doSomething(); //DoSomething
        }
        //Remove DEAD ones
        if(!(*itTurnips)->isAlive()){
            delete *itTurnips;
            itTurnips = myTurnips.erase(itTurnips);
        }
        itTurnips++;
    }

///////////////////////////Torpedoes//////////////////////////////
    list<Torpedo*>::iterator itTorpedo;
    itTorpedo = myTorpedoes.begin();
    while(itTorpedo!= myTorpedoes.end()){
        CheckTorpedoCollides(*itTorpedo);
        (*itTorpedo)->doSomething(); //DoSomething
        
        //Remove DEAD ones
        if(!(*itTorpedo)->isAlive()){
            delete *itTorpedo;
            itTorpedo = myTorpedoes.erase(itTorpedo);
        }
        itTorpedo++;
    }
    
    
    
    
///////////////////////////Explosion//////////////////////////////
    list<Explosion*>::iterator itExplosion;
    itExplosion = myExplosion.begin();
    while(itExplosion!=myExplosion.end()){
        if((*itExplosion)->getTicks()>0){
            (*itExplosion)->doSomething();
            break;
        }
            cerr << "deleted an Explosion"<<endl;
            delete (*itExplosion);
            itExplosion =  myExplosion.erase(itExplosion);
        itExplosion++;
    }

    
///////////////////////////Goodies//////////////////////////////
    list<ExtraLife*>:: iterator itGoodie;
    itGoodie = myGoodies.begin();
    while(itGoodie!=myGoodies.end()){
        if((*itGoodie)->isCollide(*Nachen)){
            (*itGoodie)->setDead();
            increaseScore(100); //increase score by 100
            switch ((*itGoodie)->benifit()) {
                case 1:
                    incLives();
                    break;
                case 2:
                    (*Nachen)->setHitpoints(10);
                    break;
                case 3:
                    (*Nachen)->increaseTorpedo();
                default:
                    break;
            }
            playSound(SOUND_GOODIE);
            delete *itGoodie;
            itGoodie = myGoodies.erase(itGoodie);
            itGoodie++;
            continue;
        }
        (*itGoodie)->doSomething();

        
        //Remove DEAD ones
        if(!(*itGoodie)->isAlive()){
            delete *itGoodie;
           itGoodie = myGoodies.erase(itGoodie);
        }
        itGoodie++;
    }
    
    cerr <<"cur aliens " << curAliens <<endl;
    cerr <<"total alien "<< totalAlien << endl;
    cerr <<"destroyed alien "<< destroyedAlien << endl;

/////////////////////update text and checkscore/////////////////////////
    updateDisplayText();
    if(destroyedAlien>=totalAlien){
        curLevel++;
        return GWSTATUS_FINISHED_LEVEL;
    }
    
     //decLives();
    //return GWSTATUS_PLAYER_DIED;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::removeDeadObject(){
    
}


void StudentWorld::cleanUp()
{
    //clean up the stars
    while(myStars.size()!=0)
    {
        list<Stars*>::iterator it;  //????????????????????????
        it = myStars.begin();
        list<Stars*>::iterator itemp;
        itemp = it;
        it++;
        delete (*itemp);    //memory leak!!!!!!
        myStars.erase(itemp);
    }
    
    //clean the blaster
    if(myBlaster.size()!=0){
        delete (*myBlaster.begin());
        myBlaster.erase(myBlaster.begin());
    }
    
    //clean the smallgons
    while (mySmallgons.size()!=0) {
        list<Smallgon*>::iterator it;  //????????????????????????
        it = mySmallgons.begin();
        list<Smallgon*>::iterator itemp;
        itemp = it;
        it++;
        delete (*itemp);    //memory leak!!!!!!
        mySmallgons.erase(itemp);
    }
    
    //clean up the smoregons
    while(mySmoregons.size()!=0){
        list<Smoregon*>::iterator it;
        it = mySmoregons.begin();
        delete *it;
        it = mySmoregons.erase(it);
        it++;
    }
    //clean up the snagglegons
    while(mySnagglegons.size()!=0){
        list<Snagglegon*>::iterator it;
        it = mySnagglegons.begin();
        delete *it;
        it = mySnagglegons.erase(it);
        it++;
    }
    //clean up the explosion
    while(myExplosion.size()!=0){
        list<Explosion*>::iterator it;
        it = myExplosion.begin();
        delete *it;
        it = myExplosion.erase(it);
        it++;
    }
    //clean up the cabbage
    while(myCabbages.size()!=0){
        list<Cabbage*>::iterator it;
        it = myCabbages.begin();
        delete *it;
        it = myCabbages.erase(it);
        it++;
    }
    //clean up the turnip
    while(myTurnips.size()!=0){
        list<Turnip*>::iterator it;
        it = myTurnips.begin();
        delete *it;
        it = myTurnips.erase(it);
        it++;
    }
    //clean up the torpedoes
    while(myTorpedoes.size()!=0){
        list<Torpedo*>::iterator it;
        it = myTorpedoes.begin();
        delete *it;
        it = myTorpedoes.erase(it);
        it++;
    }
    //clean up the goodies
    while(myGoodies.size()!=0){
        list<ExtraLife*>::iterator it;
        it = myGoodies.begin();
        delete *it;
        it = myGoodies.erase(it);
        it++;
    }
}



StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::updateDisplayText(){
    //setGameStatText("Lives: 3  Health: 100%  Score: 0  Level: 3  Cabbages: 80%  Torpedoes: 4");
    //the Onscreen Text
    ostringstream oss;
    oss<<"Lives: "<<setw(1)<<getLives()<<"  Health: "<<setw(4)<<2*(*myBlaster.begin())->currentHitpoints()<<"%  Score: "<<setw(3)<<getScore()<<"  Level: "<<curLevel<<"  Cabbages: "<<setw(3)<<(*myBlaster.begin())->getCabbageEnergy()*100/30<<"%  Torpedoes: "<<(*myBlaster.begin())->getNumTorpedo();
    screenText = oss.str();
    setGameStatText(screenText);
    
    
}

void StudentWorld::newCabbage(){
    myCabbages.push_back(new Cabbage((*myBlaster.begin())->getX()+12,(*myBlaster.begin())->getY(),this,*myBlaster.begin(),2,8));
    playSound(SOUND_PLAYER_SHOOT);
}

void StudentWorld::newTurnip(Actor* alien){
    myTurnips.push_back(new Turnip(alien->getX()-14, alien->getY(),this, *myBlaster.begin(),2,-6));
}
void StudentWorld::newSnagTorpedo(Actor* actor){
    myTorpedoes.push_back(new Torpedo(actor->getX()-14,actor->getY(),this,*myBlaster.begin(),8,-8));
    list<Torpedo*>::iterator temp;
    temp = myTorpedoes.end(); temp--;
    (*temp)->setDirection(180);
}

void StudentWorld::newNachenTorpedo(){
    myTorpedoes.push_back(new Torpedo((*myBlaster.begin())->getX()+14,(*myBlaster.begin())->getY(),this,*myBlaster.begin(),8,8));
}

void StudentWorld::newExtraLifeGoodie(Actor* alien){
    myGoodies.push_back(new ExtraLife(alien->getX(),alien->getY(), this, IID_LIFE_GOODIE, *myBlaster.begin()));
}

void StudentWorld::newRepairGoodie(Actor* alien){
    myGoodies.push_back(new RepairGoodie(alien->getX(),alien->getY(), this, *myBlaster.begin()));
}


void StudentWorld::newTorpedoGoodie(Actor* alien){
    myGoodies.push_back(new FlatulanTor(alien->getX(),alien->getY(), this, *myBlaster.begin()));
}

void StudentWorld::CheckCabbageCollides(Cabbage* itCabbages){
    if(itCabbages->isAlive()){
    //if it Collide with any Alien
    //Smallgons?
    list<Smallgon*>::iterator itSmallgontemp;
    itSmallgontemp = mySmallgons.begin();
    while(itSmallgontemp!=mySmallgons.end()){
        if((itCabbages)->isCollide(*itSmallgontemp)){
            (itCabbages)->setHitpoints(0);
            (*itSmallgontemp)->setHitpoints(-2);
            if(!(*itSmallgontemp)->isAlive()){
                myExplosion.push_back(new Explosion((*itSmallgontemp)->getX(),(*itSmallgontemp)->getY(),this));
                playSound(SOUND_DEATH);
                destroyedAlien++;
                increaseScore(250); //increaset the Score
            }
            playSound(SOUND_BLAST);
        }
        itSmallgontemp++;
    }
    //Smoregons?
    list<Smoregon*>::iterator itSmoregontemp;
    itSmoregontemp = mySmoregons.begin();
    while(itSmoregontemp!=mySmoregons.end()){
        if((itCabbages)->isCollide(*itSmoregontemp)){
            (itCabbages)->setHitpoints(0);
            (*itSmoregontemp)->setHitpoints(-2);
            if(!(*itSmoregontemp)->isAlive()){
                myExplosion.push_back(new Explosion((*itSmoregontemp)->getX(),(*itSmoregontemp)->getY(),this));
                playSound(SOUND_DEATH);
                increaseScore(250); //increase the Score
                //chance for a goodie
                if(randInt(1, 3)==1){
                    if(randInt(1, 2)==1) newRepairGoodie(*itSmoregontemp);
                    else newTorpedoGoodie(*itSmoregontemp);
                }
                destroyedAlien++;

            }
            playSound(SOUND_BLAST);
        }
        itSmoregontemp++;
    }
    //Snagglegons?
    list<Snagglegon*>::iterator itSnagglegontemp;
    itSnagglegontemp = mySnagglegons.begin();
    while (itSnagglegontemp!=mySnagglegons.end()) {
        if((itCabbages)->isCollide(*itSnagglegontemp)){
            (itCabbages)->setHitpoints(0);
            (*itSnagglegontemp)->setHitpoints(-2);
            if(!(*itSnagglegontemp)->isAlive()){
                destroyedAlien++;
                myExplosion.push_back(new Explosion((*itSnagglegontemp)->getX(),(*itSnagglegontemp)->getY(),this));
                playSound(SOUND_DEATH);
                increaseScore(1000); //increase the Scored
                //chance for 1/6 extra life goodie
                if(randInt(1, 6) == 1)
                    newExtraLifeGoodie(*itSnagglegontemp);
            }
            playSound(SOUND_BLAST);
            
        }
        itSnagglegontemp++;
    }
    }
}


void StudentWorld::CheckTorpedoCollides(Torpedo* itTorpedo){
    if(itTorpedo->isAlive()){
    //for Snagglegon Torpedoes
    if((itTorpedo)->getSpeed()<0){
        if((itTorpedo)->isCollide(*myBlaster.begin())){
            myExplosion.push_back(new Explosion((itTorpedo)->getX(),(itTorpedo)->getY(),this));
            (*myBlaster.begin())->setHitpoints(-8);//damage
            if(!(*myBlaster.begin())->isAlive())//check whether dead
                playSound(SOUND_DEATH);
            playSound(SOUND_BLAST);
            (itTorpedo)->setDead();
        }
    }
    //For NachenBlaster Torpedoes
    if((itTorpedo)->getSpeed()>0){
        //if it Collide with any Alien
        //Smallgons?
        list<Smallgon*>::iterator itSmallgontempp;
        itSmallgontempp = mySmallgons.begin();
        while(itSmallgontempp!=mySmallgons.end()){
            if((itTorpedo)->isCollide(*itSmallgontempp)){
                (itTorpedo)->setDead();
                (*itSmallgontempp)->setHitpoints(-8);//damage
                if(!(*itSmallgontempp)->isAlive()){
                    destroyedAlien++;
                    myExplosion.push_back(new Explosion((*itSmallgontempp)->getX(),(*itSmallgontempp)->getY(),this));
                    playSound(SOUND_DEATH);
                    increaseScore(250); //increase the Score
                }
                playSound(SOUND_BLAST);
            }
            itSmallgontempp++;
        }
        //Smoregons?
        list<Smoregon*>::iterator itSmoregontempp;
        itSmoregontempp = mySmoregons.begin();
        while(itSmoregontempp!=mySmoregons.end()){
            if((itTorpedo)->isCollide(*itSmoregontempp)){
                (itTorpedo)->setDead();
                (*itSmoregontempp)->setHitpoints(-8);//damage
                if(!(*itSmoregontempp)->isAlive()){
                    destroyedAlien++;
                    myExplosion.push_back(new Explosion((*itSmoregontempp)->getX(),(*itSmoregontempp)->getY(),this));
                    playSound(SOUND_DEATH);
                    increaseScore(250); //increase the Score
                    //chance for a goodie
                    if(randInt(1, 3)==1){
                        if(randInt(1, 2)==1) newRepairGoodie(*itSmoregontempp);
                        newTorpedoGoodie(*itSmoregontempp);
                    }
                }
                playSound(SOUND_BLAST);
            }
            itSmoregontempp++;
        }
        //Snagglegons?
        list<Snagglegon*>::iterator itSnagglegontempp;
        itSnagglegontempp = mySnagglegons.begin();
        while (itSnagglegontempp!=mySnagglegons.end()) {
            if((itTorpedo)->isCollide(*itSnagglegontempp)){
                (itTorpedo)->setDead();
                (*itSnagglegontempp)->setHitpoints(-8);//damage
                if(!(*itSnagglegontempp)->isAlive()){
                    destroyedAlien++;
                    myExplosion.push_back(new Explosion((*itSnagglegontempp)->getX(),(*itSnagglegontempp)->getY(),this));
                    playSound(SOUND_DEATH);
                    increaseScore(1000); //increase the Score
                    //chance for 1/6 extra life goodie
                    if(randInt(1, 6) == 1)
                        newExtraLifeGoodie(*itSnagglegontempp);
                }
                playSound(SOUND_BLAST);
            }
            itSnagglegontempp++;
        }
    }
    }
}
