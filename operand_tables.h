/*Registers names array*/
char *registersNames[] = {"r0","r1","r2","r3","r4","r5","r6","r7","PSW"};

/*Operands names array*/
char *operandsNames[] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop","\0"};

/*First instruction group*/
char *operandsFirstGroup[] = {"mov","cmp","add","sub","lea","\0"};

/*Second instruction group*/
char *operandsSecondGroup[] = {"not","clr","inc","dec","jmp","bne","red","prn","jsr","\0"};

/*Third instruction group*/
char *operandsThirdGroup[] = {"rts","stop","\0"};

/*Legal adressing source operand*/
const char* adressingSourceOp[16][5] = {
/*op,   0,  1,  2,  3*/
{"mov","V","V","V","V"},
{"cmp","V","V","V","V"},
{"add","V","V","V","V"},
{"sub","V","V","V","V"},
{"not","X","X","X","X"},
{"clr","X","X","X","X"},
{"lea","X","V","V","X"},
{"inc","X","X","X","X"},
{"dec","X","X","X","X"},
{"jmp","X","X","X","X"},
{"bne","X","X","X","X"},
{"red","X","X","X","X"},
{"prn","X","X","X","X"},
{"jsr","X","X","X","X"},
{"rts","X","X","X","X"},
{"stop","X","X","X","X"}};

/*Legal adressing destination operand*/
const char* adressingDestOp[16][5] = {
/*op,   0,  1,  2,  3*/
{"mov","X","V","V","V"},
{"cmp","V","V","V","V"},
{"add","X","V","V","V"},
{"sub","X","V","V","V"},
{"not","X","V","V","V"},
{"clr","X","V","V","V"},
{"lea","X","V","V","V"},
{"inc","X","V","V","V"},
{"dec","X","V","V","V"},
{"jmp","X","V","V","V"},
{"bne","X","V","V","V"},
{"red","X","V","V","V"},
{"prn","V","V","V","V"},
{"jsr","X","V","V","V"},
{"rts","X","X","X","X"},
{"stop","X","X","X","X"}};



