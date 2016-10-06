//
// Created by user on 06/10/16.
//

#ifndef GESTION_THREADS_PERSON_H
#define GESTION_THREADS_PERSON_H


#include <iostream>
using namespace std;

class Person
{
public:
    // Constructeurs
    Person();
    Person(double x, double y);

    //Accesseurs et mutateurs
    void setX(double x);
    void setY(double y);
    double getX() const;
    double getY() const;

    // Autres méthodes
    double distance(const Person &P) const;
    Person milieu(const Person &P) const;

    void saisir();
    void afficher() const;

private:
    double x,y;
};


#endif //GESTION_THREADS_PERSON_H
