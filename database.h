#pragma once
#include <string.h>
#include <stdio.h>

//Note: Extract a single name from the second row of the CSV file
void getname(char *FILENAME, char *namevariable) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return; 
    }
    
    char buffer[1024];
    //Skip header
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        //Read the second row
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            char *name = strtok(buffer, ",");
            if (name != NULL) { 
                strcpy(namevariable, name);
            }
        }
    }
    fclose(file);
}

//Note: Load both name and round from the second row directly into variables
void loadUserData(char *FILENAME, char *namevariable, int *roundvariable) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return; 
    }
    
    char row[100];
    
    //Skip header
    fgets(row, sizeof(row), file);
    
    //Read data and parse
    if (fgets(row, sizeof(row), file) != NULL) {
        sscanf(row, "%[^,],%d", namevariable, roundvariable);
    }
    
    fclose(file);
}

//Note: Overwrite the second row (data row) with updated name and round,
//      while keeping the header intact
void saveUserData(char *FILENAME, char *name, int round) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        return;
    }
    fprintf(file, "nama,ronde\n");
    fprintf(file, "%s,%d\n", name, round);
    fclose(file);
}

//Note: Append a new player name at the end of the CSV file
void addName(char *FILENAME, char name[]) {
    FILE *file = fopen(FILENAME, "a");
    if (file != NULL) {
        fprintf(file, "%s,0\n", name);
        fclose(file);
    }
}

//Note: This function saves flattened w in the first row and b in the second row
void savemodel(const char *filename, float *w, int total_w, float *b, int total_b) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Failed to create file %s\n", filename);
        return;
    }

    //Write w
    for (int i = 0; i < total_w; i++) { 
        fprintf(file, "%f", w[i]);
        if (i < total_w - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    //Write b
    for (int i = 0; i < total_b; i++) { 
        fprintf(file, "%f", b[i]);
        if (i < total_b - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");
    fclose(file);
}

//Note: This function takes w and b according to how savemodel() works
void loadModel(const char *filename, float *w, int total_w, float *b, int total_b) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: File %s not found.\n", filename);
        return;
    }
    
    //Read w
    for (int i = 0; i < total_w; i++) { 
        fscanf(file, "%f,", &w[i]); 
    }
    //Read b
    for (int i = 0; i < total_b; i++) { 
        fscanf(file, "%f,", &b[i]);
    }
    fclose(file);
}
