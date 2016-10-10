/**
 *
 * Person class header
 *
 * @author Paul Lafaurie
 * @version 1.0
 *
 */


#ifndef GESTION_THREADS_PERSON_H
#define GESTION_THREADS_PERSON_H
#define endx 0 //Fin en x
#define endy1 60 //Fin en y, c'est un intervalle
#define endy2 68


struct Grid;

#include <iostream>
using namespace std;

class Person
{
public:
    // Constructeurs
    Person();
    Person(int x, int y, int id);
    //Destructor
    ~Person();

    //Accesseurs et mutateurs
    void setX(int x);
    void setY(int y);
    void setArrived(bool b);
    int getX() const;
    int getY() const;
    int getID() const;
    bool getArrived() const;


    // Autres méthodes
    Person move(Grid &grid);
    /*int distance(const Person &P) const;
    Person milieu(const Person &P) const;

    void saisir();*/
    void afficher() const;

private:
    int x,y,id; //Coordonées sur la grille, identificateur pour ne pas avoir à parcourir toute la matrice pour chaque mouvement
    bool arrived; //Flag pour savoir si l'entité est arrivée au bout ou pas
};

struct Grid {//Structure contenant le champ sur lequel les 'Person' se deplacent, et la liste des gens sur ce champ
    int matrix[512][128] = {{0}};
    Person ppl[];
};

#endif //GESTION_THREADS_PERSON_H
