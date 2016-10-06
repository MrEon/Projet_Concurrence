#include <iostream>
#define nbr 8
using namespace std;

#include "Person.h"
int* nextCoord(int *arr){
    int tmp = arr[1];
    arr[1] = (arr[1]+8)%128;
    if(tmp>arr[1] && arr[0]>127 )
        arr[0]-=8;

    return arr;
}

int main()
{
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
            printf("Personne %d\n", i);
            ppl[i].afficher();
            if(ppl[i].getX() == endx && (ppl[i].getY() >= endy1 || ppl[i].getY() < endy2))
                arrived = true;
            else
                arrived = false;
        }
        printf(".");
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
