#include <iostream>
#include <pthread.h>
#define num_threads 4

#include <string>


using namespace std;

#include "Person.h"


struct Args{
    int *zone;
    int id;
    Grid grid;
    bool *arrived;
};

static int nbr = 4; // p2 by default

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
    for(int i = 0; i<nbr; i++){
        if(!arr[i])
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
int execute(){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    bool arrived[nbr] = {false};
    Grid grid;

    init(grid,ptr);


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

int * init_t1(Grid &grid, int *ptr){
    int zones[nbr];
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
    return zones;
}

//Changes zone  if necessary
void zone_set(Args *arg, int index){
    if(arg->grid.ppl[index].getX()>255){
        if(arg->grid.ppl[index].getY()>63)
            arg->zone[index] = 3;
        else
            arg->zone[index] = 4;
    }else{
        if(arg->grid.ppl[index].getY()>63)
            arg->zone[index] = 2;
        else
            arg->zone[index] = 1;
    }
}

void *zone_mgmt(void *args){
    printf("Hello world\n");
    Args *arg = (Args *)args;
    printf("lets manage! %d", arg->id);
    while(!check(arg->arrived)) {
        for (int i = 0; i < nbr; i++) {
            if (arg->zone[i] == arg->id) {
                    if (!arg->arrived[i]) {
                        arg->grid.ppl[i] = arg->grid.ppl[i].move(arg->grid);
                        zone_set(arg, i);
                    }
                    printf("Person n° %d",i);
                    arg->grid.ppl[i].afficher();
                    if (arg->grid.ppl[i].getX() == endx &&
                        (arg->grid.ppl[i].getY() >= endy1 || arg->grid.ppl[i].getY() < endy2))
                        arg->arrived[i] = true;
            }
        }
    }
    pthread_exit(NULL);
}

pthread_t * init_threads(pthread_t threads[], Args *args){
    int rc;
    for( int i=0; i < num_threads; i++ ){
        cout << "main() : creating thread, " << i << endl;
        args->id =i;
        rc = pthread_create(&threads[i], NULL,
                            zone_mgmt, (void *)args);
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}




int main(int argc, char * argv[]) {

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
        nbr = 16;
    } else if (contains(argc, argv, "-p8")) // 256 people
    {
        nbr = 256;
    }

    cout << nbr << " " << metrics << " " << thread_mode;

    execute();

    return 0;
}

//t1 main
void four_threads(){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    bool arrived[nbr] = {false};
    Grid grid;
    int *zones = init_t1(grid,ptr);
    pthread_t threads[num_threads];

    Args arg;
    arg.zone = zones;
    arg.arrived = arrived;
    arg.grid = grid;

    init_threads(threads,&arg);


    for(int i = 0; i<num_threads; i++){
        pthread_join(threads[i],NULL);
        printf("Waiting");
    }

    printf("\nDone!");
}



int main()
{
    //execute();
    four_threads();
    return 0;
}
