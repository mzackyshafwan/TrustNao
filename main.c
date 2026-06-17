#include "database.h"
#include "mymachinelearning.h"
#include "gameanddialog.h"
#include <windows.h>
#include <time.h>

#define USERDATA "userdata.csv"
#define MODELDATA "modeldata.csv"

int main(void){
    srand(time(NULL));

    //Variables
    char username[20];
    int round = 0;
    
    //Model structures
    float *w, *b, *z, *a, *dz, *da;
    int numberofnode[4] = {2, 3, 2, 1};
    int numberoflayer = 4;

    //Calculate total_w and total_b from numberofnode so it stays in sync with the architecture
    int total_w = 0;
    int total_b = 0;
    for(int i = 1; i < numberoflayer; i++){
        total_w += numberofnode[i] * numberofnode[i - 1];
        total_b += numberofnode[i];
    }
    
    allocatememory(numberoflayer, numberofnode, &w, &b, &z, &a, &dz, &da); 

    if(isNew(USERDATA, MODELDATA) == true){
        initweights(numberoflayer, numberofnode, w, b, (int)time(NULL));
        initiatesSaveData();
        
        printf("Game Master\t: Hello! You must be new here!\n");
        Sleep(1500);
        printf("Game Master\t: May I know your name, stranger?\n");
        Sleep(1500);
        printf("You\t\t: Uhhh ... My name is ");
        scanf("%s", username);
        addName(USERDATA, username);
        printf("Game Master\t: Nice to meet you %s\n", username);

        //Show intro rules only for new players
        initialgame(username);

        //Start the main game loop — round starts at 0 and gets updated inside
        playPrisonersDilemma(numberoflayer, numberofnode, w, b, z, a, dz, da, &round);
    }
    else{
        //Load existing session — model weights and user progress
        loadModel(MODELDATA, w, total_w, b, total_b);
        loadUserData(USERDATA, username, &round);
        
        printf("Game Master: Welcome back %s! You've played %d round(s) before\n", username, round);

        //Continue the game loop from where they left off
        playPrisonersDilemma(numberoflayer, numberofnode, w, b, z, a, dz, da, &round);
    }

    //Close: Save model weights and user progress, then free all memory
    savemodel(MODELDATA, w, total_w, b, total_b);
    saveUserData(USERDATA, username, round);
    free(w);
    free(b);
    free(z);
    free(a);
    free(dz);
    free(da);

    return 0;
}
