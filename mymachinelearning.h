//LinkedIn: @zackyshafwan
#pragma once

//Note: Number of layer means all layer from Input Layer to Output Layer.
//For Example:  You have two input X1 and X2, These are considered as Input Layer or Layer 0. 
//              Then there are processing layer, call it Layer 1, 2, and 3. If layer 3 output is the one to be compared to the actual result of 
//              the sample, either it needs to be converted with activation function (Sigmoid, ReLU), then Layer 3 is an Output Layer
//              So, we have 4 layer in total, Layer 0, 1, 2, 3.
//Note: For ease, assume the input X1 and X2 is part of A in layer 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

//<====== HELPER FUNCTION =================================================>

//Note: This function is optional, use it when the array of numberoflayer has not been set. First you need to create the pointer of integer to-
//store the number of node in each layer, and a variable to store the number of layer in the main program
//Don't forget to free the pointer in the end of the program
    void initialinput(int *numberoflayer, int **numberofnodeinlayer){
        printf("Type the number of layer: ");
        scanf("%d", numberoflayer);

        *numberofnodeinlayer = (int *) malloc(*numberoflayer * sizeof(int));

        for(int i = 0; i < *numberoflayer; i++){
            printf("\tType the number of node in layer %d: ", i);
            scanf("%d", &((*numberofnodeinlayer)[i]));
        }
        printf("You are all set!\n");
    }

    void allocatememory(int numberoflayer, int *numberofneuroninlayer, float **w, float **b, float **z, float **a, float **dz, float **da){

        if(numberoflayer <= 1){
            printf("Error\t: Not enough layer. More than 1 layer required\n");
            return;
        }

        int temp1 = 0;
        int temp2 = 0;

        for(int layernow = 1; layernow < numberoflayer; layernow++){
            temp1 += numberofneuroninlayer[layernow];
            temp2 += numberofneuroninlayer[layernow] * numberofneuroninlayer[layernow - 1];
        }
        *w = (float *) malloc(temp2 * sizeof(float));
        *b = (float *) malloc(temp1 * sizeof(float)); 
        *z = (float *) malloc(temp1 * sizeof(float));
        *a = (float *) malloc((temp1 + numberofneuroninlayer[0]) * sizeof(float));
        *dz = (float *) malloc(temp1 * sizeof(float)); 
        *da = (float *) malloc(temp1 * sizeof(float));
    }

    //How to use: For example you want to get an index from the flattened array correspond at nth layer and mth neuron (both start by 0). Use n as atlayer value, and m as atneuron value
    //Note: This is only avalaible for the following array: w, b, z, a, dw, db, dz, da. Specific for w and dw, it can only return an index of its first w/dw in the neuron
    int getindex(int numberofnode[], char *flattenedarray, int atlayer, int atneuron){
        int temp = 0;

        if((strcmp(flattenedarray, "w") == 0) || (strcmp(flattenedarray, "dw") == 0)){
            for(int i = 1; i < atlayer; i++){
                temp += numberofnode[i] * numberofnode[i - 1];
            }
            return temp + (numberofnode[atlayer - 1] * atneuron);
        }


        for(int i = 0; i < atlayer; i++){
            if(((strcmp(flattenedarray, "a")) != 0) && i == 0){
                continue;
            }
            temp += numberofnode[i];
        }
        return temp + atneuron;
    }

    //Note: Call this function after generatedatabase() and before training
    //      This uses He Initialization for weights (recommended for ReLU)
    //      and sets all biases to 0
    //      seed parameter: use any number (e.g. 42), change it to get different random results
    float gaussianrandom(){
        float u1 = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f);
        float u2 = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f);
        return sqrtf(-2.0f * logf(u1)) * cosf(2.0f * 3.14159265f * u2);
    }

    void initweights(int numberoflayer, int *numberofnode, float *w, float *b, int seed){
        srand(seed);

        for(int layer = 1; layer < numberoflayer; layer++){
            float scale = sqrtf(2.0f / numberofnode[layer - 1]); //He initialization scale

            for(int neuron = 0; neuron < numberofnode[layer]; neuron++){
                for(int i = 0; i < numberofnode[layer - 1]; i++){
                    w[getindex(numberofnode, "w", layer, neuron) + i] = gaussianrandom() * scale;
                }
                b[getindex(numberofnode, "b", layer, neuron)] = 0.0f;
            }
        }
    }

//<====== MAIN FUNCTIOn =================================================>

    //Note: This function assume that all the neuron use ReLU as activation function
    void forwardprop(int numberoflayer, int *numberofnode, float *w, float *b, float *z, float *a){
        for(int layer = 1; layer < numberoflayer; layer++){
            for(int neuron = 0; neuron < numberofnode[layer]; neuron++){
                z[getindex(numberofnode, "z", layer, neuron)] = 0.0f;
                for(int i = 0; i < numberofnode[layer - 1]; i++){
                    z[getindex(numberofnode, "z", layer, neuron)] += w[getindex(numberofnode, "w", layer, neuron) + i] * a[getindex(numberofnode, "a", layer - 1, i)]; 
                }
                z[getindex(numberofnode, "z", layer, neuron)] += b[getindex(numberofnode, "b", layer, neuron)]; //Add the Vector dot result with it's bias

                //You can change the activation function below! :)
                a[getindex(numberofnode, "a", layer, neuron)] = (z[getindex(numberofnode, "z", layer, neuron)] >= 0 ? z[getindex(numberofnode, "z", layer, neuron)] : 0.0f); //Activate ReLU
            }   
        }
    }

    //In my case, I use ReLU function in every neuron except the output neuron is sigmoid
    void trustnaofp(int numberoflayer, int *numberofnode, float *w, float *b, float *z, float *a){
        for(int layer = 1; layer < numberoflayer; layer++){

            for(int neuron = 0; neuron < numberofnode[layer]; neuron++){
                z[getindex(numberofnode, "z", layer, neuron)] = 0.0f;
                for(int i = 0; i < numberofnode[layer - 1]; i++){
                    z[getindex(numberofnode, "z", layer, neuron)] += w[getindex(numberofnode, "w", layer, neuron) + i] * a[getindex(numberofnode, "a", layer - 1, i)]; 
                }
                z[getindex(numberofnode, "z", layer, neuron)] += b[getindex(numberofnode, "b", layer, neuron)]; //Add the Vector dot result with it's bias

                if(layer == numberoflayer - 1){
                    a[getindex(numberofnode, "a", layer, neuron)] = 1 / (1 + exp( - (z[getindex(numberofnode, "z", layer, neuron)]))); //Use sigmoid
                    continue;
                }
                
                a[getindex(numberofnode, "a", layer, neuron)] = (z[getindex(numberofnode, "z", layer, neuron)] >= 0 ? z[getindex(numberofnode, "z", layer, neuron)] : 0.0f); //Activate ReLU
            }   
        }
    }


    void trustnaobp(int numberoflayer, int *numberofnode, float *w, float *b, float *z, float *a, float *da, float *dz, float actual_y, float learning_rate){
    
        int out_layer = numberoflayer - 1;

        dz[getindex(numberofnode, "dz", out_layer, 0)] = a[getindex(numberofnode, "a", out_layer, 0)] - actual_y;

        for(int layernow = out_layer - 1; layernow >= 1; layernow--){
            for(int neuron = 0; neuron < numberofnode[layernow]; neuron++){
                da[getindex(numberofnode, "da", layernow, neuron)] = 0.0f;
                for(int b = 0; b < numberofnode[layernow + 1]; b++){
                    da[getindex(numberofnode, "da", layernow, neuron)] += dz[getindex(numberofnode, "dz", layernow + 1, b)] * w[getindex(numberofnode, "w", layernow + 1, b) + neuron];
                }
                dz[getindex(numberofnode, "dz", layernow, neuron)] = (z[getindex(numberofnode, "z", layernow, neuron)] > 0.0f) ? da[getindex(numberofnode, "da", layernow, neuron)] : 0.0f;
            }
        }

        //Loop for updating weight and bias
        for(int layer = 1; layer < numberoflayer; layer++){
            for(int neuron = 0; neuron < numberofnode[layer]; neuron++){
                for(int i = 0; i < numberofnode[layer - 1]; i++){
                    w[getindex(numberofnode, "w", layer, neuron) + i] -= learning_rate * dz[getindex(numberofnode, "dz", layer, neuron)] * a[getindex(numberofnode, "a", layer - 1, i)];
                }
                b[getindex(numberofnode, "b", layer, neuron)] -= learning_rate * dz[getindex(numberofnode, "dz", layer, neuron)];
            }
       }
    }
