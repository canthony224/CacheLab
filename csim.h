#ifndef CSIM_H
#define CSIM_H

int VERBOSE = 0;
int idxBits = 0;
int numLines = 0;
int blockBits = 0;
char* tracefile = "";

//Function Stubs
void parseArgs(int argc, char* argv[]);
void readTraceFile(void);

#endif
