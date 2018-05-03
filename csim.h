#ifndef CSIM_H
#define CSIM_H

int VERBOSE = 0;
int setBits = 0;
int numLines = 0;
int blockBits = 0;
char* tracefile = "";
int* cacheMemory;

//Function Stubs
void parseArgs(int argc, char* argv[]);
void readTraceFile(void);
void loadAddress(int address);
void calculateMemory(void);

#endif
