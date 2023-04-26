#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dataStructs.h"
#include "utils.h"
#include "global_vars.h"

/*Saves data values*/
void insertToData(Data** data, char* value, char type, int lineNum){
	Data *temp, *ptr;
	temp = (Data*)malloc_safely(sizeof(Data));

	temp->value = (char*)malloc_safely(sizeof(char)*strlen(value));
	temp->value = strcpy(temp->value, value);

	temp->type = (char*)malloc_safely(sizeof(char));
	temp->type = type;
	
	temp->lineNum = (int*)malloc_safely(sizeof(int));
	temp->lineNum = lineNum;

	temp->next = NULL;

	if (*data == NULL)
		*data = temp;
	else {
		ptr = *data;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
}

/*Saves instruction values*/
void saveToInstruction(Instruction** ins, int second_adressing_type, int first_adressing_type, char* operand, int addressing2Par2, int addressing2Par1, char* second_par_val, char* first_par_val, char* addressing2Par2_val, char* addressing2Par1_val, int lineNum){

	Instruction *temp, *ptr;
	temp = (Instruction*)malloc_safely(sizeof(Instruction));
	
	if (second_adressing_type != NULL){
		temp->adressingSource = (int*)malloc_safely(sizeof(int));
		temp->adressingDest = second_adressing_type;
	} else {
		temp->adressingDest = NULL;
	}

	if (first_adressing_type != NULL){
		temp->adressingSource = (int*)malloc_safely(sizeof(int));
		temp->adressingSource = first_adressing_type;
	} else {
		temp->adressingSource = NULL;
	}

	temp->opcode = (char*)malloc_safely(sizeof(char)*strlen(operand));
	temp->opcode = strcpy(temp->opcode, operand);

	if (addressing2Par2 != NULL){
		temp->secondPar = (int*)malloc_safely(sizeof(int));
		temp->secondPar = addressing2Par2;
	} else {
		temp->secondPar = NULL;
	}

	if (addressing2Par1 != NULL){
		temp->firstPar = (int*)malloc_safely(sizeof(int));
		temp->firstPar = addressing2Par1;
	} else {
		temp->firstPar = NULL;
	}

	if (addressing2Par2_val != NULL){
		temp->secondParVal = (char*)malloc_safely(sizeof(char)*strlen(addressing2Par2_val));
		temp->secondParVal = strcpy(temp->secondParVal, addressing2Par2_val);
	} else {
		temp->secondParVal = NULL;
	}

	if (addressing2Par1_val != NULL){
		temp->firstParVal = (char*)malloc_safely(sizeof(char)*strlen(addressing2Par1_val));
		temp->firstParVal = strcpy(temp->firstParVal, addressing2Par1_val);
	} else {
		temp->firstParVal = NULL;
	}

	if (second_par_val != NULL){
		temp->adressingDestVal = (char*)malloc_safely(sizeof(char)*strlen(second_par_val));
		temp->adressingDestVal = strcpy(temp->adressingDestVal, second_par_val);
	} else {
		temp->adressingDestVal = NULL;
	}

	if (first_par_val != NULL){
		temp->adressingSourceVal = (char*)malloc_safely(sizeof(char)*strlen(first_par_val));
		temp->adressingSourceVal = strcpy(temp->adressingSourceVal, first_par_val);
	} else {
		temp->adressingSourceVal = NULL;
	}

	temp->lineNum = (int*)malloc_safely(sizeof(int));
	temp->lineNum = lineNum;

	temp->ARE = NULL;
	temp->next = NULL;

	if (*ins == NULL)
		*ins = temp;
	else {
		ptr = *ins;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
}


/*Labels table data structure functions. type d = data, c = code, e = exrtern, n=entry*/
void insertEndLabelsTable(labelsTable** table, char* word, int val, char type){
	labelsTable *temp, *ptr;
	temp = (labelsTable*)malloc_safely(sizeof(labelsTable));

	temp->name = (char*)malloc_safely(sizeof(char)*strlen(word));
	temp->name = strcpy(temp->name, word);
	temp->value = val;
	temp->type = type;
	temp->next = NULL;

	if(*table == NULL){
		*table = temp;
	}
	else{
		ptr = *table;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
	
}

/*insert new macro*/
void insertEndTable(Table** tb, char* rowName){
	Table *temp, *ptr;
	
	temp = (Table*)malloc_safely(sizeof(Table));
	temp->row = (char*)malloc_safely(sizeof(strlen(rowName)));
	temp->row = strcpy(temp->row, rowName);
	temp->val = NULL;
	temp->values = 0;
	temp->next = NULL;

	if (*tb == NULL){
		*tb = temp;
	}
	else {
		ptr = *tb;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
}

/*returns true if a macro is in the table*/
int searchTable(Table* tb, char* rowName){
	
	Table *temp = tb;
	int result = 0;
	int cmp;
	
	while (temp != NULL){
		cmp = strcmp(temp->row, rowName);
		if (cmp == 0)
			result = 1;
		temp = temp->next;
	}
	return result;
}

/*insert a new line to the macro data stracture*/
void insertValuesTable(Table** tb, char* rowName, char* value){
	Table *temp;
	temp = getTableNode (*tb, rowName);
	char** temp_val = (char**)malloc_safely(sizeof(char*));
	char** current_val = temp->val;

	int values = temp->values;
	int newValues = 0;
	int i;

	/*gets the existing lines*/
	for (i = 0;i<values;i++){
		temp_val = (char**)realloc_safely(temp_val, sizeof(char*)*(newValues + 1));
		temp_val[newValues] = (char*)malloc_safely(sizeof(char)*(strlen(current_val[newValues]) + 1));
		temp_val[newValues] = strcpy(temp_val[newValues], current_val[newValues]);
		newValues++;
	}
	/*adds the new line*/
	temp_val = (char**)realloc_safely(temp_val, sizeof(char*)*(newValues + 1));
	temp_val[newValues] = (char*)malloc_safely(sizeof(char)*(strlen(value) + 1));
	temp_val[newValues] = strcpy(temp_val[newValues], value);
	newValues++;

	temp->values = newValues;
	temp->val = temp_val;	
}

/*returns the number of lines in a macro*/
int getTableValues(Table* tb, char* rowName){
	int result;
	Table* temp = tb;
	
	while (temp != NULL){
		if (strcmp(temp->row, rowName) == 0){
			result = temp->values;
		}
				
		temp = temp->next;
	}

	return result;
}

/*returns the array of lines in the macro*/
char** getTableVals(Table* tb, char* rowName){
	char** result;
	Table* temp = tb;
	
	while (temp != NULL){
		if (strcmp(temp->row, rowName) == 0){
			result = (char**)malloc_safely(sizeof(char*)*temp->values);
			result = temp->val;
		}
				
		temp = temp->next;
	}

	return result;
}

/*prints the macro table*/
void printTable (Table* tb){
	Table* temp = tb;
	int pos;
	int i;

	while (temp != NULL){
		printf("\n*********");
		printf("\nRow name is: %s", temp->row);

		pos = temp->values;
		for (i = 0; i< pos; i++){
			printf("\nLine %d: %s", i, *(temp->val + i));
		}

		printf("\nValues: %d\n", pos);
		printf("\n*********");
				
		temp = temp->next;
	}
}

/*returns a specific macro from macros*/
Table* getTableNode(Table* tb, char* rowName){
	Table* temp = tb;
	Table* result = NULL;
	while (temp != NULL){
		if (strcmp(temp->row, rowName) == 0)
			result = temp;

		temp = temp->next;
	}

	return result;
}

/*checks if a label (string) exists in a labels table*/
int existsInLabels(labelsTable* lb, char* label){
	int result = 0;
	labelsTable* temp = lb;

	while (temp != NULL){
		if (strcmp(temp->name, label) == 0 && temp->type != 'e')
			result = 1;

		temp = temp->next;
	}

	return result;
}

/*prints the labels table to the command line*/
void printLabelsTable(labelsTable* lb){
	labelsTable* temp = lb;

	while (temp != NULL){
		printf("\n*********");
		printf("\nName is: %s", temp->name);

		printf("\nValue: %d", temp->value);

		printf("\nType: %c", temp->type);
		printf("\n*********");
				
		temp = temp->next;
	}
}

/*prints the instructions to the command line*/
void printInstructions(Instruction* ins){
	Instruction* temp = ins;

	while (temp != NULL){
		printf("\n*********");
		printf("\nARE is: %d", temp->ARE);
		printf("\nadressingDest: %d", temp->adressingDest);
		printf("\nadressingSource: %d", temp->adressingSource);
		printf("\nopcode: %s", temp->opcode);
		printf("\nsecondPar: %d", temp->secondPar);
		printf("\nfirstPar: %d", temp->firstPar);
		printf("\nadressingDestVal: %s", temp->adressingDestVal);
		printf("\nadressingSourceVal: %s", temp->adressingSourceVal);
		printf("\nsecondParVal: %s", temp->secondParVal);
		printf("\nfirstParVal: %s", temp->firstParVal);
		printf("\nlineNum: %d", temp->lineNum);
		printf("\n*********");
				
		temp = temp->next;
	}
}

/*prints the data to the command line*/
void printData(Data* data){
	Data* temp = data;

	while (temp != NULL){
		printf("\n*********");
		printf("\nValue is: %s", temp->value);

		printf("\nType: %c", temp->type);
		printf("\nlineNum: %d", temp->lineNum);
		printf("\n*********");
				
		temp = temp->next;
	}	
}

/*update the value of all lables of a specific type*/
void updateLabelsValues(labelsTable** lb, int IC, char type){
	labelsTable* temp = *lb;

	while (temp != NULL){

		if (temp->type == type){
			temp->value += IC;
		}		
				
		temp = temp->next;
	}
}

/*changes a specific label in table to type entry*/
void changeLabelToEntry(labelsTable** lb, char* label){
	labelsTable *temp;
	temp = *lb;

	while(temp != NULL){
		if (strcmp(temp->name, label) == 0)
			temp->type = 'n';

		temp = temp->next;
	}
}

/*returns a character - the type of the label we got on the variable*/
char getLabelType(labelsTable** lb, char* label){
	char result = NULL;
	labelsTable* temp = *lb;

	if (label == NULL)
		return NULL;

	while (temp != NULL){

		if (strcmp(temp->name, label) == 0){
			result = temp->type;
		}		
				
		temp = temp->next;
	}
	return result;	
}

/*calculates ARE value*/
void calcARE(Instruction** ins, labelsTable** lb, int lineNum){
	Instruction* temp;
	labelsTable* tempLb;
	temp = *ins;
	tempLb = *lb;
	

	while (temp != NULL){
		if (temp->lineNum == lineNum){/*we are in the correnct line*/
			temp->ARE = (int*)malloc_safely(sizeof(int));
			if (temp->adressingDest == 1 || temp->adressingDest == 2 || temp->adressingSource == 1 || temp->adressingSource == 2 || temp->secondPar == 1 || temp->firstPar == 1){
				temp->ARE = 2;
				if (getLabelType(lb, temp->adressingDestVal) == 'e' || getLabelType(lb, temp->adressingSourceVal) == 'e' || getLabelType(lb, temp->secondParVal) == 'e' || getLabelType(lb, temp->firstParVal) == 'e')
					temp->ARE = 1;
			} else 
				temp->ARE = 0;
		}

		temp = temp->next;
	}
	

}

/*insert an instruction to memory*/
int* insertInstruction(Instruction* ins){
	int* line = (int*)malloc_safely(sizeof(int)*14);
	int j;
	/*ARE*/
	if (ins->ARE == 0){
		line[13-0] = 0;
		line[13-1] = 0;
	} else if (ins->ARE == 1){
		line[13-0] = 1;
		line[13-1] = 0;		
	} else {
		line[13-0] = 0;
		line[13-1] = 1;	
	}
	/*dest addressing*/	
	if (ins->adressingDest == 0 || ins->adressingDest == -1){
		line[13-2] = 0;
		line[13-3] = 0;
	} else if (ins->adressingDest == 1){
		line[13-2] = 1;
		line[13-3] = 0;		
	} else if (ins->adressingDest == 2){
		line[13-2] = 0;
		line[13-3] = 1;		
	} else {
		line[13-2] = 1;
		line[13-3] = 1;
	}
	/*source addressing*/	
	if (ins->adressingSource == 0 || ins->adressingSource == -1){
		line[13-4] = 0;
		line[13-5] = 0;
	} else if (ins->adressingSource == 1){
		line[13-4] = 1;
		line[13-5] = 0;		
	} else if (ins->adressingSource == 2){
		line[13-4] = 0;
		line[13-5] = 1;		
	} else {
		line[13-4] = 1;
		line[13-5] = 1;
	}
	/*opcode*/
	if (strcmp(ins->opcode, "mov") == 0){
		line[13-6] = 0;
		line[13-7] = 0;
		line[13-8] = 0;
		line[13-9] = 0;
	} else if (strcmp(ins->opcode, "cmp") == 0){
		line[13-6] = 1;
		line[13-7] = 0;
		line[13-8] = 0;
		line[13-9] = 0;	
	} else if (strcmp(ins->opcode, "add") == 0){
		line[13-6] = 0;
		line[13-7] = 1;
		line[13-8] = 0;
		line[13-9] = 0;	
	} else if (strcmp(ins->opcode, "sub") == 0){
		line[13-6] = 1;
		line[13-7] = 1;
		line[13-8] = 0;
		line[13-9] = 0;	
	} else if (strcmp(ins->opcode, "not") == 0){
		line[13-6] = 0;
		line[13-7] = 0;
		line[13-8] = 1;
		line[13-9] = 0;	
	} else if (strcmp(ins->opcode, "clr") == 0){
		line[13-6] = 1;
		line[13-7] = 0;
		line[13-8] = 1;
		line[13-9] = 0;	
	} else if (strcmp(ins->opcode, "lea") == 0){
		line[13-6] = 0;
		line[13-7] = 1;
		line[13-8] = 1;
		line[13-9] = 0;	
	} else if (strcmp(ins->opcode, "inc") == 0){
		line[13-6] = 1;
		line[13-7] = 1;
		line[13-8] = 1;
		line[13-9] = 0;	
	} else if (strcmp(ins->opcode, "dec") == 0){
		line[13-6] = 0;
		line[13-7] = 0;
		line[13-8] = 0;
		line[13-9] = 1;	
	} else if (strcmp(ins->opcode, "jmp") == 0){
		line[13-6] = 1;
		line[13-7] = 0;
		line[13-8] = 0;
		line[13-9] = 1;	
	} else if (strcmp(ins->opcode, "bne") == 0){
		line[13-6] = 0;
		line[13-7] = 1;
		line[13-8] = 0;
		line[13-9] = 1;	
	} else if (strcmp(ins->opcode, "red") == 0){
		line[13-6] = 1;
		line[13-7] = 1;
		line[13-8] = 0;
		line[13-9] = 1;	
	} else if (strcmp(ins->opcode, "prn") == 0){
		line[13-6] = 0;
		line[13-7] = 0;
		line[13-8] = 1;
		line[13-9] = 1;	
	} else if (strcmp(ins->opcode, "jsr") == 0){
		line[13-6] = 1;
		line[13-7] = 0;
		line[13-8] = 1;
		line[13-9] = 1;	
	} else if (strcmp(ins->opcode, "rts") == 0){
		line[13-6] = 0;
		line[13-7] = 1;
		line[13-8] = 1;
		line[13-9] = 1;	
	} else{
		line[13-6] = 1;
		line[13-7] = 1;
		line[13-8] = 1;
		line[13-9] = 1;	
	}
	/*parameter 2*/	
	if (ins->secondPar == 0 || ins->secondPar == NULL){
		line[13-10] = 0;
		line[13-11] = 0;
	} else if (ins->secondPar == 1){
		line[13-10] = 1;
		line[13-11] = 0;		
	} else if (ins->secondPar == 2){
		line[13-10] = 0;
		line[13-11] = 1;		
	} else {
		line[13-10] = 1;
		line[13-11] = 1;
	}
	/*parameter 1*/	
	if (ins->firstPar == 0 || ins->firstPar == NULL){
		line[13-12] = 0;
		line[13-13] = 0;
	} else if (ins->firstPar == 1){
		line[13-12] = 1;
		line[13-13] = 0;		
	} else if (ins->firstPar == 2){
		line[13-12] = 0;
		line[13-13] = 1;		
	} else {
		line[13-12] = 1;
		line[13-13] = 1;
	}

	return line;
}

/*inserts the extra words to memory - return an int array of bits*/
/*direction = dest/source, addressing2Type = pars, par1, par2*/
int* insertWordToMemory(Instruction* ins, labelsTable* lb, int addressing, char* direction, char* addressing2Type){

	int* result = (int*)malloc_safely(sizeof(int)*14);
	labelsTable* temp;
	temp = lb;
	
	/*MITADI*/
	if (addressing == 0){
		if (strcmp(direction, "dest") == 0){
			result = numToBitArray12(atoi(ins->adressingDestVal));
		} else {
			result = numToBitArray12(atoi(ins->adressingSourceVal));
		}

	/*YASHIR*/
	} else if (addressing == 1){
		if (strcmp(direction, "dest") == 0){
			while (temp != NULL){
				if (strcmp(ins->adressingDestVal, temp->name) == 0)				
					result = numToBitArray12(temp->value);
				temp = temp->next;
			}
		} else {
			while (temp != NULL){
				if (strcmp(ins->adressingSourceVal, temp->name) == 0)				
					result = numToBitArray12(temp->value);
				temp = temp->next;
			}
		}

	/*JUMP*/
	} else if (addressing == 2){
		if (strcmp(direction, "dest") == 0){
			if (strcmp(addressing2Type, "val") == 0){
				while (temp != NULL){
					if (strcmp(ins->adressingDestVal, temp->name) == 0)				
						result = numToBitArray12(temp->value);
					temp = temp->next;
				}
			} else if (strcmp(addressing2Type, "pars") == 0){
				result = codeRegisters(ins, lb, 0);
			} else if (strcmp(addressing2Type, "par1") == 0){
				if (ins->secondPar == 0){
					result = numToBitArray12(atoi(ins->secondParVal));
				} else if (ins->secondPar == 1){
					while (temp != NULL){
						if (strcmp(ins->secondParVal, temp->name) == 0)				
							result = numToBitArray12(temp->value);
						temp = temp->next;
					}
				} else {
					char num = ins->secondParVal[1];
					result = numToBitArray12(num - 48);
				}

			} else {
				if (ins->firstPar == 0){
					result = numToBitArray12(atoi(ins->firstParVal));
				} else if (ins->firstPar == 1){
					while (temp != NULL){
						if (ins->firstParVal != NULL)
							if (strcmp(ins->firstParVal, temp->name) == 0)				
								result = numToBitArray12(temp->value);
							temp = temp->next;
					}
				} else {
					char num = ins->firstParVal[1];
					result = numToBitArray12(num - 48);
				}
			}
		} else {
			if (strcmp(addressing2Type, "val") == 0){
				while (temp != NULL){
					if (strcmp(ins->adressingSourceVal, temp->name) == 0)				
						result = numToBitArray12(temp->value);
					temp = temp->next;
				}
			} else if (strcmp(addressing2Type, "pars") == 0){
				result = codeRegisters(ins, lb, 0);
			} else if (strcmp(addressing2Type, "par1") == 0){
				if (ins->secondPar == 0){
					result = numToBitArray12(atoi(ins->secondParVal));
				} else if (ins->secondPar == 1){
					while (temp != NULL){
						if (strcmp(ins->secondParVal, temp->name) == 0)				
							result = numToBitArray12(temp->value);
						temp = temp->next;
					}
				} else {
					char num = ins->secondParVal[1];
					result = numToBitArray12(num - 48);
				}

			} else {
				if (ins->firstPar == 0){
					result = numToBitArray12(atoi(ins->firstParVal));
				} else if (ins->firstPar == 1){
					while (temp != NULL){
						if (ins->firstParVal != NULL)
							if (strcmp(ins->firstParVal, temp->name) == 0)				
								result = numToBitArray12(temp->value);
						temp = temp->next;
					}
				} else {
					char num = ins->firstParVal[1];
					result = numToBitArray12(num - 48);
				}
			}
		}
	}
	/*REGISTER*/
	 else {

		if (strcmp(direction, "dest") == 0){
			if (strcmp(addressing2Type, "pars") == 0){
				result = codeRegisters(ins, lb, 1);
			} else {
				char num = ins->adressingDestVal[1];
				result = numToBitArray12(num - 48);
			}
		} else {
			if (strcmp(addressing2Type, "pars") == 0){
				result = codeRegisters(ins, lb, 1);
			} else {
				char num = ins->adressingSourceVal[1];
				result = numToBitArray12(num - 48);
			}
		}
	}

	/*ARE*/
	if (ins->ARE == 0){
		result[13-0] = 0;
		result[13-1] = 0;
	} else if (ins->ARE == 1){
		result[13-0] = 1;
		result[13-1] = 0;		
	} else {
		result[13-0] = 0;
		result[13-1] = 1;	
	}

	return result;
}

/*returns the value of a word containing 2 registers*/
int* codeRegisters(Instruction* ins, labelsTable* lb, int regFlag){
	int* result = (int*)malloc_safely(sizeof(int)*14);
	int* first = (int*)malloc_safely(sizeof(int)*6);
	int* second = (int*)malloc_safely(sizeof(int)*6);
	char num;
	result = numToBitArray(0);

	if(regFlag == 0){
		num = ins->firstParVal[1];
		first = numToBitArray6(num - 48);
		num = ins->secondParVal[1];
		second = numToBitArray6(num - 48);
	} else {
		num = ins->adressingSourceVal[1];
		first = numToBitArray6(num - 48);
		num = ins->adressingDestVal[1];
		second = numToBitArray6(num - 48);
	}

	result[0] = first[0];
	result[1] = first[1];
	result[2] = first[2];
	result[3] = first[3];
	result[4] = first[4];
	result[5] = first[5];
	result[6] = second[0];
	result[7] = second[1];
	result[8] = second[2];
	result[9] = second[3];
	result[10] = second[4];
	result[11] = second[5];
	
	return result;
}




