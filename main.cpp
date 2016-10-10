/**
 * Main file of the project
 *
 * @author Renaud Costa Paul Lafaurie
 * @version 1.0
 *
 */

#include <iostream>
#include <pthread.h>
#define num_threads 4
#include <string>
#include <vector>


using namespace std;

#include "Person.h"

//Structure de données contenant les arguments passés aux threads lors de leur creation
struct Args{
    int nbr; //Nombre d'entité sur le champ (l'option -p[0..9] )
    int *zone;// Champ uniquement utilisé pour -t1, pour determiner quelle entité se situe dans quelle zone à l'instant t
    int id; //Identificateur du thread, c'est son "numero de zone" avec -t1, et cela correspond à l'id de la Person qu'il gère en -t2
    Grid *grid; //Structure Grid, centrale à l'exectution, partagée par tout les threads (plus de details dans Person.h)
};



//Fonction de génération des prochaines coordonées de spawn, puisqu'on souhaite pour le moment avoir chaque Person partant d'un même endroit fixe
int* nextCoord(int *arr){
    int tmp = arr[1];
    arr[1] = (arr[1]+8)%128;
    if(tmp>arr[1] && arr[0]>127 )
        arr[0]-=8;

    return arr;
}
//Verifie si toute les Person sont arrivées, c'est la condition de sortie de chaque fonction
bool check(Grid *grid, int nbr){
    for(int j = 0; j < nbr ; j++){
        if(!grid->ppl[j].getArrived())
            return false;
    }
    return true;
}


//Pour savoir si un argument est present dans le tableau des paramètres
bool contains(int argc, char * argv [], string arg)
{
    for (int i = 0; i < argc; i++)
    {
        if (arg.compare(argv[i]) == 0)
            return true;
    }
    return false;
}


//initialise la structure Grid à 0 partout, et prend pour valeur l'id des personnes presentes sur chaque case
void init(Grid &grid, int *ptr, int nbr){
    for(int i = 0; i<nbr; i++){

        grid.ppl[i] = Person(ptr[0], ptr[1], i+1);
        grid.matrix[ptr[0]][ptr[1]] = i+1;//PPl are 1x1 for now
        ptr = nextCoord(ptr);
    }
}

//Le coeur de l'execution de -t0
int execute(int nbr){
    int coord[] = {506, 2};
    int *ptr = coord;
    Grid grid;
    cout << "test";
    init(grid,ptr, nbr);

    while(!check(&grid, nbr)){
        for(int i = 0; i<nbr; i++) {
            if (!grid.ppl[i].getArrived())
                grid.ppl[i] = grid.ppl[i].move(grid);
        }
    }


    printf("Freeing memory\n");
    for(int i = 0; i<nbr; i++)
        grid.ppl[i].~Person();

    for(int i = 0; i<512; i++)
        free(grid.matrix[i]);


    printf("Done!\n");

}

//initialisation particulière pour -t1, qui demande un syteme de zones un peu particulier à mettre en oeuvre (il faut placer les Person dès l'init)
void init_t1(Grid &grid, int *ptr, int *zones , int nbr){
    for(int i = 0; i<nbr; i++){
        grid.ppl[i] = Person(ptr[0], ptr[1], i+1);
        grid.matrix[ptr[0]][ptr[1]] = i+1;//PPl are 1x1 for now
        if(ptr[0]>255){
            if(ptr[1]>63)
                zones[i] = 3;
            else
                zones[i] = 4;
        }else{
            if(ptr[1]>63)
                zones[i] = 2;
            else
                zones[i] = 1;
        }
        ptr = nextCoord(ptr);
    }
}

//Verifie et change au cas echeant la zone dans laquelle est repertoriée chaque entité de Person
void zone_set(Args *arg, int index){
    if(arg->grid->ppl[index].getX()>255){
        if(arg->grid->ppl[index].getY()>63)
            arg->zone[index] = 3;
        else
            arg->zone[index] = 4;
    }else{
        if(arg->grid->ppl[index].getY()>63)
            arg->zone[index] = 2;
        else
            arg->zone[index] = 1;
    }
}

//Fonction exectutée par les threads en -t1
void *zone_mgmt(void *args){
    Args *arg = (Args *)args;
    int id = arg->id;
    while(!check(arg->grid, arg->nbr)) {
        for (int i = 0; i < arg->nbr; i++) {
            if (arg->zone[i] == id ) {//Pour savoir si le  thread doit affecter ou pas une Person,
                // il regarde si l'inidice correspondant à la Person dans l'array zone si elle est repertoriée dans la bonne zone
                    if (!arg->grid->ppl[i].getArrived()) {
                        arg->grid->ppl[i] = arg->grid->ppl[i].move(*arg->grid);
                        zone_set(arg, i);
                    }

            }
        }
    }
    pthread_exit(NULL);
}



//t1 main
void four_threads(int nbr){
    int coord[] = {506, 2};
    int *ptr = coord;
    Grid grid;
    int zones[nbr];
    init_t1(grid,ptr, zones,nbr);
    pthread_t threads[num_threads];//Tableau contenant les differents threads crée ci-après

    Args arg;
    arg.zone = zones;
    arg.grid = &grid;
    arg.nbr = nbr;

    Args tab_arg[num_threads];

    //init threads
    int rc;
    for( int i=0; i < num_threads; i++ ){
        tab_arg[i] = arg;
        tab_arg[i].id = i+1;
        cout << "main() : creating thread, " << i+1 << endl;
        rc = pthread_create(&threads[i], NULL,
                            zone_mgmt, (void *)&tab_arg[i]);
        printf("created!\n");
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }


    for(int i = num_threads; i>0; i--)
        pthread_join(threads[i-1],NULL);

    cout<<"Done!"<<endl;
    pthread_exit(NULL);
}

//Fonction executée par chaque thread dans -t2
void *many_threads(void *args){
    Args *arg = (Args *)args;
    int id = arg->id;
    while(arg->grid->ppl[id].getArrived()){
        arg->grid->ppl[id] = arg->grid->ppl[id].move(*arg->grid);
    }
    cout << id<< " has arrived"<<endl;
    //pthread_exit(NULL);
}

//t2 main
void to_each_a_thread(int nbr){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    Grid grid;
    init(grid,ptr, nbr);
    pthread_t threads[nbr];

    Args arg;
    arg.grid = &grid;

    Args tab_arg[nbr];

    //init threads
    int rc;
    for( int i=0; i < nbr; i++ ){
        tab_arg[i] = arg;
        tab_arg[i].id = i;
        cout << "main() : creating thread, " << i+1 << endl;
        rc = pthread_create(&threads[i], NULL,
                            many_threads, (void *)&tab_arg[i]);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}







//metrics
int average_time(int thread_mode, int nbr)
{
    long double t = time(0);
    int results [5];
    switch (thread_mode)
    {
        case 0:
            int a = 0;
            for (int i = 0; i < 5; i++)
            {
                execute(nbr);
            }
    }
}

int main(int argc, char * argv[]) {


    bool metrics = false; // off by default
    int thread_mode = 1; // t1 by default
    int nbr = 16; // p4 by default

    if (contains(argc, argv, "-t0")) // t0
    {
        // ONE THREAD
        thread_mode = 0;
    } else if (contains(argc, argv, "-t2")) // t2
    {
        // ONE THREAD PER PERSON
        thread_mode = 2;
    }

    if (contains(argc, argv, "-m")) {
        //METRICS
        metrics = true;
    }

    if (contains(argc, argv, "-p2")) // 4 people
    {
        nbr = 4;
    } else if (contains(argc, argv, "-p8")) // 256 people
    {
        nbr = 256;
    }

    cout << nbr << " " << metrics << " " << thread_mode;


    cout << "NUMBER: " << nbr;
    //return 0;

    switch (thread_mode)
    {
        case 0:
            execute(nbr);
            break;
        case 1:
            four_threads(nbr);
            break;
        case 2:
            to_each_a_thread(nbr);
            break;
    }

    if (metrics)
    {
        cout << average_time(thread_mode, nbr);
    }

    return 0;
}
