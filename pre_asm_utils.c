#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pre_asm.h"
#include "global_vars.h"
#include "dataStructs.h"
#include "utils.h"

/*First read through, checks for line length errors*/
int firstCheck(FILE* fb, char* fileFullName){

	int result = 1;
	char current_line [LINE_LENGTH]; /*Saves a line read from the file*/
	int c; /*For moving to next line in case the line is too long*/
	int lineNum; /*Saves the current line in the file*/

	lineNum = 1;
	while (fgets(current_line, LINE_LENGTH, fb)!=NULL && !feof(fb)){
		/*Checks if the length of a line is allowed*/
		if (strchr(current_line, '\n') == NULL){
			printf("ERROR: in file %s line %d is too long. Any line should be %d characters max.\n", fileFullName, lineNum, LINE_LENGTH);
			result = 0;
			/*Force moving to the next line in case the line is too long*/			
			if (!feof(fb)){
				while ((c = getc(fb)) != '\n' && c != EOF)
					fseek(fb, 1, SEEK_CUR);
			}
		}
		lineNum++;
	}
	
	return result;
}

/*Creates the .am file*/
void createAm(FILE* current_file, char* fName){
	char current_line [LINE_LENGTH]; /*Saves a line read from the file*/
	Table *macros = NULL; /*The macros table*/
	FILE *amFile; /*The new .am file created*/
	char* getWord; /*Saves the first word of a new line*/
	int getWord_pos; /*Saves the first word position*/
	char* nextWord;
	int nextWord_pos;
	int i = 0; /*Index for getWord*/
	int j; /*Index for macro writing*/
	char* current_macro; /*Saves the name of a macro read*/
	boolean macroFlag = FALSE; /*Flag for macro storing algorithm*/
	int macroLinesNum; /*Line number of a certain macro*/
	char **macroLines; /*Lines 2D array of a certain macro*/
	/*char* MCR = (char*)malloc_safely(sizeof(char)*3);
	MCR = "mcr";
	char* ENDMCR = (char*)malloc_safely(sizeof(char)*6);
	ENDMCR = "endmcr";*/
	int newmcrcpyResult;
	int endmcrcpyResult;
	Table* currentTableNode;

	amFile = fopen(createFIleName(fName, ".am"), "w+"); /*Creates the .am file, removes the .as*/
	if (amFile == NULL){
		printf("ERROR: file %s could not be opened.\n", (char*)amFile);
		free(amFile);
		return;
	}

	int loopCounter = 0;
	/*run through the .as file and wrting the .am file*/
	while(fgets(current_line, LINE_LENGTH, current_file)!=NULL && !feof(current_file)){
		loopCounter++;
		/*getWord = (char*)malloc_safely(sizeof(char)*strlen(getNextWord(current_line, i, &getWord_pos)));*/
		getWord = getNextWord(current_line, i, &getWord_pos);
		newmcrcpyResult = strcmp(getWord, MCR);
		endmcrcpyResult = strcmp(getWord, ENDMCR);
		
		/*This line is using macro*/
		if (searchTable(macros, getWord) == 1){
			macroLinesNum = getTableValues(macros, getWord);
			macroLines = getTableVals(macros, getWord);
			for(j=0;j < macroLinesNum; j++){
				fprintf(amFile, macroLines[j]);
			}
		}
		/*This line is the end of the macro, prints the lines saved in the macro*/
		else if (endmcrcpyResult == 0){
			macroFlag = FALSE;
			free(current_macro);
		}
		/*This line is defining a new macro, saves the macro name in the macros table*/
		else if (newmcrcpyResult == 0){
			macroFlag = TRUE;
			/*Gets the second word - name of macro*/
			nextWord = getNextWord(current_line, getWord_pos+strlen(getWord), &nextWord_pos);
			current_macro = (char*)malloc_safely(strlen(nextWord));
			current_macro = nextWord;
			insertEndTable(&macros, current_macro);
		}
		/*This saves the line in the macro table next to the macro name*/
		else if (macroFlag == TRUE && endmcrcpyResult != 0){
			insertValuesTable(&macros, current_macro, current_line);
		}
		/*This is a regular line*/
		else {
			fprintf(amFile, current_line);
		}

		free(getWord);
	}

	fclose(amFile);
}

