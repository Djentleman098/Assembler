#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_vars.h"
#include "utils.h"
#include "passes.h"

/*second pass - save .entry values, saves instructions and data to mamory image*/
int beginSecondPass(char* fName, labelsTable** lb, Instruction** ins, Data** dt, int ***MEMORY_IMG, int* MEMORY_POS){
	
	int result = 1;
	FILE* fb;
	int IC = 0;
 
	labelsTable* labels_table = *lb;
	Instruction* ins_list = *ins;
	Data* data = *dt;

	char* current_line [LINE_LENGTH];
	char* getWord;
	char* nextWord;
	int getWord_pos;
	int nextWord_pos;
	int i = 0;
	int lineNum = 0;

	fb = fopen(fName, "r");
	
	/*go through the file and read only instractions first, and handle .entrys*/
	while(fgets(current_line, LINE_LENGTH, fb)!=NULL && !feof(fb)){
		lineNum++;
		getWord = getNextWord(current_line, i, &getWord_pos);


		/*line starts with LABEL:*/
		if (isLabel(getWord)){
			nextWord = getNextWord(current_line, getWord_pos + strlen(getWord), &nextWord_pos);
			/*line structure is- LABEL: .entry/ ...*/
			if (isExIn(nextWord) == 1 && (strcmp(ENTRY, nextWord) == 0)){
				if(handleWithEntry(current_line, lb, nextWord_pos, fName, lineNum) == 0)
					result = 0;
			}
			/*line structure is- LABEL: operand ...*/
			if (isOperand(nextWord)){
				calcARE(ins, lb, lineNum);
				if(insertOperandToMemory(ins, lineNum, MEMORY_IMG, MEMORY_POS, fName, lb) == 0)
					result = 0;
			}
			
			free(nextWord);
		}
		
		/*line starts with .entry/*/
		if (isExIn(getWord) == 1 && (strcmp(ENTRY, getWord) == 0)){
			if(handleWithEntry(current_line, lb, getWord_pos, fName, lineNum) == 0)
					result = 0;
		}

		/*line starts with operand*/
		if (isOperand(getWord)){
			calcARE(ins, lb, lineNum);
			if(insertOperandToMemory(ins, lineNum, MEMORY_IMG, MEMORY_POS, fName, lb) == 0)
				result = 0;
		}
			
		free(getWord);
	}

	lineNum = 0;
	fseek(fb, 0, SEEK_SET);

	/*go through the file and read only data secondly*/
	while(fgets(current_line, LINE_LENGTH, fb)!=NULL && !feof(fb)){
		lineNum++;
		getWord = getNextWord(current_line, i, &getWord_pos);

		/*line starts with LABEL:*/
		if (isLabel(getWord)){
			nextWord = getNextWord(current_line, getWord_pos + strlen(getWord), &nextWord_pos);
			/*line structure is- LABEL: .data/.string ...*/
			if (isData(nextWord)){
				if(insertDataToMemory(dt, lineNum, MEMORY_IMG, MEMORY_POS, fName) == 0)
					result = 0;
			}
			
			free(nextWord);
		}
		
		/*line starts with .data/.string*/
		if (isData(getWord)){
			if(insertDataToMemory(dt, lineNum, MEMORY_IMG, MEMORY_POS, fName) == 0)
				result = 0;
		}
			
		free(getWord);
	}

	*lb = labels_table;
	*ins = ins_list;
	*dt = data;
	
	return result;
}

