//
// Created by user on 06/10/16.
//

#ifndef GESTION_THREADS_PERSON_H
#define GESTION_THREADS_PERSON_H
#define endx 0
#define endy1 60
#define endy2 68
#include <iostream>
using namespace std;

class Person
{
public:
    // Constructeurs
    Person();
    Person(int x, int y);

    //Accesseurs et mutateurs
    void setX(int x);
    void setY(int y);
    int getX() const;
    int getY() const;

    // Autres méthodes
    Person move();
    /*int distance(const Person &P) const;
    Person milieu(const Person &P) const;

    void saisir();*/
    void afficher() const;

private:
    int x,y;
};


#endif //GESTION_THREADS_PERSON_H
