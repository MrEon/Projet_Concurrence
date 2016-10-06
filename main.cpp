#include <iostream>
#include <pthread.h>

using namespace std;

#include "Person.h"

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

void init(Grid &grid, int *ptr){
    for(int i = 0; i<nbr; i++){
        grid.ppl[i] = Person(ptr[0], ptr[1], i+1);
        grid.matrix[ptr[0]][ptr[1]] = true;//PPl are 1x1 for now
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

void four_threads(){
    int coord[] = {506, 2};
    int *ptr = coord;
    //Exit condition && tells the algorithm when to stop moving the ones already there to focus on the ppl still on the field
    bool arrived[nbr] = {false};
    Grid grid;

    init(grid,ptr);

    //initiate threads, make a function to cut grid in 4

    while(!check(arrived)){
        //invoke threaded function
    }
    printf("\nDone!");
}



int main()
{
    execute();
    return 0;
}
