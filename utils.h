/*this function return a string that is a file name*/
char* createFIleName(char*, char*);

/*this function mallocs data safely*/
void* malloc_safely(long);

/*this function reallocs data safely*/
void* realloc_safely (void*, long);

/*this function returns a substring from a position in a string, without spaces*/
char* getNextWord(char*, int, int*);

/*this function rmoves tabs and spaces from a string and return a new string*/
char* removeTabsSpaces(char*);

/*returns how many words are in an array*/
int wordInArray(char* [], char*);

/*returns the length of the longest word in an array*/
int longestWordInArray (char* []);

/*returns the position of a string in an array*/
int wordPosArray (char*[], char*);

/*returns true or false if a string represens a legal number - +/- and then sigits*/
int legalNumber (char*);

/*returns the position of a character in a string*/
int howManyChars(char*, char);

/*reurns the next word - substring, stops when hot space, tab, end of line or comma*/
char* getNextWordComma(char*, int, int*);

/*returns integer the string represents*/
int stringToNum(char*);

/*returns an array that represents a number in binary with 14 bits*/
int* numToBitArray(int);

/*returns an array that represents a number in binary with 12 bits*/
int* numToBitArray12(int);

/*returns an array that represents a number in binary with 6 bits*/
int* numToBitArray6(int);



