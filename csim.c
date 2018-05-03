#include "cachelab.h"
#include "csim.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
//Made by Ryan Houck and Caleb Anthony


void readTraceFile(void){
    FILE *file = fopen("./traces/yi.trace", "r");
    char traceLine[255];
    printf("%s", "Got here! \n");
    fflush(stdout);
    while ((fgets(traceLine, 255, file)) != NULL){
        printf("%s \n", traceLine);
        
        char instruction = traceLine[1];
        if (instruction != ' ') { //if there is an I or something without a space, skip line
            //printf("Instruction: %c \n",instruction);
            int i = 3;
            for (; traceLine[i+1] != ','; i++){ }
            int maxi = i;
            unsigned address = 0;
            for(; i >= 3; i--){
                int addrVal = (int)traceLine[i];
                if(addrVal > 57){ //If greater than ASCII 9
                    addrVal -= 87;
                } else {
                    addrVal -= 48;
                }
                if(i == maxi){
                    address = addrVal;
                } else{
                    address += addrVal * (int)pow(2, (4 * (maxi - i)));
                }
            }
            printf("%d \n", address);
            loadAddress(address);

        }
        //fgets(traceLine, 255, file);
    }
}

/*
int moveMemory(int address){


}

int loadMemory(int address){
    

}

int storeMemory(int address){

}
*/

void loadAddress(int address){
    int bitsBlock = (address % (2 << blockBits));
    int bitsSets = (address >> blockBits) % (2 << setBits);
    int signBit = (address >> 31) & 0x1;
    int bitsTag = ((address & 0x7fffffff) >> (blockBits + setBits)) + (signBit << (32 - (blockBits + setBits)));
        
    printf("block bit value: %x \n", bitsBlock);
    printf("set bits value: %x \n", bitsSets);
    printf("tag : %x \n", bitsTag);

}



void calculateMemory(){
   int numBlocks = 2 << blockBits;
   int numSets = 2 << setBits;
   int totalMemory = numBlocks * numLines *  numSets;
   cacheMemory = malloc(totalMemory);
   printf("calculated memory to: %d \n",totalMemory);
}

void parseArgs(int argc, char* argv[]){
    for (int i =0; i < argc; i ++){
        char arg = argv[i][1];
        if (arg == 'h') {
            // Optional help flag here
        }
        else if (arg == 'v') {
            VERBOSE = 1;
            // Optional Verbose flag
        }
        else if (arg == 's') {
            // number of set index bits setting
            printf("%s", "Got here");
            i++;
            if(i < argc){
                setBits = atoi(argv[i]);
            } else {
                printf("%s \n", "Please give a valid number of set index bits.");
            }
        }
        else if (arg == 'E') {
            // Set associativity (number of lines/set)
            i++;
            if(i < argc){
                numLines = atoi(argv[i]);
            } else {
                printf("%s \n", "Please give a valid number of lines per set.");
            }
        }
        else if (arg == 'b') {
            // Number of block bits (blocksize)
            i++;
            if(i < argc){
                blockBits = atoi(argv[i]);
            } else {
                printf("%s \n", "Please give a valid number of block bits.");
            }
        }
        else if (arg == 't') {
            // name of valgrind trace (filename)
            i++;
            if(i < argc){
                tracefile = argv[i];
            } else {
                printf("%s \n", "Please give a valid valgrind trace file name.");
            }
        }

        printf("%d arg, %s \n ",i,argv[i]);
    }
}


int main(int argc, char* argv[])
{
    parseArgs(argc, argv);
    printf("Verbose: %d \n", VERBOSE);
    printf("setBits: %d \n", setBits);
    printf("numLines: %d \n", numLines);
    printf("blockBits: %d \n", blockBits);
    printf("tracefile: %s \n", tracefile);
    calculateMemory();

    readTraceFile();
    printSummary(0, 0, 0);
    return 0;
}
