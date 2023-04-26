#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pre_asm.h"
#include "global_vars.h"
#include "dataStructs.h"
#include "utils.h"

/*Gets a file from command line argument, reading it*/
int beginPreAsm(char *as_file){

	char* fileFullName;
	fileFullName = createFIleName(as_file, ".as");

	FILE *fb; /*Current file*/
	fb = fopen(fileFullName, "r");
	/*Check for file failure*/
	if (fb == NULL){
		printf("ERROR: file %s could not be opened.\n", as_file);
		return 0;
	}

	/*First read through, checks for line length errors*/
	if (firstCheck(fb, fileFullName) == 0)
		return 0;

	fseek(fb, 0, SEEK_SET);
	createAm(fb, as_file);/* creates the .am file*/
	return 1;
}


