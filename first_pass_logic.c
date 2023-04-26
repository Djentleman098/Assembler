#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_vars.h"
#include "utils.h"
#include "passes.h"

/*Go through the .am file and handles every line - checks for errors, create labels table and insert data and instructions to memory image*/
int beginFirstPass(char* fName, labelsTable** lb, Instruction** ins, Data** dt){

	int result = 1;
	FILE* fb;
	/*Creates DC and IC for the current file*/
	int IC = 0;
	int DC = 0;

	char* current_line [LINE_LENGTH];
	char* getWord;
	char* nextWord;
	int getWord_pos;
	int nextWord_pos;
	int i = 0;
	int lineNum = 0;
	boolean legalLine = FALSE;

	fb = fopen(fName, "r");
	
	/*reads the file and analize the structure of each line, and handles the components accordingly*/
	while(fgets(current_line, LINE_LENGTH, fb)!=NULL && !feof(fb)){
		lineNum++;
		getWord = getNextWord(current_line, i, &getWord_pos);

		/*line starts with LABEL:*/
		if (isLabel(getWord)){
			nextWord = getNextWord(current_line, getWord_pos + strlen(getWord), &nextWord_pos);
			/*line structure is- LABEL: .data/.string ...*/
			if (isData(nextWord)){
				legalLine = TRUE;
				handleWithLabel(getWord, DC, 'd', lb);
				if(handleWithData(dt, nextWord, current_line, nextWord_pos, &DC, fName, lineNum) == 0)
					result = 0;
			}
			/*line structure is- LABEL: .extern/ ...*/
			if (isExIn(nextWord)){
				legalLine = TRUE;
				if ((strcmp(EXTERN, getWord) == 0))
					if (handleWithExIn(current_line, lb, nextWord_pos, DC, fName, lineNum) == 0)
						result = 0;
			}
			/*line structure is- LABEL: operand ...*/
			if (isOperand(nextWord)){
				legalLine = TRUE;
				handleWithLabel(getWord, IC, 'c', lb);
				if(handleWithOperand(ins, current_line, nextWord, nextWord_pos, &IC, fName, lineNum) == 0)
					result = 0;
			}
			
			free(nextWord);
		}
		
		/*line starts with .data/.string*/
		if (isData(getWord)){
			legalLine = TRUE;
			if(handleWithData(dt, getWord, current_line, getWord_pos, &DC, fName, lineNum) == 0)
				result = 0;
			}

		/*line starts with .extern/*/
		if (isExIn(getWord)){
			legalLine = TRUE;
			if ((strcmp(EXTERN, getWord) == 0))
				if (handleWithExIn(current_line, lb, getWord_pos, DC, fName, lineNum) == 0)
					result = 0;
			}
		
		/*line starts with operand*/
		if (isOperand(getWord)){
			legalLine = TRUE;
			if(handleWithOperand(ins, current_line, getWord, getWord_pos, &IC, fName, lineNum) == 0)
					result = 0;
			}

		/*comment line*/
		if (getWord[0] == ';' || getWord[0] == '\0')
			legalLine = TRUE;

		/*if we didnt encounter a legal line, the line is illegal*/
		if (legalLine == FALSE){
			printf("ERROR: in file %s line %d - illegal line.\n", fName, lineNum);
			result = 0;
		}
		
		/*finished reading the line, continue*/	
		legalLine = FALSE;
		free(getWord);
	}

	updateLabelsValues(lb, IC, 'd');
	updateLabelsValues(lb, 100, 'd');/*change the values so it starts on 100 memory position*/
	updateLabelsValues(lb, 100, 'c');/*change the values so it starts on 100 memory position*/
	updateLabelsValues(lb, 100, 'e');/*change the values so it starts on 100 memory position*/
	
	return result;
}



