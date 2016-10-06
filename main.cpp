#include <iostream>
#include <string>


using namespace std;

#include "Person.h"
int* nextCoord(int *arr){
    int tmp = arr[1];
    arr[1] = (arr[1]+8)%128;
    if(tmp>arr[1] && arr[0]>127 )
        arr[0]-=8;

    return arr;
}

bool contains(int argc, char * argv [], string arg)
{
    for (int i = 0; i < argc; i++)
    {
        if (arg.compare(argv[i]) == 0)
            return true;
    }
    return false;
}

int main(int argc, char * argv[])
{

    bool metrics = false; // off by default
    int nbr = 4; // p2 by default
    int thread_mode = 1; // t1 by default

    if (contains(argc, argv, "-t0")) // t0
    {
        // ONE THREAD
        thread_mode = 0;
    }
    else if (contains(argc, argv, "-t2")) // t2
    {
        // ONE THREAD PER PERSON
        thread_mode = 2;
    }

    if (contains(argc, argv, "-m"))
    {
        //METRICS
        metrics = true;
    }

    if (contains(argc, argv, "-p4")) // 16 people
    {
        nbr = 16;
    }
    else if (contains(argc, argv, "-p8")) // 256 people
    {
        nbr = 256;
    }

    cout << nbr << " " << metrics << " " << thread_mode;


    int coord[] = {506, 2};
    int *ptr = coord;
    Person ppl[nbr];
    for(int i = 0; i<nbr; i++){
        ppl[i] = Person(ptr[0], ptr[1]);
        ptr = nextCoord(ptr);
    }

    bool arrived = false;
    printf("Bufferring...");
    while(!arrived){
        for(int i = 0; i<nbr; i++){
            ppl[i] = ppl[i].move();
            //printf("Personne %d\n", i);
            //ppl[i].afficher();
            if(ppl[i].getX() == endx && (ppl[i].getY() >= endy1 || ppl[i].getY() < endy2))
                arrived = true;
            else
                arrived = false;
        }
        //printf(".");
    }
    printf("\nDone!");
    /*Person A, B, C;
    double d;

    cout << "SAISIE DU POINT A" << endl;
    A.saisir();
    cout << endl;

    cout << "SAISIE DU POINT B" << endl;
    B.saisir();
    cout << endl;

    C = A.milieu(B);
    d = A.distance(B);

    cout << "MILIEU DE AB" << endl;
    C.afficher();
    cout << endl;

    cout << "La distance AB vaut : " << d << endl;*/

    return 0;
}


