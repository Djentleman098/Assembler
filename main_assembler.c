
#include "utils.h"
#include "pre_asm.h"
#include "passes.h"

/*
Start the program here
for each file, begin the process
inicialize the memory image and memory position variables
*/
int main (int argc, char *argv[]) {
	int i;
	int result = 1;

	int **MEMORY_IMG = NULL;
	MEMORY_IMG = (int***)malloc_safely(sizeof(int**)*256);
	int MEMORY_POS = 0;

	for (i=1;i<argc;i++){
		if(assembler(argv[i], &MEMORY_IMG, &MEMORY_POS) == 0)
			result = 0;
	}

	if (result == 1)
		printf("\nFiles created successfully\n");
	else
		printf("\nFailed in files creation\n");
}

/*
This function calls each step of the assembly and stops if a step fails:
pre assembly
first pass
second pass
variables: as_file = the file we are working on, MEMORY_IMG, MEMORY_POS
*/
int assembler(char *as_file, int ***MEMORY_IMG, int* MEMORY_POS){

	char* fileFullName;
	int i, j;
	int memory_start = (*MEMORY_POS);
	FILE *object, *entries, *externals;
	char *obName, *enName, *exName;
	char int_string[4];
	int counter = 0;
	
	/*Start pre assembly and quits if fails*/
	if (beginPreAsm(as_file) == 0){ 
		printf("ERROR: failes pre assembly of %s\n", fileFullName);
		return 0;
	}

	/*Creates the data structures used for reading and saving the file*/
	labelsTable* lb = NULL;
	Instruction* ins = NULL;
	Data* data = NULL;

	/*Start first pass and quits if fails*/
	fileFullName = createFIleName(as_file, ".am");
	if (beginFirstPass(fileFullName, &lb, &ins, &data) == 0){ 
		printf("ERROR: failes first pass of %s\n", fileFullName);
		return 0;
	}

	/*Start second pass and quits if fails*/
	if (beginSecondPass(fileFullName, &lb, &ins, &data, MEMORY_IMG, MEMORY_POS) == 0){ 
		printf("ERROR: failes second pass of %s\n", fileFullName);
		return 0;
	}

	/*Creates files*/
	obName = createFIleName(as_file, ".ob");
	enName = createFIleName(as_file, ".ent");
	exName = createFIleName(as_file, ".ext");

	/*.ob file*/
	if (*MEMORY_IMG != NULL){
		object = fopen(obName, "w+");
		for (i = memory_start; i< *MEMORY_POS; i++){	
			fputc('0', object);
			sprintf(int_string, "%d", i+100);
			fputs(int_string , object);
			fputc('\t', object);
			for (j = 0; j< 14; j++){
				if (*(*((*MEMORY_IMG) + sizeof(int*)*i) + j) == 0)
					fputc('.', object);
				else
					fputc('/', object);
			}
			fputc('\n', object);
		}
		fclose(object);
	}

	/*.ent file*/
	labelsTable* temp;
	temp = lb;
	/*cheacks if there are any entries*/
	while (temp != NULL){
		if (temp->type == 'n')
			counter++;
		temp = temp->next;
	}
	if (counter > 0){
		entries = fopen(enName, "w+");
		temp = lb;	
		while (temp != NULL){
			if (temp->type == 'n'){
				fputs(temp->name , entries);
				fputc('\t', entries);
				sprintf(int_string, "%d", temp->value);
				fputs(int_string , entries);
				fputc('\n', object);
			}
			temp = temp->next;
		}
		fclose(entries);
	}

	/*.ext file*/
	labelsTable* temp2;
	temp2 = lb;
	counter = 0;
	/*cheacks if there are any externals*/
	while (temp2 != NULL){
		if (temp2->type == 'e')
			counter++;
		temp2 = temp2->next;
	}
	if (counter > 0){
		externals = fopen(exName, "w+");
		temp2 = lb;	
		while (temp != NULL){
			if (temp2->type == 'e'){
				fputs(temp->name , entries);
				fputc('\t', entries);
				sprintf(int_string, "%d", temp2->value);
				fputs(int_string , entries);
				fputc('\n', object);
			}
			temp2 = temp2->next;
		}
		fclose(externals);
	}

	return 1;

}
