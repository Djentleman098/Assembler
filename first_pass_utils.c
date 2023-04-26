#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_vars.h"
#include "utils.h"
#include "passes.h"
#include "operand_tables.h"

/********* Handles with Labels ********/
int isLabel(char* getWord){/*Checks if a string is a legal label*/
	int result = 1;
	int labelLength = strlen(getWord);
	char currentChar = getWord[labelLength - 1];/*the last character of the label*/
	int i;

	char* tempWord = (char*)malloc_safely(sizeof(char)*labelLength);/*The label without the :*/
	strcpy (tempWord, getWord);
	tempWord[labelLength - 1] = '\0';

	if ((labelLength - 1) > 30)/*illegel length of label, check length wothout :*/
		result = 0;

	if (currentChar != ':')/*last character is not :, illegel label*/
		result = 0;

	currentChar = *getWord;/*the first character*/
	if (currentChar < 'A' || (currentChar > 'Z' && currentChar < 'a') || currentChar > 'z')/*first character is not letter, illegel label*/
		result = 0;
		
	if (labelLength > 2){
		for (i = 1; i < labelLength-1; i++){
			currentChar = getWord[i];
			if (currentChar < '0' || (currentChar > '9' && currentChar < 'A') || (currentChar > 'Z' && currentChar < 'a') || currentChar > 'z')
				result = 0;/*middle character is not letter or digit, illegel label*/	
		}
	}

	for (i = 0; i < 16; i++){/*The label is equal to an operand*/
		if ((strcmp(operandsNames[i], tempWord) == 0))
			result = 0;
	}

	for (i = 0; i < 8; i++){/*The label is equal to a register name*/
		if ((strcmp(registersNames[i], tempWord) == 0))
			result = 0;
	}
	if ((strcmp("PSW", tempWord) == 0) || (strcmp("PC", tempWord) == 0))/*The label is equal to special register name*/
			result = 0;

	return result;
}

/*Saves a label in the labels table*/
void handleWithLabel(char* getWord, int DCIC, char type, labelsTable** labels_table){
	int length = strlen(getWord);/*this few lines removes the : from the label*/
	char* tempWord = (char*)malloc_safely(sizeof(char)*length);
	strcpy (tempWord, getWord);
	tempWord[length-1] = '\0';

	/*saves the label with the position*/
	insertEndLabelsTable(labels_table, tempWord, DCIC, type);
}
/**************************************/


/********* Handles with Data ********/
int isData(char* getWord){/*Checks if a word is a legal data definition*/
	int result = 0;

	/*compares to constant strings*/
	if ((strcmp(DATA, getWord) == 0) || (strcmp(STRING, getWord) == 0))
		result = 1;

	return result;
}

/*this function saves the data of numbers or characters in the data data structure*/
int handleWithData(Data** data, char* getWord, char* line, int pos, int* DC, char* fileName, int lineNum){
	char* vars = (char*)malloc_safely(sizeof(char)*strlen(line));/*the part from the original line after .data*/
	char *current_symbol;
	int current_symbol_index = 0;
	char current_char;
	char* nextWord;
	int getWord_pos;
	int nextWord_pos;
	int length = strlen(line);
	int i = 0;
	int result = 1;
	boolean plusMinusFlag = FALSE;/*true if plus or minus is read now*/
	boolean numFlag = FALSE;/*true if number is read now*/
	boolean commaFlag = FALSE;/*true if comma is read now*/
	boolean firstNumFlag = FALSE;/*true if we read at least one number*/
	boolean readingFlag = FALSE;/*true if we are currently reading a number*/
	int isData;
	char lastChar;
	char* tempWord;

	/*checks if we are currently reading .data*/
	isData = strcmp(getWord, DATA);

	if (isData == 0){/*.data*/
		strncpy(vars, line + (pos+strlen(DATA)), length - (pos+strlen(DATA)));/*saves the reminder of the string after .data*/
		current_char = vars[i];
		current_symbol = (char*)malloc_safely(1);

		while (current_char != '\n'){/*makes sure the numbers and commas are a legal sequence and saves the number to memory*/

			if (current_char == '+' || current_char == '-'){/*makes sure we have +/- in legal positions*/
				if (plusMinusFlag == TRUE){
					printf("ERROR: in file %s line %d - inappropriate +/-.\n", fileName, lineNum);
					result = 0;
				}
				plusMinusFlag = TRUE;
				readingFlag = TRUE;
				commaFlag = FALSE;
				/*tempWord = (char*)malloc_safely(sizeof(char)*(current_symbol_index+1));
				current_symbol = tempWord;*/
				current_symbol = (char*)realloc_safely(current_symbol, sizeof(char)*(current_symbol_index+1));
				current_symbol[current_symbol_index] = current_char;
				current_symbol_index++;
				/*free(tempWord);*/

			} else if (current_char >= '0' && current_char <= '9'){/*we allow reading digits always*/
				readingFlag = TRUE;
				commaFlag = FALSE;
				firstNumFlag = TRUE;
				plusMinusFlag = FALSE;
				/*tempWord = (char*)malloc_safely(sizeof(char)*(current_symbol_index+1));
				current_symbol = tempWord;*/
				current_symbol = (char*)realloc_safely(current_symbol, sizeof(char)*(current_symbol_index+1));
				current_symbol[current_symbol_index] = current_char;
				current_symbol_index++;
				/*free(tempWord);*/

			} else if (current_char == ' ' || current_char == '\t'){/*spaces will make a number end and store it in the data structure*/
				if (plusMinusFlag == TRUE){
					printf("ERROR: in file %s line %d - inappropriate +/-.\n", fileName, lineNum);
					result = 0;
				}
				if (readingFlag == TRUE){
					/*tempWord = (char*)malloc_safely(sizeof(char)*(current_symbol_index+1));
					current_symbol = tempWord;*/
					current_symbol = (char*)realloc_safely(current_symbol, sizeof(char)*(current_symbol_index+1));
					current_symbol[current_symbol_index] = '\0';
					insertToData(data, current_symbol, 'n', lineNum);
					current_symbol_index = 0;
					/*free(current_symbol);*/
					(*DC)++;
				}
				readingFlag = FALSE;
				plusMinusFlag = FALSE;
				
			} else if (current_char == ','){/*comma will make a number end and store it in the data structure*/
				if (firstNumFlag == FALSE){
					printf("ERROR: in file %s line %d - comma before number.\n", fileName, lineNum);
					result = 0;
				}
				if (plusMinusFlag == TRUE){
					printf("ERROR: in file %s line %d - inappropriate +/-.\n", fileName, lineNum);
					result = 0;
				}
				if (commaFlag == TRUE){
					printf("ERROR: in file %s line %d - inappropriate comma.\n", fileName, lineNum);
					result = 0;
				}
				if (readingFlag == TRUE){
					/*tempWord = (char*)malloc_safely(sizeof(char)*(current_symbol_index+1));
					current_symbol = tempWord;*/
					current_symbol = (char*)realloc_safely(current_symbol, sizeof(char)*(current_symbol_index+1));
					current_symbol[current_symbol_index] = '\0';
					insertToData(data, current_symbol, 'n', lineNum);
					current_symbol_index = 0;
					/*free(current_symbol);*/
					(*DC)++;
				}
				readingFlag = FALSE;
				plusMinusFlag = FALSE;
				commaFlag = TRUE;

			} else {
				printf("ERROR: in file %s line %d - not a number.\n", fileName, lineNum);
				result = 0;
			}
			i++;
			current_char = vars[i];
			if (vars[i] == '\n')
				lastChar = vars[i-1];
		}

		if (readingFlag == TRUE && plusMinusFlag == FALSE){/*makes sure to enter the last number if after it there is \n */
			current_symbol = (char*)realloc_safely(current_symbol, sizeof(char)*(current_symbol_index+1));
			current_symbol[current_symbol_index] = '\0';
			insertToData(data, current_symbol, 'n', lineNum);
			current_symbol_index = 0;
			/*free(current_symbol);*/
			(*DC)++;
		}

		if (lastChar == ',' || lastChar == '+' || lastChar == '-'){/*illegal last characters*/
			printf("ERROR: in file %s line %d - illegal last character.\n", fileName, lineNum);
			result = 0;
		}
		
	}

	/*we are currently reading .string*/
	else{
		strncpy(vars, line + (pos+strlen(STRING)), length - (pos+strlen(STRING)));/*the part from the original line after .string*/
		current_symbol = getNextWord(vars, 0, &getWord_pos);
		if (current_symbol[0] != '"' || current_symbol[strlen(current_symbol)-1] != '"'){/*missing first or last " */
			printf("ERROR: in file %s line %d - inappropriate definition of string.\n", fileName, lineNum);
			result = 0;
		}

		nextWord = getNextWord(vars, getWord_pos + strlen(current_symbol), &nextWord_pos);/*nother word - illegal*/
		if (*nextWord != '\0'){
			printf("ERROR: in file %s line %d - to many variables in .string.\n", fileName, lineNum);
			result = 0;
		}
		
		if (result ==1){/*saves each character to data data structure*/
			for (i=1; i<strlen(current_symbol)-1; i++){
				char str1[2] = {current_symbol[i], '\0'};
				char str2[1] = "";
				strcpy(str2, str1);
				insertToData(data, str2	, 'c', lineNum);
				(*DC)++;
			}
			current_char = '\0';/*saves \0 to data data stracture*/
			insertToData(data, &current_char, 'c', lineNum);
			(*DC)++;
		}

		free(current_symbol);
	}

	free(vars);
	return result;
}
/**************************************/

/********* Handles with Extern & entry ********/
int isExIn(char* getWord){/*Checks if a word is a legal extern or entry*/
	int result = 0;

	if ((strcmp(EXTERN, getWord) == 0) || (strcmp(ENTRY, getWord) == 0))
		result = 1;

	return result;
}

int handleWithExIn(char* line, labelsTable** labels_table, int pos, int DC, char* fileName, int lineNum){
	char* tempLabel;
	char* nextWord;
	int nextWord_pos;
	int result = 1;
	
	/*gets the reminder of the string after .extern/.entry*/
	nextWord = getNextWord(line, pos + 7, &nextWord_pos);
	tempLabel = (char*)malloc_safely(sizeof(char) * (strlen(nextWord) + 1));
	strcpy(tempLabel, nextWord);
	strcat(tempLabel, ":");

	/*makes sure the label is a legal one and saves it to labels table*/
	if (isLabel(tempLabel)){
		insertEndLabelsTable(labels_table, nextWord, DC, 'e');
	} else {
		printf("ERROR: in file %s line %d - not a valid Label in .extern.\n", fileName, lineNum);
		result = 0;
	}

	/*makes sure there is no other character after the one label allowed*/
	nextWord = getNextWord(line, nextWord_pos + strlen(nextWord), &nextWord_pos);
	if (*nextWord != '\0'){
		printf("ERROR: in file %s line %d - to many variables in .extern.\n", fileName, lineNum);
		result = 0;
	}

	return result;
}


/**************************************/


/********* Handles with operands ********/
int isOperand(char* getWord){/*Checks if a word is a legal operand*/
	int result = 0;
	int i;
	
	/*compares the strings to operand names array*/
	for (i=0; i<16; i++){
		if ((strcmp(operandsNames[i], getWord) == 0))
			result = 1;
	}

	return result;
}

int handleWithOperand(Instruction** ins_list, char* line, char* operand, int pos, int* IC, char* fileName, int lineNum){
	int result = 1;
	int i;
	
	int lineReminder = strlen(line) - (pos + strlen(operand));
	char* parameters = (char*)malloc_safely(sizeof(char) * lineReminder);

	strncpy(parameters, line + (pos + strlen(operand)), lineReminder);

	/*checks which operand group, for each group continue the error checking and memory insert seperately*/
	for (i=0; i<5; i++){/*group one of operands*/
		if ((strcmp(operandsFirstGroup[i], operand) == 0))
			if (handleWithGroupOne(ins_list, operand, parameters, fileName, lineNum, IC) == 0)
				result = 0;
	}
		
	for (i=0; i<9; i++){/*group two of operands*/
		if ((strcmp(operandsSecondGroup[i], operand) == 0))
			if (handleWithGroupTwo(ins_list, operand, parameters, fileName, lineNum, IC) == 0)
				result = 0;
	}

	for (i=0; i<2; i++){/*group three of operands*/
		if ((strcmp(operandsThirdGroup[i], operand) == 0))
			if (handleWithGroupThree(ins_list, operand, parameters, fileName, lineNum, IC) == 0)
				result = 0;
	}

	return result;
}

/*this function makes sure the group 1 operand got legal operands and saves in to instruction data stracture*/
int handleWithGroupOne(Instruction** ins_list, char* operand, char* vars, char* fileName, int lineNum, int* IC){

	int result = 1;
	char* first_par = (char*)malloc_safely(sizeof(char)*80);
	char* first_temp;
	int first_index = 0;
	int first_adressing_type = NULL;
	char* second_par = (char*)malloc_safely(sizeof(char)*80);
	int second_index = 0;
	int second_adressing_type = NULL;
	char* vars_clean;
	int reminder_index;
	int i = 0;
	char current_char = vars[i];
	boolean commaFlag = FALSE;/*true if comma is read now*/
	boolean firstFlag = FALSE;/*true if we read the first operand*/
	boolean startFirst = FALSE;/*true if we startes to read the first operand*/
	boolean secondFlag = FALSE;/*true if we read the second operand*/
	boolean startSecond = FALSE;/*true if we startes to read the second operand*/
	int commas;

	/*this values will be saved in the instruction data stracture*/
	char* first_par_val;
	char* second_par_val;
	char* addressing2Par1_val;
	int adressing2Par1;
	char* addressing2Par2_val;
	int adressing2Par2;
	char* tempVal;

	int operandPos;
	char* sourceVal;
	char* destVal;

	/*allow spaces at the bigining*/
	while(current_char == ' ' || current_char == '\t'){
		i++;
		current_char = vars[i];
	}
	if (current_char == '\n'){
		printf("ERROR: in file %s line %d - no operands.\n", fileName, lineNum);
		result = 0;
	}
	if (current_char == ','){
		printf("ERROR: in file %s line %d - comma before operands.\n", fileName, lineNum);
		result = 0;
	}
	/*read first word*/
	while(current_char != ' ' && current_char != '\t' && current_char != ',' && current_char != '\n'){
		first_par[first_index] = current_char;
		first_index++;
		i++;
		current_char = vars[i];
	}
	first_par[first_index] = '\0';
	/*allow spaces after first word*/
	while(current_char == ' ' || current_char == '\t'){
		i++;
		current_char = vars[i];
	}
	if (current_char == '\n'){
		printf("ERROR: in file %s line %d - only one operand.\n", fileName, lineNum);
		result = 0;
	}
	/*must have a comma between operands*/
	if (current_char != ','){
		printf("ERROR: in file %s line %d - missing comma.\n", fileName, lineNum);
		result = 0;
	}
	i++;
	current_char = vars[i];
	/*allow spaces after comma*/
	while(current_char == ' ' || current_char == '\t'){
		i++;
		current_char = vars[i];
	}
	if (current_char == '\n'){
		printf("ERROR: in file %s line %d - missing second operand.\n", fileName, lineNum);
		result = 0;
	}
	if (current_char == ','){
		printf("ERROR: in file %s line %d - to many commas.\n", fileName, lineNum);
		result = 0;
	}
	/*read second word*/
	while(current_char != ' ' && current_char != '\t' && current_char != '\n'){
		second_par[second_index] = current_char;
		second_index++;
		i++;
		current_char = vars[i];
	}
	second_par[second_index] = '\0';
	/*allow spaces after second word*/
	while(current_char == ' ' || current_char == '\t'){
		i++;
		current_char = vars[i];
	}
	/*end of line*/
	if (current_char != '\n'){
		printf("ERROR: in file %s line %d - long line.\n", fileName, lineNum);
		result = 0;
	}
	

	if (result != 0){
 	/*determends and saves the adressing type of the operands*/
		first_adressing_type = getAddressingType(first_par, &first_par_val, &addressing2Par1_val, &addressing2Par2_val);
		/*if the addressing type is 2 we need to save all the parameters - addressings and values*/
		if (first_adressing_type == 2){
			adressing2Par1 = getAddressingType(addressing2Par1_val, &tempVal, NULL, NULL);
			if (adressing2Par1 == -1){
				printf("ERROR: in file %s line %d - illegal addressing of parameter.\n", fileName, lineNum);
				result = 0;
			}
		}
		/*again for second operand*/
		second_adressing_type = getAddressingType(second_par, &second_par_val, &addressing2Par1_val, &addressing2Par2_val);
		if (first_adressing_type == 2){
			adressing2Par2 = getAddressingType(addressing2Par2_val, &tempVal, NULL, NULL);
			if (adressing2Par2 == -1){
				printf("ERROR: in file %s line %d - illegal addressing of parameter.\n", fileName, lineNum);
				result = 0;
			}
		}

		/*makes sure this adressing is legal*/
		if (first_adressing_type == -1 || second_adressing_type == -1){
			printf("ERROR: in file %s line %d - illegal adressing.\n", fileName, lineNum);
			result = 0;
		} else {
			/*makes sure the addressing compatible with the operand type - compare to legal addressings table*/
			operandPos = wordPosArray(operandsNames, operand);
			sourceVal = (char*)malloc_safely(sizeof(char));
			sourceVal = adressingSourceOp[operandPos][first_adressing_type + 1];
			destVal = (char*)malloc_safely(sizeof(char));
			destVal = adressingDestOp[operandPos][second_adressing_type + 1];
			if (*sourceVal == 'X' || *destVal == 'X'){
				printf("ERROR: in file %s line %d - illegal adressing type for operand.\n", fileName, lineNum);
				result = 0;
			} else {
			/*save everything in Instruction structure - codes and values*/
				if (first_adressing_type != 2 && second_adressing_type != 2){
					adressing2Par2 = NULL;
					adressing2Par1 = NULL;
					addressing2Par2_val = NULL;
					addressing2Par1_val = NULL;	
				}
				saveToInstruction(ins_list, second_adressing_type, first_adressing_type, operand, adressing2Par2, adressing2Par1, second_par_val, first_par_val, addressing2Par2_val, addressing2Par1_val, lineNum);
				/*increases the IC counter accordingly*/
				(*IC)++;
				if (first_adressing_type == 0)
					(*IC)++;
				if (first_adressing_type == 1)
					(*IC)++;
				if (first_adressing_type == 2){
					if (adressing2Par2 == 3 && adressing2Par1 == 3)/*both are registers*/
						(*IC)+=2;
					else
						(*IC)+=3;	
				}
				if (first_adressing_type == 3 || second_adressing_type == 3)
					(*IC)++;
				if (second_adressing_type == 0)
					(*IC)++;
				if (second_adressing_type == 1)
					(*IC)++;
				if (second_adressing_type == 2){
					if (adressing2Par2 == 3 && adressing2Par1 == 3)/*both are registers*/
						(*IC)+=2;
					else
						(*IC)+=3;	
				}
					

			}
		}
	}
	
	return result;
}

/*this function makes sure the group 2 operand got legal operands and saves in to instruction data stracture*/
int handleWithGroupTwo(Instruction** ins_list, char* operand, char* vars, char* fileName, int lineNum, int* IC){

	int result = 1;
	char* first_par = (char*)malloc_safely(sizeof(char)*80);
	int first_index = 0;
	boolean firstFlag = FALSE;
	int adressing_type = NULL;
	int i = 0;
	char current_char = vars[i];

	char* par_val;
	char* addressing2Par1_val;
	int addressing2Par1;
	char* addressing2Par2_val;
	int addressing2Par2;
	char* tempVal;

	int operandPos;
	char* destVal;

	/*spaces*/
	while(current_char == ' ' || current_char == '\t'){
		i++;
		current_char = vars[i];
	}
	if (current_char == '\n'){
		printf("ERROR: in file %s line %d - missing operand.\n", fileName, lineNum);
		result = 0;
	}
	/*word*/
	while(current_char != ' ' && current_char != '\t' && current_char != '\n'){
		first_par[first_index] = current_char;
		first_index++;
		i++;
		current_char = vars[i];
	}
	first_par[first_index] = '\0';
	/*spaces*/
	while(current_char == ' ' || current_char == '\t'){
		i++;
		current_char = vars[i];
	}
	/*end of line*/
	if (current_char != '\n'){
		printf("ERROR: in file %s line %d - long line.\n", fileName, lineNum);
		result = 0;
	}

	if (result != 0){
		/*determends and saves the adressing type of the operands*/
		adressing_type = getAddressingType(first_par, &par_val, &addressing2Par1_val, &addressing2Par2_val);

		/*if the addressing type is 2 we need to save all the parameters - addressings and values*/
		if (adressing_type == 2){
			addressing2Par1 = getAddressingType(addressing2Par1_val, &tempVal, NULL, NULL);
			
			if (addressing2Par1 == -1){
				printf("ERROR: in file %s line %d - illegal addressing of parameter.\n", fileName, lineNum);
				result = 0;
			}
			addressing2Par2 = getAddressingType(addressing2Par2_val, &tempVal, NULL, NULL);
			
			if (addressing2Par2 == -1){
				printf("ERROR: in file %s line %d - illegal addressing of parameter.\n", fileName, lineNum);
				result = 0;
			}
		}
		/*makes sure this adressing is legal*/
		if (adressing_type == -1){
			printf("ERROR: in file %s line %d - illegal adressing.\n", fileName, lineNum);
			result = 0;
		} else {
			/*makes sure the addressing compatible with the operand type*/
			operandPos = wordPosArray(operandsNames, operand);
			destVal = (char*)malloc_safely(sizeof(char));
			destVal = adressingDestOp[operandPos][adressing_type + 1];
			if (*destVal == 'X'){
				printf("ERROR: in file %s line %d - illegal adressing type for operand.\n", fileName, lineNum);
				result = 0;
			} else {
			/*save everything in Instruction structure - codes and values*/
				if (adressing_type != 2){
					addressing2Par2 = NULL;
					addressing2Par1 = NULL;
					addressing2Par2_val = NULL;
					addressing2Par1_val = NULL;	
				}
				saveToInstruction(ins_list, adressing_type, -1, operand, addressing2Par2, addressing2Par1, par_val, NULL, addressing2Par2_val, addressing2Par1_val, lineNum);
				/*increases the IC counter accordingly*/
				(*IC)++;
				if (adressing_type == 0)
					(*IC)++;
				if (adressing_type == 1)
					(*IC)++;
				if (adressing_type == 2){
					if (addressing2Par2 == 3 && addressing2Par1 == 3)/*both are registers*/
						(*IC)+=2;
					else
						(*IC)+=3;	
				}
				if (adressing_type == 3)
					(*IC)++;
			}
		}
	}

	return result;
}

/*this function makes sure the group 3 operand got no operands and saves in to instruction data stracture*/
int handleWithGroupThree(Instruction** ins_list, char* operand, char* vars, char* fileName, int lineNum, int* IC){
	int result = 1;
	int i = 0;
	char current_char = vars[i];
	
	/*allow spaces before operand*/
	while(current_char == ' ' || current_char == '\t'){
		i++;
		current_char = vars[i];
	}
	/*end of line*/
	if (current_char != '\n'){
		printf("ERROR: in file %s line %d - long line.\n", fileName, lineNum);
		result = 0;
	}

	/*save in instruction data stracture*/
	if (result != 0){
		saveToInstruction(ins_list, -1, -1, operand, NULL, NULL, NULL, NULL, NULL, NULL, lineNum);
		(*IC)++;
	}

	return result;
}

/*this function checks if an addressing is 0 and saves the value*/
int isAddressingMiyadi(char* parameter, char** val){
	/*the extra word is the number in 12 bits, with ARE*/
	int result = 1;
	char firstChar = parameter[0];
	char* num = (char*)malloc_safely(sizeof(char) * (strlen(parameter) - 1));
	
	num = strncpy(num, parameter + 1, strlen(parameter) - 1);
	num[strlen(parameter) - 1] = '\0';

	if (firstChar != '#')/*checks if first character is #*/
		result = 0;

	if (legalNumber(num) == 0)/*checks if the rest of the string is a number*/
		result = 0;
	else{
		if (result != 0){
			*val = (char*)malloc_safely(sizeof(char) * strlen(num));
			*val = num;
		}
	}

	/*free(num);*/
	return result;	
}

/*this function checks if an addressing is 1 and saves the value*/
int isAddressingYashir(char* parameter, char** val){
	/*the extra word is the place in memory of the label in 12 bits, with ARE*/
	int result = 0;
	int length = strlen(parameter);
	char* label = (char*)malloc_safely(sizeof(char) * (length + 2));
	strcpy(label, parameter);
	label[length] = 58;
	label[length+1] = '\0';
	
	if (isLabel(label)){
		result = 1;
		label[length] = '\0';
		*val = (char*)malloc_safely(sizeof(char) * strlen(label));
		*val = strcpy(*val, label);
	}
	
	/*free(label);*/
	return result;
}

/*this function checks if an addressing is 2 and saves all the value*/
int isAddressingJump(char* parameter, char** val, char** addressing2Par1_val, char** addressing2Par2_val){

	int result = 1;
	char* label = (char*)malloc_safely(sizeof(char)*80);
	int label_length = 0;
	char* first_par = (char*)malloc_safely(sizeof(char)*80);
	int first_length = 0;
	char* second_par = (char*)malloc_safely(sizeof(char)*80);
	int second_length = 0;
	int i = 0;
	char current_char = parameter[i];
	char* tempLabel;
	boolean labelFlag = FALSE;
	boolean firstFlag = FALSE;
	boolean secondFlag = FALSE;
	int first_type = NULL;
	int second_type = NULL;

	/*makes sure the syntax is correct*/
	/*label*/
	while (current_char != '('){
		if (current_char == '\0'){
			result = 0;
			break;
		}
		label[label_length] = current_char;
		label_length++;
		i++;
		current_char = parameter[i];
	}
	label[label_length] = '\0';
	/* ( */
	i++;
	current_char = parameter[i];
	/*comma*/
	while (current_char != ','){
		if (current_char == '\0'){
			result = 0;
			break;
		}
		first_par[first_length] = current_char;
		first_length++;
		i++;
		current_char = parameter[i];
	}
	first_par[first_length] = '\0';
	/* , */
	i++;
	current_char = parameter[i];
	/*comma*/
	while (current_char != ')'){
		if (current_char == '\0'){
			result = 0;
			break;
		}
		second_par[second_length] = current_char;
		second_length++;
		i++;
		current_char = parameter[i];
	}
	second_par[second_length] = '\0';
	/* ) */
	i++;
	current_char = parameter[i];
	/*end of var*/
	if (current_char != '\0')
		result = 0;

	/*makes sure the label is realy a label*/
	tempLabel = (char*)malloc_safely(sizeof(char)*(label_length+2));
	tempLabel = strcpy(tempLabel, label);
	tempLabel[label_length] = 58;
	tempLabel[label_length+1] = '\0';
	if (isLabel(tempLabel) == 0)
		result = 0;

	/*saves all the variables - main label and 2 parameters*/
	if (result != 0 && addressing2Par1_val != NULL && addressing2Par2_val != NULL){
		tempLabel[label_length] = '\0';
		*val = (char*)malloc_safely(sizeof(char)*strlen(tempLabel));
		strcpy(*val, label);
		*addressing2Par1_val = (char*)malloc_safely(sizeof(char)*strlen(first_par));
		*addressing2Par2_val = (char*)malloc_safely(sizeof(char)*strlen(second_par));
		strcpy(*addressing2Par1_val, first_par);		
		strcpy(*addressing2Par2_val, second_par);
	}

	free(label);
	free(first_par);
	free(second_par);
	free(tempLabel);

	return result;
}

/*this function checks if an addressing is 3 and saves the value*/
int isAddressingRegister(char* parameter, char** val){

	int result = 0;
	int i;
	
	for (i=0; i<8; i++){
		if ((strcmp(registersNames[i], parameter) == 0)){
			result = 1;
			*val = (char*)malloc_safely(sizeof(char)*strlen(parameter));
			*val = parameter;
		}
	}

	return result;
}

/*this function calls all the addressing options and returns the value*/
int getAddressingType(char* parameter, char** val, char** addressing2Par1_val, char** addressing2Par2_val){
	int result = -1;
	
	/*miyadi - 0*/
	if (isAddressingMiyadi(parameter, val))
		return 0;	
	/*yashir - 1*/
	if (isAddressingYashir(parameter, val))
		return 1;
	/*register - 3*/
	if (isAddressingRegister(parameter, val))
		return 3;
	/*jump - 2*/
	if (isAddressingJump(parameter, val, addressing2Par1_val, addressing2Par2_val))
		return 2;

	return result;
}
/**************************************/
