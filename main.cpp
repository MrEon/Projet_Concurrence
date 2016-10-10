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
#define nbr 4
#include <string>

using namespace std;

#include "Person.h"


struct Args{
    int *zone;
    int id;
    Grid *grid;
    bool *arrived;
};

int numbr = nbr; // p2 by default

//Produces the next coordinates to use for spawning purpouses
int* nextCoord(int *arr){
    int tmp = arr[1];
    arr[1] = (arr[1]+8)%128;
    if(tmp>arr[1] && arr[0]>127 )
        arr[0]-=8;

    return arr;
}
//Checks if every one has reached the exit
bool check(bool arr[]){
    for(int j = 0; j<nbr ; j++){
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

void init(Grid &grid, int *ptr){
    for(int i = 0; i<nbr; i++){

        grid.ppl[i] = Person(ptr[0], ptr[1], i+1);
        grid.matrix[ptr[0]][ptr[1]] = i+1;//PPl are 1x1 for now
        ptr = nextCoord(ptr);
    }
}

//The heart of the exectution process
void execute(){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    bool arrived[nbr] = {false};
    Grid grid;
    init(grid,ptr);

    while(!check(arrived)){
        for(int i = 0; i<nbr; i++) {
            if (!arrived[i])
                grid.ppl[i] = grid.ppl[i].move(grid);

            if (grid.ppl[i].getX() == endx && (grid.ppl[i].getY() >= endy1 || grid.ppl[i].getY() < endy2) &&
                !arrived[i]) {
            arrived[i] = true;
                printf("Person %d has arrived!\n",grid.ppl[i].getID());
            }
        }
    }
    printf("Freeing memory\n");
    for(int i = 0; i<nbr; i++)
        grid.ppl[i].~Person();

    for(int i = 0; i<512; i++)
        free(grid.matrix[i]);


    printf("Done!\n");

}

void init_t1(Grid &grid, int *ptr, int *zones ){
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
    Args *arg = (Args *)args;
    int id = arg->id;
    printf("Hello from %d\n", id);
    while(!check(arg->arrived)) {
        for (int i = 0; i < nbr; i++) {
           // printf("in da 4, ID: %d, Looking at person in  zone: %d\n", id, arg->zone[i]);
            if (arg->zone[i] == id ) {
                    if (!arg->arrived[i]) {
                        arg->grid->ppl[i] = arg->grid->ppl[i].move(*arg->grid);
                        zone_set(arg, i);
                    }
                    printf("Person n° %d, zone: %d\n",i, id);
                    arg->grid->ppl[i].afficher();
                    if (arg->grid->ppl[i].getX() == endx &&
                        (arg->grid->ppl[i].getY() >= endy1 || arg->grid->ppl[i].getY() < endy2) && !arg->arrived[i])
                        arg->arrived[i] = true;
            }
        }
    }
    pthread_exit(NULL);
}



//t1 main
void four_threads(){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    bool arrived[nbr] = {false};
    Grid grid;
    int zones[nbr];
    init_t1(grid,ptr, zones);
    pthread_t threads[num_threads];

    Args arg;
    arg.zone = zones;
    arg.arrived = arrived;
    arg.grid = &grid;

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


void *many_threads(void *args){
    Args *arg = (Args *)args;
    int id = arg->id;
    while(arg->arrived[id]){
        arg->grid->ppl[id] = arg->grid->ppl[id].move(*arg->grid);
        if(arg->grid->ppl[id].getX() == endx &&
           (arg->grid->ppl[id].getY() >= endy1 || arg->grid->ppl[id].getY() < endy2) &&
           !arg->arrived[id])
            arg->arrived[id] = true;
    }
    cout << id<< " has arrived"<<endl;
    //pthread_exit(NULL);
}


void to_each_a_thread(){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    bool arrived[nbr] = {false};
    Grid grid;
    init(grid,ptr);
    pthread_t threads[nbr];

    Args arg;
    arg.arrived = arrived;
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





int main(int argc, char * argv[]) {
    /*
    bool metrics = false; // off by default
    int thread_mode = 1; // t1 by default

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

    if (contains(argc, argv, "-p4")) // 16 people
    {
        numbr = 16;
    } else if (contains(argc, argv, "-p8")) // 256 people
    {
        numbr = 256;
    }

    cout << nbr << " " << metrics << " " << thread_mode;
    */
    //execute();
    to_each_a_thread();
    //four_threads();
    return 0;
}
