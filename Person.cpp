/**
 *
 * Person class that represents a Person who will move on the grid
 *
 * @author Paul Lafaurie
 * @version 1.0
 *
 */

#include "Person.h"

#include <cmath>
#include <iostream>
using namespace std;
//Coords 0<x<511 && 0<y<127

//Person = 4x4 square, referenced by top left corner, positive y going down, and positive x going right


Person::Person() : x(0), y(0), id(0)
{}

Person::Person(int x, int y, int id) :x(x), y(y), id(id)
{
    this->arrived = false;
}

Person::~Person()
{}

void Person::setX(int x)
{
    this->x = x;
}

void Person::setY(int y)
{
    this->y = y;
}

void Person::setArrived(bool b)
{
    this->arrived = b;
}

int Person::getX() const
{
    return this->x;
}

int Person::getY() const
{
    return this->y;
}

int Person::getID() const
{
    return this->id;
}

bool Person::getArrived() const
{
    return this->arrived;
}


Person Person::move(Grid &grid)
{
    Person moved;
    moved.y = this->y;
    moved.x = this->x;
    moved.id = this->id;
    moved.arrived = this->arrived;
    //L'algo de deplacement est très simple, il vérifie en premier lieu si l'entité est arrivée (auquel cas pas de deplacement necessaire)
    if (this->x == endx && (this->y >= endy1 || this->y < endy2) && !this->arrived)
        moved.arrived = true;
    else if(this->y>=endy2 )//Sinon, l'entité va se placer en premier lieu vers le bon intervalle de coordonées en ordonnées
        moved.y = this->y-1;
    else if(this->y<endy1)
        moved.y = this->y+1;
    else//Si l'ordonnée est bonne, on a plus que bouger en abscice jusqu'a toucher au but
        moved.x = this->x - 1;

        grid.matrix[this->x][this->y] -= this->id;
        grid.matrix[moved.x][moved.y] += this->id;


    return moved;
}

//Fonction principalement utilisée pour du debeug, conjoitement avec GDB
void Person::afficher() const
{
    cout << "X vaut " << this->x << endl;
    cout << "Y vaut " << this->y << endl;
    cout << "Statut d'arrivee: " << this->arrived<<endl;
}