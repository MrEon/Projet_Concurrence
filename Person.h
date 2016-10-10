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
#define endx 0
#define endy1 60
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
    int getX() const;
    int getY() const;
    int getID() const;


    // Autres méthodes
    Person move(Grid &grid);
    /*int distance(const Person &P) const;
    Person milieu(const Person &P) const;

    void saisir();*/
    void afficher() const;

private:
    int x,y,id;
    bool arrived;//faudrait utiliser ça plutot!! et pas le vector de bool
};

struct Grid {
    int matrix[512][128] = {{0}};
    Person ppl[];
};

#endif //GESTION_THREADS_PERSON_H
