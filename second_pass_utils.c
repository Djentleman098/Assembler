#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_vars.h"
#include "utils.h"
#include "passes.h"

/*handle with .entry*/
int handleWithEntry(char* line, labelsTable** labels_table, int pos, char* fileName, int lineNum){
	char* tempLabel;
	char* nextWord;
	int nextWord_pos;
	int result = 1;
	
	/*makes sure the parameter is a legal label*/
	nextWord = getNextWord(line, pos + 6, &nextWord_pos);
	tempLabel = (char*)malloc_safely(sizeof(char) * (strlen(nextWord) + 1));
	strcpy(tempLabel, nextWord);
	strcat(tempLabel, ":");

	if (isLabel(tempLabel)){
		if (existsInLabels(*labels_table, nextWord))/*calls a function that changes the type of the label to entry*/
			changeLabelToEntry(labels_table, nextWord);
		else{
			printf("ERROR: in file %s line %d - illegal .entry.\n", fileName, lineNum);
			result = 0;
		}
	} else {
		printf("ERROR: in file %s line %d - invalid label in .entry.\n", fileName, lineNum);
		result = 0;
	}

	/*makes sure there are no other parameters*/
	nextWord = getNextWord(line, nextWord_pos + strlen(nextWord), &nextWord_pos);
	if (*nextWord != '\0'){
		printf("ERROR: in file %s line %d - to many variables in .entry.\n", fileName, lineNum);
		result = 0;
	}

	return result;

}

/*saves data to memory image*/
int insertDataToMemory(Data** dt, int lineNum, int ***MEMORY_IMG, int* MEMORY_POS, char* fileName){
	Data* temp;
	temp = *dt;
	char* value;
	char character;
	int val;
	int j;
	int** memory = *MEMORY_IMG;
	int result = 1;

	/*go throug the data data stracture*/
	while (temp != NULL){
		/*make sure we are in memory bounds*/
		if ((*MEMORY_POS) >= 256){
			printf("ERROR: in file %s line %d - out of memory.\n", fileName, lineNum);
			result = 0;
			break;
		}
		
		/*gets the current value from data stracture and allocates memory for new mempry word*/
		if (temp->lineNum == lineNum){
			value = (char*)malloc_safely(sizeof(char)*strlen(temp->value));
			value = strcpy(value, temp->value);
			
			*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
			
			/*number*/			
			if (temp->type == 'n'){
				/*remove + sign from number*/
				if (value[0] == '+')
					value++;
				/*saves the number - bits*/
				*(memory + sizeof(int*)*(*MEMORY_POS)) = numToBitArray(atoi(value));
				(*MEMORY_POS)++;
			}

			/*character*/
			if (temp->type == 'c'){
				character = value[0];
				/*saves the character - bits*/
				*(memory + sizeof(int*)*(*MEMORY_POS)) = numToBitArray(character);
					
				(*MEMORY_POS)++;
			}
		}

		temp = temp->next;
	}
	return result;
	
}

/*saves instructions to memory image*/
int insertOperandToMemory(Instruction** ins, int lineNum, int ***MEMORY_IMG, int* MEMORY_POS, char* fileName, labelsTable** lb){

	Instruction* temp;
	temp = *ins;
	labelsTable* tempLb;
	tempLb = *lb;
	int** memory = *MEMORY_IMG;
	int* tempLine;
	char* value;
	char character;
	int val;
	int j;
	int result = 1;

	/*go throug the instrctions data stracture*/
	while (temp != NULL){

		/*make sure we are in memory bounds*/
		if ((*MEMORY_POS) >= 256){
			printf("ERROR: in file %s line %d - out of memory.\n", fileName, lineNum);
			result = 0;
			break;
		}
		
		if (temp->lineNum == lineNum){
			/*insert the instruction line to memory*/
			*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
			*(memory + sizeof(int*)*(*MEMORY_POS)) = insertInstruction(temp);
			(*MEMORY_POS)++;


			/*insert the extra words to memory*/
			/*MIYADI*/
			if (temp->adressingDest == 0){
				*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
				*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 0, "dest", "");
				(*MEMORY_POS)++;			}
			if (temp->adressingSource == 0){
				*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
				*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 0, "source", "");
				(*MEMORY_POS)++;			}
			/*YASHIR*/
			if (temp->adressingDest == 1){
				*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
				*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 1, "dest", "");
				(*MEMORY_POS)++;			}
			if (temp->adressingSource == 1){
				*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
				*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 1, "source", "");
				(*MEMORY_POS)++;			}
			/*JUMP - for each operand, check all the possible addressing of parameters*/
			if (temp->adressingDest == 2){
				*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
				*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 2, "dest", "val");
				(*MEMORY_POS)++;
				if (temp->secondPar == 3 && temp->firstPar == 3){
					*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
					*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 2, "dest", "pars");
					(*MEMORY_POS)++;
				} else {
					*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
					*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 2, "dest", "par1");
					(*MEMORY_POS)++;
					*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
					*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 2, "dest", "par2");
					(*MEMORY_POS)++;
				}
			}
			if (temp->adressingSource == 2){
				*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
				*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 2, "source", "val");
				(*MEMORY_POS)++;
				if (temp->secondPar == 3 && temp->firstPar == 3){
					*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
					*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 2, "source", "pars");
					(*MEMORY_POS)++;
				} else {
					*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
					*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 2, "source", "par1");
					(*MEMORY_POS)++;
					*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
					*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 2, "source", "par2");
					(*MEMORY_POS)++;
				}
			}

			/*REGISTER*/
			if (temp->adressingDest == 3){
				if (temp->adressingSource == 3){/*if both operands are registers - save them in one word*/
					*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
					*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 3, "dest", "pars");
					(*MEMORY_POS)++;
				} else {
					*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
					*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 3, "dest", "");
					(*MEMORY_POS)++;
				}
			}
			if (temp->adressingSource == 3 && temp->adressingDest != 3){
				*(memory + sizeof(int*)*(*MEMORY_POS)) = (int*)malloc_safely(sizeof(int)*14);
				*(memory + sizeof(int*)*(*MEMORY_POS)) = insertWordToMemory(temp, tempLb, 3, "source", "");
				(*MEMORY_POS)++;
			}

		}

		temp = temp->next;
	}
	
	return result;
}
