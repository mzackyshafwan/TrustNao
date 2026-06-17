#pragma once
#include <windows.h>
#include <stdio.h>

void initialgame(char *username){
    int userchoice;
    printf("Mastermind\t: Alright, %s\n", username);
    Sleep(1000);
    printf("Mastermind\t: Let's play a game!\n");
    Sleep(1000);
    printf("Mastermind\t: The rules is simple\n");
    Sleep(1000);
    printf("==== GAME RULES ====\n");
    printf("1. Pretend that you and me were being caught by police by fraud\n");
    printf("2. We both were brought to different introgation room\n");
    printf("3. You have two option, confess that you and me are doing fraud, or just stay silent\n");
    printf("4. If a person is confessing while others silent, the person is free whie the others got 10 years of prison\n");
    printf("5. However, if both stay silent, they both got 1 year of prison\n");
    printf("6. If they both confessed, both get 7 years at prison\n");
    Sleep(10000);
    printf("Mastermind\t: Is that clear to you?\n");
    printf("[1] Yes\n");
    printf("[2] Then both of us just need to silent right?\n");
    scanf("%d", &userchoice);
    if(userchoice == 2){
    printf("You\t\t: Then both of us just need to silent right?\n")
    printf("Mastermind\t: At the first glance, we will thought so. But let's just play the game\n");
    }
}