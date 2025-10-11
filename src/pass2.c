#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char opcode[20], operand[20], symbol[20], label[20], code[20], mnemonic[25],
      character;
  char add[20], objectcode[50]; // increased size to hold full objectcode
  int flag, flag1, locctr, location, loc;

  FILE *fp1, *fp2, *fp3, *fp4;

  // Open all required files
  fp1 = fopen("out1.txt", "r");
  fp2 = fopen("twoout.txt", "w");
  fp3 = fopen("opttab.txt", "r");
  fp4 = fopen("sym1.txt", "r");

  if (!fp1 || !fp2 || !fp3 || !fp4) {
    printf("Error opening file(s).\n");
    exit(1);
  }

  fscanf(fp1, "%s%s%s", label, opcode, operand);

  if (strcmp(opcode, "START") == 0) {
    fprintf(fp2, "%s\t%s\t%s\n", label, opcode, operand);
    fscanf(fp1, "%d%s%s%s", &locctr, label, opcode, operand);
  }

  while (strcmp(opcode, "END") != 0) {
    flag = 0;

    // Rewind opttab to read from start every time
    rewind(fp3);
    while (fscanf(fp3, "%s%s", code, mnemonic) == 2) {
      if ((strcmp(opcode, code) == 0) && (strcmp(mnemonic, "*") != 0)) {
        flag = 1;
        break;
      }
    }

    if (flag == 1) {
      flag1 = 0;
      rewind(fp4);
      while (fscanf(fp4, "%s%d", symbol, &loc) == 2) {
        if (strcmp(symbol, operand) == 0) {
          flag1 = 1;
          break;
        }
      }

      if (flag1 == 1) {
        sprintf(add, "%d", loc);
        strcpy(objectcode, mnemonic);
        strcat(objectcode, add);
      } else {
        strcpy(objectcode, "0000"); // symbol not found, dummy code
      }
    } else if (strcmp(opcode, "BYTE") == 0) {
      if (operand[0] == 'C') {
        // BYTE C'EOF' → convert each char to hex
        int i;
        objectcode[0] = '\0';
        for (i = 2; operand[i] != '\'' && operand[i] != '\0'; i++) {
          char temp[5];
          sprintf(temp, "%X", operand[i]);
          strcat(objectcode, temp);
        }
      } else if (operand[0] == 'X') {
        // BYTE X'F1' → copy hex directly
        strncpy(objectcode, operand + 2, strlen(operand) - 3);
        objectcode[strlen(operand) - 3] = '\0';
      }
    } else if (strcmp(opcode, "WORD") == 0) {
      int val = atoi(operand);
      sprintf(objectcode, "%06X", val); // 3 bytes
    } else {
      strcpy(objectcode, "\0"); // no object code
    }

    // Write output line
    fprintf(fp2, "%s\t%s\t%s\t%d\t%s\n", label, opcode, operand, locctr,
            objectcode);

    // Read next line
    if (fscanf(fp1, "%d%s%s%s", &locctr, label, opcode, operand) != 4) {
      break;
    }
  }

  // Write END record
  fprintf(fp2, "%s\t%s\t%s\t%d\n", label, opcode, operand, locctr);

  // Close all files
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);

  return 0;
}
