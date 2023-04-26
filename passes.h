#include "dataStructs.h"

/************* first pass ****************/
/*Starting the first pass using the .am file from pre assembler*/
int beginFirstPass(char*, labelsTable**, Instruction**, Data**);

/*For each type of word, returns true or false*/
int isLabel(char*);
int isData(char*);
int isExIn(char*);
int isOperand(char*);

/*For each type of word, handle the line according to the algorithm - print errors and save to data or memory*/
void handleWithLabel(char*, int, char, labelsTable**);
int handleWithData(Data**, char*, char*, int, int*, char*, int);
int handleWithExIn(char*, labelsTable**, int, int, char*, int);
int handleWithEntry(char*, labelsTable**, int, char*, int);
int handleWithOperand(Instruction**, char*, char*, int, int*, char*, int);

/*For each operand group, handle the operand - errors and memory*/
int handleWithGroupOne(Instruction**, char*, char*, char*, int, int*);
int handleWithGroupTwo(Instruction**, char*, char*, char*, int, int*);
int handleWithGroupThree(Instruction**, char*, char*, char*, int, int*);

/*Addressing methods*/
int isAddressingMiyadi(char*, char**);
int isAddessingYashir(char*, char**);
int isAddressingJump(char*, char**, char**, char**);
int isAddressingRegister(char*, char**);

int getAddressingType(char*, char**, char**, char**);

/************* second pass ****************/
/*Starting the second pass using the .am file from pre assembler*/
int beginSecondPass(char*, labelsTable**, Instruction**, Data**, int ***, int*);

/*For entry, handle the line according to the algorithm - print errors and save to data or memory*/
int handleWithEntry(char*, labelsTable**, int, char*, int);

/*Save the data and instructions from the data stractures to memory image*/
int insertOperandToMemory(Instruction**, int, int***, int*, char*, labelsTable**);
int insertDataToMemory(Data**, int, int***, int*, char*);
