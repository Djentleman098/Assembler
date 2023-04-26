#include <stdio.h>

/*Gets a file from command line argument, reading it*/
int beginPreAsm(char*);

/*First read through, checks for line length errors*/
int firstCheck(FILE*, char*);

/*Creates .am file*/
void createAm(FILE*, char*);

