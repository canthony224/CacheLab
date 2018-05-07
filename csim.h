#ifndef CSIM_H
#define CSIM_H

int VERBOSE = 0;
int setBits = 0;
int numLines = 0;
int blockBits = 0;
int numSets;
char* tracefile = "";
int* cacheMemory;

unsigned int num_Hits;
unsigned int num_Misses;
unsigned int num_Evicts;

//Function Stubs
void modifyMemory(int bitsSets, int bitsTag, int** timetable);
void loadMemory(int bitsSets, int bitsTag, int** timetable);
//void storeMemory(int bitsSets, int bitsTag);
void initTimetable();
void parseArgs(int argc, char* argv[]);
void readTraceFile(int** timetable);
void loadAddress(int address, char instruction, int** timetable);
void calculateMemory(void);

#endif
