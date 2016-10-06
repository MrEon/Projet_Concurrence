//
// Created by user on 06/10/16.
//

#include "Person.h"

#include <cmath>
#include <iostream>
using namespace std;
//Coords 0<x<511 && 0<y<127

//Person = 4x4 square, referenced by top left corner, positive y going down, and positive x going right


Person::Person() : x(0), y(0)
{}

Person::Person(int x, int y)/* : x(x), y(y)*/
{
    this->x = x;
    this->y = y;
}

void Person::setX(int x)
{
    this->x = x;
}

void Person::setY(int y)
{
    this->y = y;
}

int Person::getX() const
{
    return this->x;
}

int Person::getY() const
{
    return this->y;
}

Person Person::move()
{
    Person moved;
    moved.y = this->y;
    moved.x = this->x;
    if(this->y>=endy2 )
        moved.y = this->y-1;
    else if(this->y<endy1)
        moved.y = this->y+1;
    else
        moved.x = this->x-1;


    return moved;
}
/*int Person::distance(const Person &P) const
{
    int dx = this->x - P.x;
    int dy = this->y - P.y;
    return sqrt(dx*dx + dy*dy);
}

Person Person::milieu(const Person &P) const
{
    Person result;
    result.x = (P.x + this->x) / 2;
    result.y = (P.y + this->y) / 2;
    return result;
}

void Person::saisir()
{
    cout << "Tapez l'abscisse : ";  cin >> this->x;
    cout << "Tapez l'ordonnée : ";  cin >> this->y;
}
*/
void Person::afficher() const
{
    cout << "X vaut " << this->x << endl;
    cout << "Y vaut " << this->y << endl;
}