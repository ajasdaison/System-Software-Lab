#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50

int searchOptab(char opcode[]) {
  FILE *optab;
  char code[10], mnemonic[10];
  int found = 0;
  optab = fopen("optab.txt", "r");
  while (fscanf(optab, "%s %s", mnemonic, code) != EOF) {
    if (strcmp(opcode, mnemonic) == 0) {
      found = 1;
      break;
    }
  }
  fclose(optab);
  return found;
}

int main() {
  FILE *input, *intermediate, *symtab;
  char label[20], opcode[20], operand[20];
  char line[MAX_LEN];
  int locctr, startAddr = 0, programLength;

  input = fopen("input.txt", "r");
  intermediate = fopen("intermediate.txt", "w");
  symtab = fopen("symtab.txt", "w");

  fscanf(input, "%s %s %s", label, opcode, operand);

  if (strcmp(opcode, "START") == 0) {
    startAddr = strtol(operand, NULL, 16);
    locctr = startAddr;
    fprintf(intermediate, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);
    fscanf(input, "%s %s %s", label, opcode, operand);
  } else {
    locctr = 0;
  }

  while (strcmp(opcode, "END") != 0) {

    if (strcmp(label, "**") != 0) {
      fprintf(symtab, "%s\t%04X\n", label, locctr);
    }
    if (searchOptab(opcode)) {
      locctr += 3;
    } else if (strcmp(opcode, "WORD") == 0) {
      locctr += 3;
    } else if (strcmp(opcode, "RESW") == 0) {
      locctr += 3 * atoi(operand);
    } else if (strcmp(opcode, "RESB") == 0) {
      locctr += atoi(operand);
    } else if (strcmp(opcode, "BYTE") == 0) {
      if (operand[0] == 'C')
        locctr += strlen(operand) - 3;
      else if (operand[0] == 'X')
        locctr += (strlen(operand) - 3) / 2;
    } else {
      printf("Error: Invalid opcode %s\n", opcode);
    }

    fprintf(intermediate, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);
    fscanf(input, "%s %s %s", label, opcode, operand);
  }

  fprintf(intermediate, "    \t%s\t%s\t%s\n", label, opcode, operand);
  programLength = locctr - startAddr;
  printf("\nProgram length = %X\n", programLength);

  fclose(input);
  fclose(intermediate);
  fclose(symtab);

  return 0;
}
