/**
 * Main file of the project
 *
 * @author Renaud Costa
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


struct Args{
    int nbr;
    int *zone;
    int id;
    Grid *grid;
    vector<bool> arrived;
};

//Produces the next coordinates to use for spawning purpouses
int* nextCoord(int *arr){
    int tmp = arr[1];
    arr[1] = (arr[1]+8)%128;
    if(tmp>arr[1] && arr[0]>127 )
        arr[0]-=8;

    return arr;
}
//Checks if every one has reached the exit
bool check(vector<bool> arr){
    for(int j = 0; j < sizeof(arr)/sizeof(bool); j++){
        if(!arr[j])
            return false;
    }
    return true;
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

void init(Grid &grid, int *ptr, int nbr){
    for(int i = 0; i<nbr; i++){
        grid.ppl[i] = Person(ptr[0], ptr[1], i+1);
        grid.matrix[ptr[0]][ptr[1]] = i+1;//PPl are 1x1 for now
        ptr = nextCoord(ptr);
    }
}

//The heart of the exectution process
int execute(int nbr){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    vector<bool> arrived = {false};
    Grid grid;
    cout << "test";
    init(grid,ptr, nbr);

    while(!check(arrived)){
        for(int i = 0; i<nbr; i++){
            if(!arrived[i])
                grid.ppl[i] = grid.ppl[i].move(grid);

            if(grid.ppl[i].getX() == endx && (grid.ppl[i].getY() >= endy1 || grid.ppl[i].getY() < endy2))
                arrived[i] = true;
        }
    }
    printf("\nDone!");
}

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

//Changes zone  if necessary
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

void *zone_mgmt(void *args){
    printf("hello\n");
    Args *arg = (Args *)args;
    int id = arg->id;
    printf("Hello from %d\n", id);
    while(!check(arg->arrived)) {
        for (int i = 0; i < arg->nbr; i++) {
           // printf("in da 4, ID: %d, Looking at person in  zone: %d\n", id, arg->zone[i]);
            if (arg->zone[i] == id ) {
                    if (!arg->arrived[i]) {
                        arg->grid->ppl[i] = arg->grid->ppl[i].move(*arg->grid);
                        zone_set(arg, i);
                    }
                    printf("Person n° %d, zone: %d\n",i, id);
                    arg->grid->ppl[i].afficher();
                    if (arg->grid->ppl[i].getX() == endx &&
                        (arg->grid->ppl[i].getY() >= endy1 || arg->grid->ppl[i].getY() < endy2))
                        arg->arrived[i] = true;
            }
        }
    }
    pthread_exit(NULL);
}

pthread_t * init_threads(pthread_t threads[], Args *args){

}


//t1 main
void four_threads(int nbr){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    vector<bool> arrived = {false};
    Grid grid;
    int zones[nbr];
    init_t1(grid,ptr, zones,nbr);
    pthread_t threads[num_threads];

    Args arg;
    arg.zone = zones;
    arg.arrived = arrived;
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
    for(int i = num_threads; i>0; i--){
        printf("Now we wait for %d\n", i);
        pthread_join(threads[i-1],NULL);
    }
    pthread_exit(NULL);
    printf("\nDone!");
}

//t2 main
void nbr_threads(int nbr){

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



    switch (thread_mode)
    {
        case 0:
            execute(nbr);
            break;
        case 1:
            four_threads(nbr);
            break;
        case 2:
            nbr_threads(nbr);
            break;
    }

    if (metrics)
    {
        cout << average_time(thread_mode, nbr);
    }

    return 0;
}
