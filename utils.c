#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/*Creates file name with ending*/
char* createFIleName(char* fName, char* ending){
	char* fullName = (char*) malloc_safely (strlen(fName) + strlen(ending) + 1);
	strcpy(fullName, fName);
	strcat(fullName, ending);
	return fullName;
}

/*Allocate memory safely*/
void* malloc_safely (long value){
	void *temp = malloc(value);
	if (temp == NULL){
		printf("\nERROR: Mempry allocation Failed.");
		exit(1);
	}
	return temp;
}

/*Reallocate memory safely*/
void* realloc_safely (void* ptr, long value){
	void *temp = realloc(ptr, value);
	if (temp == NULL){
		printf("\nERROR: Mempry reallocation Failed.");
		exit(1);
	}
	return temp;
}

/*Returns a word - characters not including space,tab, from a specific place in the string*/
char* getNextWord(char* string, int i, int* wordPos){
	int pos = i;
	int len = 0;
	int tempPos;
	char * returnWord;

	if (string[pos] == '\n'){
		returnWord = (char*)malloc_safely(1);
		*returnWord = '\0';
		return returnWord;
	}
	
	while (string[pos] == '\t' || string[pos] == ' '){
		pos++;
	}

	tempPos = pos;
	*wordPos = pos;
	while (string[tempPos] != '\t' && string[tempPos] != ' ' && string[tempPos] != '\n'){
		len++;
		tempPos++;			
	}
	if (len > 0){
		returnWord = (char*)malloc_safely(len);
		strncpy(returnWord, string+(pos), len);
		*(returnWord + len) = '\0';
		return returnWord;
	}else{
		returnWord = (char*)malloc_safely(1);
		*returnWord = '\0';
		return returnWord;
	}
}

/*Returns a word - characters not including space,tab or comma from a specific place in the string*/
char* getNextWordComma(char* string, int i, int* wordPos){
	int pos = i;
	int len = 0;
	int tempPos;
	char * returnWord;

	if (string[pos] == '\n'){
		returnWord = (char*)malloc_safely(1);
		*returnWord = '\0';
		return returnWord;
	}
	
	while (string[pos] == '\t' || string[pos] == ' '){
		pos++;
	}

	tempPos = pos;
	*wordPos = pos;
	while (string[tempPos] != '\t' && string[tempPos] != ' ' && string[tempPos] != ',' && string[tempPos] != '\n'){
		len++;
		tempPos++;			
	}
	if (len > 0){
		returnWord = (char*)malloc_safely(len);
		strncpy(returnWord, string+(pos), len);
		*(returnWord + len) = '\0';
		return returnWord;
	}else{
		returnWord = (char*)malloc_safely(1);
		*returnWord = '\0';
		return returnWord;
	}
}


/*Returns string without tabs and spaces*/
char* removeTabsSpaces(char* word){
	int counter = 0;
	char current_char = word[0];
	int i=0;
	char * result;
	int result_index = 0;
	
	while(current_char != '\n'){
		if (current_char == '\t' || current_char == ' ')
			counter++;
		i++;
		current_char = word[i];
	}

	result = (char*)malloc_safely(sizeof(char)*(strlen(word)-counter));/*allocate original size minus amount of tabs and spaces*/

	for (i = 0; i<strlen(word); i++){
		current_char = word[i];
		if (current_char != '\t' && current_char != ' '){
			result[result_index] = current_char;
			result_index++;
		}
	}
	
	return result;
}

/*Returns true is a string is in an array of strings*/
int wordInArray (char* array[], char* word){
	int result = 0;
	int i = 0;
	char* current_word = (char*)malloc_safely(sizeof(char) * strlen(array[i]));
	current_word = array[i];

	while (strcmp(current_word ,"\0") != 0){
		if (strcmp(current_word, word) == 0)
			result = 1;
	
		i++;
		current_word = (char*)realloc_safely(current_word, sizeof(char) * strlen(array[i]));
		current_word = array[i];
	}

	return result;
}

/*Returns position of a string  in an array of strings*/
int wordPosArray (char* array[], char* word){
	
	int result = NULL;
	int i = 0;

	while (strcmp(array[i], "\0") != 0){
		if (strcmp(array[i], word) == 0)
			return i;
	
		i++;
	}

	return result;
}

/*returns the length of the longest word in an array*/
int longestWordInArray (char* array[]){
	int result = 0;
	int i = 0;
	int length;
	char* temp = (char*)malloc_safely(sizeof(char) * 80);
	temp = array[i];
	
	while (strcmp(temp, "\0") != 0){
		temp = array[i];
		length = strlen(array[i]);
		temp[length] = '\0';
		if (length > result)
			result = length;
		i++;
		temp = array[i];
	}
	return result;
}

/*returns true or false if a string represens a legal number - +/- and then sigits*/
int legalNumber (char* num){
	int result = 1;
	int i;
	char current_char;
	int length = strlen (num);
	
	for (i = 0; i<length; i++){
		current_char = num[i];
		if ((current_char == '+' || current_char == '-') && i > 0)/*if +/- is not on the first char*/
			result = 0;
		if (current_char != '+' && current_char != '-' && (current_char > '9' || current_char < '0'))
			result = 0;
	}
	
	return result;
}

/*returns the position of a character in a string*/
int howManyChars(char* string, char c){
	int result = 0;
	int length = strlen(string);
	int i;
	char current;

	for(i = 0;i<length; i++){
		current = string[i];
		if (current == c)
			result++;
	}

	return result;
}

/*returns integer the string represents*/
int stringToNum(char* value){
	int result;
	char first = value[0];
	char* cleanValue;

	if (first == '-' || first == '+'){
		cleanValue = (char*)malloc_safely(sizeof(char)*(strlen(value)-1));
		strncpy(cleanValue, value + 1, strlen(value)-1);
	} else {
		cleanValue = (char*)malloc_safely(sizeof(char)*strlen(value));
		strcpy(cleanValue, value);
	}

	result = atoi(cleanValue);

	if (first == '-' && result != 0){
		result = 1 + ~result;
	}

	
	return result;
}

/*returns an array that represents a number in binary with 14 bits*/
int* numToBitArray(int num){
	int i;
	int *array = (int*)malloc_safely(sizeof(int)*14);
	int negFlag = 0;
	int *onesComp = (int*)malloc_safely(sizeof(int)*14);
	int *twosComp = (int*)malloc_safely(sizeof(int)*14);
	int carry = 1;
	array[0] = 0, array[1] = 0, array[2] = 0, array[3] = 0, array[4] = 0, array[5] = 0, array[6] = 0, array[7] = 0, array[8] = 0, array[9] = 0, array[10] = 0, array[11] = 0, array[12] = 0, array[13] = 0;

	if (num < 0){/*checks if the number is negative*/
		num = num*-1;
		negFlag = 1;
	}
	
	for (i = 0; num > 0; i++){/*create the binary array*/
		array[13-i] = num % 2;
		num = num/2;
	}
	
	for (i = 0; i<14; i++){/*create ones compiment*/
		if (array[i] == 0)
			onesComp[i] = 1;
		else if (array[i] == 1)
			onesComp[i] = 0;
	}

	for (i = 13; i>=0; i--){/*create twos compiment*/
		if (onesComp[i] == 1 && carry == 1)
			twosComp[i] = 0;
		else if (onesComp[i] == 0 && carry == 1){
			twosComp[i] = 1;
			carry = 0;
		} else 
			twosComp[i] = onesComp[i];
	}

	if (negFlag == 1)/*return twos compliment for negatives*/
		return twosComp;
	else
		return array;
}

/*returns an array that represents a number in binary with 12 bits*/
int* numToBitArray12(int num){
	int i;
	int *array = (int*)malloc_safely(sizeof(int)*14);
	int negFlag = 0;
	int *onesComp = (int*)malloc_safely(sizeof(int)*14);
	int *twosComp = (int*)malloc_safely(sizeof(int)*14);
	int carry = 1;
	array[0] = 0, array[1] = 0, array[2] = 0, array[3] = 0, array[4] = 0, array[5] = 0, array[6] = 0, array[7] = 0, array[8] = 0, array[9] = 0, array[10] = 0, array[11] = 0;

	if (num < 0){/*checks if the number is negative*/
		num = num*-1;
		negFlag = 1;
	}
	
	for (i = 0; num > 0; i++){/*create the binary array*/
		array[11-i] = num % 2;
		num = num/2;
	}
	
	for (i = 0; i<11; i++){/*create ones compiment*/
		if (array[i] == 0)
			onesComp[i] = 1;
		else if (array[i] == 1)
			onesComp[i] = 0;
	}

	for (i = 11; i>=0; i--){/*create twos compiment*/
		if (onesComp[i] == 1 && carry == 1)
			twosComp[i] = 0;
		else if (onesComp[i] == 0 && carry == 1){
			twosComp[i] = 1;
			carry = 0;
		} else 
			twosComp[i] = onesComp[i];
	}

	if (negFlag == 1)/*return twos compliment for negatives*/
		return twosComp;
	else
		return array;
}

/*returns an array that represents a number in binary with 6 bits*/
int* numToBitArray6(int num){
	int i;
	int *array = (int*)malloc_safely(sizeof(int)*6);
	int negFlag = 0;
	int *onesComp = (int*)malloc_safely(sizeof(int)*6);
	int *twosComp = (int*)malloc_safely(sizeof(int)*6);
	int carry = 1;
	array[0] = 0, array[1] = 0, array[2] = 0, array[3] = 0, array[4] = 0, array[5] = 0;
	
	if (num < 0){/*checks if the number is negative*/
		num = num*-1;
		negFlag = 1;
	}

	for (i = 0; num > 0; i++){/*create the binary array*/
		array[5-i] = num % 2;
		num = num/2;
	}
	
	for (i = 0; i<6; i++){/*create ones compiment*/
		if (array[i] == 0)
			onesComp[i] = 1;
		else if (array[i] == 1)
			onesComp[i] = 0;
	}

	for (i = 5; i>=0; i--){/*create twos compiment*/
		if (onesComp[i] == 1 && carry == 1)
			twosComp[i] = 0;
		else if (onesComp[i] == 0 && carry == 1){
			twosComp[i] = 1;
			carry = 0;
		} else 
			twosComp[i] = onesComp[i];
	}

	if (negFlag == 1)/*return twos compliment for negatives*/
		return twosComp;
	else
		return array;
}


