#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "mymachinelearning.h"

bool isNew(char *USERDATAFILE, char *WNBFILE);
bool isExist(char *FILENAME);

//Note: Core game loop — runs forwardprop first to get Nao's decision,
//      then evaluates the outcome, then trains the model based on what happened.
//      actual_y = 1 - useroption, meaning Nao always tries to do the opposite of the user
//      (exploit silence, avoid being exploited when user confesses)
void playPrisonersDilemma(int numberoflayer, int *numberofnode, float *w, float *b, float *z, float *a, float *dz, float *da, int *round){
    int useroption = 0;
    int characteroption = 0;
    do{
        printf("Game Master: Please pick your strategy\n");
        printf("[1] Silent\n");
        printf("[2] Confess\n");
        scanf("%d", &useroption); 
        useroption -= 1;

        //Feed user input into input layer first, then run forwardprop to get Nao's response
        a[0] = (float)useroption;
        a[1] = (float)(*round);  //Pass current round as second input so Nao can adapt over time
        trustnaofp(numberoflayer, numberofnode, w, b, z, a);

        //Get character option from output layer — sigmoid output, threshold at 0.5
        characteroption = (a[getindex(numberofnode, "a", numberoflayer - 1, 0)] >= 0.5f) ? 1 : 0;

        //Evaluate game logic
        if(useroption == 0 && characteroption == 0){
            printf("Game Master\t: You both choose to stay silent, both of you get 1 year in prison\n");
        }
        else if(useroption == 1 && characteroption == 1){
            printf("Game Master\t: Both of you choose to confess, both of you get 7 years in prison\n");
        }
        else if(useroption == 1){
            printf("Game Master: You choose to Confess whereas Nao choose to silent, You are free, Nao will be sent to prison for 10 years\n");
            printf("Nao Kanzaki\t: No ...\n");
        }
        else{
            printf("Game Master: You choose to Silent whereas Nao choose to confess, You will be sent to prison for 10 years, Nao will be sent free\n");
            printf("Nao Kanzaki\t: I'm sorry ...\n");
        }

        //Train the model after each round — Nao learns to exploit silence and avoid worst outcome
        float actual_y = (float)(1 - useroption);
        trustnaobp(numberoflayer, numberofnode, w, b, z, a, da, dz, actual_y, 0.01f);

        (*round)++;

        useroption = 0;
        printf("Game Master\t: Do you want to play again?\n");
        printf("[1] Yes\n"); 
        printf("[2] No\n");
        scanf("%d", &useroption); 
    }while(useroption != 2);
}

//Note: To check if user just play the game without any save data, TRUE if yes
bool isNew(char *USERDATAFILE, char *WNBFILE){
    if(isExist(USERDATAFILE) == false || isExist(WNBFILE) == false){
        if(isExist(USERDATAFILE) == true){
            remove(USERDATAFILE);
        }
        if(isExist(WNBFILE) == true){
            remove(WNBFILE);
        }
        return true;
    }
    return false;
}

//Note: To check if a file exist or not, TRUE if exist
bool isExist(char *FILENAME){
    FILE *file = fopen(FILENAME, "r");
    if(file != NULL){
        fclose(file);
        return true;
    }  
    else{
        return false;
    }
}

//Note: Initialize the save files if they don't exist
void initiatesSaveData(void){
    FILE *userdata = fopen("userdata.csv", "w");
    if(userdata == NULL){
        return;
    }
    fprintf(userdata, "nama,ronde\n"); //Initialize header
    fclose(userdata);
    
    FILE *modeldata = fopen("modeldata.csv", "w");
    if(modeldata == NULL){
        return;
    }
    fclose(modeldata);
}

//Note: Intro dialog shown only to new players — explains the rules before the game starts
void initialgame(char *username){
    int userchoice;
    printf("Game Master\t: Alright, %s\n", username);
    Sleep(1000);
    printf("Game Master\t: Let's play a game!\n");
    Sleep(1000);
    printf("Game Master\t: The rules is simple\n");
    Sleep(1000);
    printf("==== GAME RULES ====\n");
    printf("1. Pretend that you and me were being caught by police by fraud\n");
    printf("2. We both were brought to different introgation room\n");
    printf("3. You have two option, confess that you and me are doing fraud, or just stay silent\n");
    printf("4. If a person is confessing while others silent, the person is free while the others got 10 years of prison\n");
    printf("5. However, if both stay silent, they both got 1 year of prison\n");
    printf("6. If they both confessed, both get 7 years at prison\n");
    Sleep(10000);
    printf("Game Master\t: Is that clear to you?\n");
    printf("[1] Yes\n");
    printf("[2] Then both of us just need to silent right?\n");
    scanf("%d", &userchoice);
    if(userchoice == 2){
        printf("You\t\t: Then both of us just need to silent right?\n");
        printf("Game Master\t: At the first glance, we will thought so. But let's just play the game\n");
    }
    else{
        printf("You\t\t: Yes\n");
    }
}
