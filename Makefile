main_assembler: main_assembler.o second_pass_logic.o second_pass_utils.o first_pass_logic.o first_pass_utils.o pre_asm_logic.o pre_asm_utils.o dataStructs.o utils.o
	gcc -g -Wall -ansi -pedantic main_assembler.o second_pass_logic.o second_pass_utils.o first_pass_logic.o first_pass_utils.o pre_asm_logic.o pre_asm_utils.o dataStructs.o utils.o -o main_assembler

main_assembler.o: main_assembler.c utils.h pre_asm.h passes.h
	gcc -c main_assembler.c -o main_assembler.o

second_pass_logic.o: second_pass_logic.c global_vars.h utils.h passes.h
	gcc -c second_pass_logic.c -o second_pass_logic.o

second_pass_utils.o: second_pass_utils.c global_vars.h utils.h passes.h
	gcc -c second_pass_utils.c -o second_pass_utils.o

first_pass_logic.o: first_pass_logic.c global_vars.h utils.h passes.h
	gcc -c first_pass_logic.c -o first_pass_logic.o

first_pass_utils.o: first_pass_utils.c global_vars.h utils.h passes.h operand_tables.h
	gcc -c first_pass_utils.c -o first_pass_utils.o

pre_asm_logic.o: pre_asm_logic.c global_vars.h dataStructs.h utils.h pre_asm.h
	gcc -c pre_asm_logic.c -o pre_asm_logic.o

pre_asm_utils.o: pre_asm_utils.c global_vars.h dataStructs.h utils.h pre_asm.h
	gcc -c pre_asm_utils.c -o pre_asm_utils.o

dataStructs.o: dataStructs.c dataStructs.h utils.h global_vars.h
	gcc -c dataStructs.c -o dataStructs.o

utils.o: utils.c utils.h
	gcc -c utils.c -o utils.o
