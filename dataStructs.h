/*Labels table data structure functions.
name = the label name
value = the mempry value
type d = data, c = code, e = exrtern, n=entry*/
typedef struct labelsTable{
	char* name;
	int value;
	char type;
	struct labelsTable* next;
} labelsTable;

/*insert new value to labels table*/
void insertEndLabelsTable(labelsTable**, char*, int, char);
/*returns a new labels table, a copy from the original*/
labelsTable* copyLB(labelsTable*);
/*prints the labels table to the command line*/
void printLabelsTable(labelsTable*);
/*update the value of all lables of a specific type*/
void updateLabelsValues(labelsTable**, int, char);
/*checks if a label (string) exists in a labels table*/
int existsInLabels(labelsTable*, char*);
/*changes a specific label in table to type entry*/
void changeLabelToEntry(labelsTable**, char*);
/*returns a character - the type of the label we got on the variable*/
char getLabelType(labelsTable**, char*);

/*Data
value = the value of the data - character or number
type n = number, c = character
lineNum = the line number in the file this data is
*/
typedef struct Data{
	char* value;
	char type;
	int lineNum;
	struct Data* next;
} Data;

/*insert new value to data*/
void insertToData(Data**, char*, char, int);
/*returns a new data, a copy from the original*/
Data* copyData(Data*);
/*prints the data to the command line*/
void printData(Data*);

/*Machine instruction
ARE, adressingDest, adressingSource, opcode, secondPar, firstPar = values of instructions
adressingDestVal = the value of the first operand
adressingSourceVal = the value og the second operand
secondParVal = the value of addressing 2 parameter
firstParVal = the value of addressing 2 parameter
lineNum = the line number in the file this data is
*/
typedef struct Instruction{
	int ARE; /*bits 0-1*/
	int adressingDest; /*bits 2-3*/
	int adressingSource; /*bits 4-5*/
	char* opcode; /*bits 6-9*/
	int secondPar; /*bits 10-11*/
	int firstPar; /*bits 12-13*/

	char* adressingDestVal;
	char* adressingSourceVal;
	char* secondParVal;
	char* firstParVal;

	int lineNum;

	struct Instruction* next;
} Instruction;

/*insert new instruction*/
void saveToInstruction(Instruction**, int, int, char*, int, int, char*, char*, char*, char*, int);
/*calculates the ARE value for an instruction*/
void calcARE(Instruction**, labelsTable**, int);
/*insert an instruction to memory*/
int* insertInstruction(Instruction*);
/*inserts the extra words to memory*/
int* insertWordToMemory(Instruction*, labelsTable*, int, char*, char*);
/*returns the value of a word containing 2 registers*/
int* codeRegisters(Instruction*, labelsTable*, int);
/*prints the instructions to the command line*/
void printInstructions(Instruction*);

/*Table - for macros
row = macro name
val = instructions in the macro
values = amount of instructions in macro
*/
typedef struct Table{
	char* row;/*table row name*/
	char** val;/*table row values 2D array*/
	int values;/*amount of values in the 2D array*/
	struct Table *next;
} Table;


/*insert new macro*/
void insertEndTable(Table**, char*);
/*insert new line to a macro*/
void insertValuesTable(Table**, char*, char*);
/*returns true if a macro is in the table*/
int searchTable(Table*, char*);
/*returns the number of lines in a macro*/
int getTableValues(Table*, char*);
/*returns the array of lines in the macro*/
char** getTableVals(Table*, char*);
/*prints the macro table*/
void printTable (Table*);
/*returns a specific macro from macros*/
Table* getTableNode(Table*, char*);


