//
// Created by user on 06/10/16.
//

#include "Person.h"

#include <cmath>
#include <iostream>
using namespace std;

Person::Person() : x(0), y(0)
{}

Person::Person(double x, double y) : x(x), y(y)
{}

void Person::setX(double x)
{
    this->x = x;
}

void Person::setY(double y)
{
    this->y = y;
}

double Person::getX() const
{
    return this->x;
}

double Person::getY() const
{
    return this->y;
}

double Person::distance(const Person &P) const
{
    double dx = this->x - P.x;
    double dy = this->y - P.y;
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

void Person::afficher() const
{
    cout << "L'abscisse vaut " << this->x << endl;
    cout << "L'ordonnée vaut " << this->y << endl;
}