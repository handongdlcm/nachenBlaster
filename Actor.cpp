#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//////////////////////////Actor:///////////////////////////////////
bool Actor::isAlive() {
    if(!m_alive)return false;
    if(currentHitpoints()>0) return true;
    else return false;
}

int Actor::currentHitpoints() const{
    return m_hitPoints;
}

StudentWorld* Actor::getMyWorld() const {
    return m_World;
}

bool Actor::isCollide(Actor* actor){
    //whether it collide with NachenBlasters
    double XX = (getX()-actor->getX())*(getX()-actor->getX());
    double YY = (getY()-actor->getY())*(getY()-actor->getY());
    double RR = 0.75*(getRadius()+actor->getRadius());
    if(XX + YY < RR * RR){
        return true;
    }
    else
        return false;
}

//////////////////////////Star:////////////////////////////////////
void Stars::doSomething()
{
    moveTo(getX()-1, getY());
}



//////////////////////////Nachenblaster:///////////////////////////////


void NachenBlaster::setHitpoints(int num)
{
    m_hitPoints = m_hitPoints+num;
}

void NachenBlaster::doSomething()
{
    if(isAlive())
    {
        int userInput;
        if(getMyWorld()->getKey(userInput)){
            switch (userInput) {
                case KEY_PRESS_LEFT:
                    if(getX()>0) moveTo(getX()-6, getY());
                    break;
                case KEY_PRESS_RIGHT:
                    if(getX()<255) moveTo(getX()+6, getY());
                    break;
                case KEY_PRESS_UP:
                    if(getY()<255) moveTo(getX(), getY()+6);
                    break;
                case KEY_PRESS_DOWN:
                    if(getY()>0) moveTo(getX(), getY()-6);
                    break;
                case KEY_PRESS_SPACE:
                    if(getCabbageEnergy()>5){
                        getMyWorld()->newCabbage();
                        m_cabbageEnergy = m_cabbageEnergy-5;
                    }
                    break;
                case KEY_PRESS_TAB:
                    if(getNumTorpedo()>0){
                        getMyWorld()->newNachenTorpedo();
                        getMyWorld()->playSound(SOUND_TORPEDO);
                        m_numTorpedo--;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return;
}



//////////////////////////Smallgon://////////////////////////////////

Smallgon::~Smallgon(){
    
}

bool Smallgon::isAlive() {
    if(!m_alive) return false;
    if(getX()<0) return false;  // fly over the left screen
    if(currentHitpoints()>0) return true;
    else return false;
}

bool Smallgon::FireorSprint(){
    if(getMyBlaster()->getX()<getX()
       && getY()>=getMyBlaster()->getY()-4
       && getY()<=getMyBlaster()->getY()+4)
    {
        int temp = randInt(1, 20/getCurLevel()+5);
        if(temp==1)
        {
            //the Fire Implementation
            getMyWorld()->newTurnip(this);
            getMyWorld()->playSound(SOUND_ALIEN_SHOOT);
            return true;
        }
        return false;
    }
    return false;
}

void Smallgon::doSomething(){
    if(isAlive())
    {
        if(FireorSprint()) return;
        letItMove();
    }
    return;
}


void Smallgon::letItMove(){
    if(m_distance == 0){
        switch (randInt(1, 3)) {
            case 1:
                dirIndex = 1;
                break;
            case 2:
                dirIndex = 2;
                break;
            case 3:
                dirIndex = 3;
                break;
            default:
                break;
        }
    }
    m_distance = randInt(1, 32);
    if(getY()>=255) dirIndex = 3;
    if(getY()<=0) dirIndex = 1;
    switch (dirIndex) {
        case 1:
            moveTo(getX()-m_speed, getY()+m_speed);
            m_distance = m_distance-1;
            break;
        case 2:
            moveTo(getX()-m_speed, getY());
            m_distance = m_distance-1;
            break;
        case 3:
            moveTo(getX()-m_speed, getY()-m_speed);
            m_distance = m_distance-1;
            break;
        default:
            break;
    }
    
}



NachenBlaster* Smallgon::getMyBlaster() const{
    return m_Blaster;
}

int Smallgon::getDirIndex() const{
    return dirIndex;
}

void Smallgon::setDirIndex(int dir){
    dirIndex = dir;
}

int Smallgon::getDistance() const {
    return m_distance;
}

void Smallgon::setDistance(int dis){
    m_distance = dis;
}

int Smallgon::getCurLevel() const {
    return currentLev;
}

double Smallgon::getSpeed() const{
    return m_speed;
}

void Smallgon::setSpeed(double speed) {
    m_speed = speed;
}





//////////////////////////Smoregon://////////////////////////////////

bool Smoregon::FireorSprint(){
    if(getMyBlaster()->getX()<getX()
       && getY()>=getMyBlaster()->getY()-4
       && getY()<=getMyBlaster()->getY()+4)
    {
        int temp = randInt(1, 20/getCurLevel()+5);
        if(temp==1 || temp == 2)
        {
            if(temp == 1){
                //the Fire Implementation
                getMyWorld()->newTurnip(this);
                getMyWorld()->playSound(SOUND_ALIEN_SHOOT);
                return true;
            }
            //the Sprint for Smoregon
            setDirIndex(2);
            setDistance(VIEW_WIDTH);
            isSprint = true;
            return true;
        }
        return false;
    }
    return false;
}

void Smoregon::doSomething(){
    if(isAlive())
    {
        if(isSprint) {Sprint();}
        else
        if(FireorSprint()) return;
        letItMove();
    }
    return;
}

void Smoregon::Sprint(){
    
    if(getDistance()>0){
        moveTo(getX()-5, getY());
        setDistance(getDistance()-5);
    }
    else isSprint = false;
}




//////////////////////////Snagglegon://///////////////////////////////

void Snagglegon::letItMove(){
    if(getY()>=255)  setDirIndex(3);
    if(getY()<=0) setDirIndex(1);
        switch (getDirIndex()) {
            case 1:
                moveTo(getX()-getSpeed(), getY()+getSpeed());
                break;
            case 3:
                moveTo(getX()-getSpeed(), getY()-getSpeed());
            default:
                break;
        }
    
}

void Snagglegon::doSomething(){
    if(isAlive())
    {
        if(FireorSprint()) return;
        letItMove();
    }
    return;
}

bool Snagglegon::FireorSprint(){
    if(getMyBlaster()->getX()<getX()
       && getY()>=getMyBlaster()->getY()-4
       && getY()<=getMyBlaster()->getY()+4)
    {
        int temp = randInt(1, 15/getCurLevel()+10);
        if(temp==1)
        {
            //the Fire Implementation
            getMyWorld()->newSnagTorpedo(this);
            getMyWorld()->playSound(SOUND_TORPEDO);
            return true;
        }
        return false;
    }
    return false;
}




//////////////////////////Projectile/////////////////////////////////

NachenBlaster* Projectile::getMyBlaster() const {
    return m_Blaster;
}

bool Projectile::isAlive() {
    if(!m_alive) return false;
    if(getX()>255) return false;  // fly over the right screen
    if(currentHitpoints()>0) return true;
    else return false;
}

void Projectile::letItMove()
{
    moveTo(getX()+m_speed, getY());
    setDirection(20+getDirection());
}

void Projectile::doSomething(){
    if(isAlive())
    {
        letItMove();
    }
    return;
}

//////////////////////////Turnip/////////////////////////////////

bool Turnip::isAlive(){
    if(!m_alive) return false;
    if(getX()<0) return false;  // fly over the left screen
    if(currentHitpoints()>0) return true;
    else return false;
}

//////////////////////////Torpedo/////////////////////////////////

bool Torpedo::isAlive(){
    if(!m_alive) return false;
    if(m_speed>0&&getX()>255)
        return false;
    if(m_speed<0&&getX()<0)
        return false;
    if(currentHitpoints()>0) return true;
    else return false;
}

void Torpedo::letItMove(){
    moveTo(getX()+m_speed, getY());
}

//////////////////////////ExtraLife Goodie///////////////////////////////
bool ExtraLife::isAlive(){
    if(!m_alive) return false;
    if(getX()<0 || getY()<0) return false;
    if(currentHitpoints()>0) return true;
    else return true;
}

void ExtraLife::doSomething(){
    if(isAlive()){
        letItMove();
    }
    return;
}


void ExtraLife::letItMove(){
    moveTo(getX()-0.75, getY()-0.75);
}
















