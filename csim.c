#include <string.h>
#include "cachelab.h"
#include "csim.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
//Made by Ryan Houck and Caleb Anthony


void readTraceFile(int** timetable){
    FILE *file = fopen(tracefile, "r");
    char traceLine[255];
    while ((fgets(traceLine, 255, file)) != NULL){
        if(VERBOSE){ printf("%s", traceLine); }
        
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
            if(VERBOSE){ printf("%d \n", address); }
            printf("Got here!");
            fflush(stdout);
            loadAddress(address,instruction,timetable);
            if(VERBOSE){ printf("\n ----- \n\n"); }
        }
    }
}


void modifyMemory(int bitsSets, int bitsTag, int** timetable){
    loadMemory(bitsSets, bitsTag, timetable);
    loadMemory(bitsSets, bitsTag, timetable);
}

void loadMemory(int bitsSets, int bitsTag, int** timetable){
    //check if tag is in cache/set
    for(int currLine = 0; currLine < numLines; currLine++){
        if (VERBOSE)  printf("BitsTag = %x, CacheMemory %d= %x\n",bitsTag,currLine, cacheMemory[bitsSets*numLines + currLine]) ;
        if ((bitsTag | 0x80000000) == cacheMemory[bitsSets*numLines + currLine]){
            num_Hits++;
            if (VERBOSE)  printf("%s\n","hit");
			int tempLineVal = 0;
			for(tempLineVal = 0; tempLineVal < numLines; tempLineVal++){
				if(!(cacheMemory[bitsSets*numLines + tempLineVal] & 0x80000000)){
					break;
				}
			}
			for(int j = 0; currLine + j + 1 < tempLineVal; j++){
				int tempAddr = cacheMemory[bitsSets*numLines + currLine + j];
				cacheMemory[bitsSets*numLines + currLine + j] = cacheMemory[bitsSets*numLines + currLine + j + 1];
				cacheMemory[bitsSets*numLines + currLine + j + 1] = tempAddr;
			}
            return;
        }
    }
    if(timetable[bitsSets][numLines] == numLines){
        timetable[bitsSets][numLines] = 0;
        timetable[bitsSets][numLines+1] = 1;
    }
    if(timetable[bitsSets][numLines+1] == 1){
        num_Evicts++;
        if (VERBOSE)  printf("%s","evict ");
        for(int j = 0; j + 1 < numLines; j++){
            int tempAddr = cacheMemory[bitsSets*numLines + j];
            cacheMemory[bitsSets*numLines + j] = cacheMemory[bitsSets*numLines + j + 1];
            cacheMemory[bitsSets*numLines + j + 1] = tempAddr;
        }
        cacheMemory[bitsSets*numLines + (numLines - 1)] = bitsTag | 0x80000000;
    } else {
        cacheMemory[bitsSets*numLines+timetable[bitsSets][numLines]] = bitsTag | 0x80000000;
        timetable[bitsSets][timetable[bitsSets][numLines]] = bitsTag;
    }
    num_Misses++;
    timetable[bitsSets][numLines]++;
    if (VERBOSE)  printf("%s\n","miss");
}

void loadAddress(int address, char instruction, int** timetable){
    int bitsBlock = (address % (2 << (blockBits - 1)));
    int bitsSets = (address >> (blockBits)) % (2 << (setBits - 1));
    int signBit = (address >> 31) & 0x1;
    int bitsTag = ((address & 0x7fffffff) >> (blockBits + setBits)) + (signBit << (32 - (blockBits + setBits)));
        
    if(VERBOSE){
        printf("block bit value: %x \n", bitsBlock);
        printf("set bits value: %x \n", bitsSets);
        printf("tag : %x \n", bitsTag);
    }

    switch(instruction){
        case 'M':
            modifyMemory(bitsSets, bitsTag, timetable);
            break;

        case 'L':
            loadMemory(bitsSets, bitsTag, timetable);
            break;

        case 'S':
            loadMemory(bitsSets, bitsTag, timetable);
            break;
    }
}

void calculateMemory(){
   int numBlocks = 2 << blockBits;
   numSets = 2 << setBits;
   int totalMemory = numBlocks * numLines *  numSets;
   cacheMemory = malloc(totalMemory * sizeof(int));

   if(VERBOSE){ printf("calculated memory to: %d Bytes \n",totalMemory); }
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
    }
}


int main(int argc, char* argv[])
{
    parseArgs(argc, argv);
    if(VERBOSE){
        printf("Verbose: %d \n", VERBOSE);
        printf("setBits: %d \n", setBits);
        printf("numLines: %d \n", numLines);
        printf("blockBits: %d \n", blockBits);
        printf("tracefile: %s \n", tracefile);
    }

    calculateMemory();
    int* timetable[numSets];
    for(int i = 0; i < numSets; i++){
        timetable[i] = malloc((numLines+2) * sizeof(int));
    }
    for(int i = 0; i < numSets; i++){
        for(int j = 0; j < numLines + 2; j++){
            timetable[i][j] = 0;
        }
    }

    readTraceFile(timetable);
    printSummary(num_Hits, num_Misses, num_Evicts);
    return 0;
}
