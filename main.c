#pragma once
#include "database.h"
#include "mymachinelearning.h"
#include "gameanddialog.h"
#include <windows.h>

#define USERDATA "userdata.csv"
#define MODELDATA "modeldata.csv"

int main(void){

    //variable
    char username[20];
    int userchoice = 100;
    int x[100];
    int y[100];
    float *w, *b, *z, *a, *dw, *db, *dz, *da;
    int numberofnode[4] = {2, 3, 2, 1};
    int numberoflayer = 4;
    generatedatabase(4, numberofnode, &w, &b, &z, &a, &dz, &da); 

    //initiate
    FILE *file = fopen(USERDATA, "r");
    if(file == NULL){
        initweights(4, numberofnode, w, b, (int)time(NULL));
        file = fopen(USERDATA, "w");
        fprintf(file, "Name, Round\n");
        printf("Mastermind\t: Welcome! You must be new here, am I right?\n");
        Sleep(3000);
        printf("Mastermind\t: Can you tell me your name?\n");
        Sleep(1000);
        printf("You\t\t: My name is ");
        scanf("%s", username);
        fclose(file);
        addname(USERDATA, username);

        initialgame(username);

    }else{
        fclose(file);
        loadmodel(MODELDATA, w, 14, b, 6);
        getname(USERDATA, username);
        printf("Mastermind\t: Welcome back, %s\n", username);
    }

    //play
    printf("Mastermind\t: What do you want to play?\n");
    printf("You\t\t: I would like to ...\n");
    printf("[1] ... play Prisoner's Dilemma\n");
    printf("[2] ... forget all the things we've spend together\n");
    printf(">> ");
    scanf("%d", &userchoice);

    switch(userchoice){
        case 1:
        break;
        case 2:
            printf("You\t\t: I would like to forget all the things we've spend together\n");
            printf("Mastermind\t: 'Ight\n");
            remove(USERDATA);
            remove(MODELDATA);
            return 0;
        break;
        default:
        break;
    }

    //Close: Save and free memory
    savemodel(MODELDATA, w, 14, b, 6);
    free(w);
    free(b);
    free(z);
    free(a);
    free(dz);
    free(da);

    
    return 0;
}