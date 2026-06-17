#include <string.h>
#include <stdio.h>

void getname(char *FILENAME, char *namevariable){
    FILE *file = fopen(FILENAME, "r");
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);
    char *name = strtok(buffer, ",");
    strcpy(namevariable, name);
    fclose(file);
}

void addname(char *FILENAME, char name[]){

    FILE *file = fopen(FILENAME, "r");
    FILE *file2 = fopen("temp.csv", "w");
    char buffer[1024];
    int row = 1;
    while(fgets(buffer, sizeof(buffer), file)){
        fprintf(file2, buffer);
        row++;
        if(row == 2){
            char name2[20];
            strcpy(name2, name);
            fprintf(file2, "%s, 0", name2);
            break;
        }
    }
    fclose(file);
    remove(FILENAME);
    fclose(file2);
    rename("temp.csv", FILENAME);
}

//Note: This function save flattened w in one row and b in another row
void savemodel(const char *filename, float *w, int total_w, float *b, int total_b) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Failed to create file %s\n", filename);
        return;
    }

    for (int i = 0; i < total_w; i++) { //write w in the first row
        fprintf(file, "%f", w[i]);
        if (i < total_w - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    for (int i = 0; i < total_b; i++) { //write b in the second row
        fprintf(file, "%f", b[i]);
        if (i < total_b - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");
    fclose(file);
}

//Note: This function take w and b according to how my savemodel() works 
void loadmodel(const char *filename, float *w, int total_w, float *b, int total_b) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: File %s not found.\n", filename);
        return;
    }
    for (int i = 0; i < total_w; i++) { //Read w
        fscanf(file, "%f,", &w[i]); 
    }
    for (int i = 0; i < total_b; i++) { //Read b
        fscanf(file, "%f,", &b[i]);
    }
    fclose(file);
}