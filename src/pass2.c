#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

char *searchOptab(char opcode[]) {
  static char code[10];
  FILE *optab;
  char mnemonic[10], value[10];
  int found = 0;

  optab = fopen("optab.txt", "r");
  if (optab == NULL) {
    printf("Error: Cannot open optab.txt\n");
    exit(1);
  }

  while (fscanf(optab, "%s %s", mnemonic, value) != EOF) {
    if (strcmp(opcode, mnemonic) == 0) {
      strcpy(code, value);
      found = 1;
      break;
    }
  }

  fclose(optab);
  if (found)
    return code;
  else
    return NULL;
}

char *searchSymtab(char symbol[]) {
  static char addr[10];
  FILE *symtab;
  char sym[10], value[10];
  int found = 0;

  symtab = fopen("symtab.txt", "r");
  if (symtab == NULL) {
    printf("Error: Cannot open symtab.txt\n");
    exit(1);
  }

  while (fscanf(symtab, "%s %s", sym, value) != EOF) {
    if (strcmp(sym, symbol) == 0) {
      strcpy(addr, value);
      found = 1;
      break;
    }
  }

  fclose(symtab);
  if (found)
    return addr;
  else
    return NULL;
}

int main() {
  FILE *intermediate, *objectcode, *listing;
  char label[20], opcode[20], operand[20];
  char objCode[80];
  char startAddr[10], textRecord[200] = "";
  int loc = 0, start = 0, textStart = 0, textLen = 0;
  int ret;

  intermediate = fopen("intermediate.txt", "r");
  objectcode = fopen("objectcode.txt", "w");
  listing = fopen("listing.txt", "w");

  if (intermediate == NULL || objectcode == NULL || listing == NULL) {
    printf("Error: Cannot open necessary files.\n");
    exit(1);
  }

  /* Read first record */
  ret = fscanf(intermediate, "%x %19s %19s %19s", &loc, label, opcode, operand);
  if (ret != 4) {
    printf("Error: intermediate.txt is empty or malformed.\n");
    fclose(intermediate);
    fclose(objectcode);
    fclose(listing);
    return 1;
  }

  if (strcmp(opcode, "START") == 0) {
    strncpy(startAddr, operand, sizeof(startAddr) - 1);
    startAddr[sizeof(startAddr) - 1] = '\0';
    start = (int)strtol(startAddr, NULL, 16);
    fprintf(objectcode, "H^%-6s^%06X^", label, start);
    /* read next record */
    ret =
        fscanf(intermediate, "%x %19s %19s %19s", &loc, label, opcode, operand);
  } else {
    start = loc; /* if no START, base start on first loc read */
  }

  textStart = loc;
  /* Placeholder for text record length — we'll write correct records as we go
   */
  fprintf(objectcode, "%06X\n",
          0); /* you can remove or change as per your format */
  fprintf(objectcode, "T^%06X^", textStart);

  /* Main loop: continue while we successfully read 4 items and opcode != "END"
   */
  while (ret == 4 && strcmp(opcode, "END") != 0) {
    if (opcode[0] != '.') {
      char *code = searchOptab(opcode);
      char *addr = NULL;
      objCode[0] = '\0'; /* clear objCode each iteration */

      if (code != NULL) {
        if (strcmp(operand, "**") != 0 && strlen(operand) > 0)
          addr = searchSymtab(operand);
        else
          addr = "0000";

        if (addr == NULL)
          addr = "0000";

        snprintf(objCode, sizeof(objCode), "%s%s", code, addr);
      } else if (strcmp(opcode, "WORD") == 0) {
        int val = atoi(operand);
        snprintf(objCode, sizeof(objCode), "%06X", val);
      } else if (strcmp(opcode, "BYTE") == 0) {
        if (operand[0] == 'C') {
          /* C'EOF' -> ASCII hex of characters */
          int i;
          objCode[0] = '\0';
          for (i = 2; i < (int)strlen(operand) - 1 &&
                      (int)strlen(objCode) < (int)sizeof(objCode) - 6;
               i++) {
            char hex[5];
            sprintf(hex, "%02X", (unsigned char)operand[i]);
            strcat(objCode, hex);
          }
        } else if (operand[0] == 'X') {
          size_t len = strlen(operand) - 3; /* skip X' and trailing ' */
          if (len >= sizeof(objCode))
            len = sizeof(objCode) - 1;
          strncpy(objCode, operand + 2, len);
          objCode[len] = '\0';
        }
      } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
        /* flush current text record if any */
        if (textLen > 0) {
          fprintf(objectcode, "^%02X%s\n", textLen, textRecord);
          textRecord[0] = '\0';
          textLen = 0;
        }
        /* read next record and continue (check return) */
        ret = fscanf(intermediate, "%x %19s %19s %19s", &loc, label, opcode,
                     operand);
        if (ret != 4)
          break;
        continue;
      } else {
        objCode[0] = '\0';
      }

      /* if adding this objCode exceeds record length, flush and start new T
       * record */
      if ((int)strlen(textRecord) + (int)strlen(objCode) + 2 > 60) {
        if (textLen > 0)
          fprintf(objectcode, "^%02X%s\n", textLen, textRecord);
        textRecord[0] = '\0';
        textLen = 0;
        fprintf(objectcode, "T^%06X^", loc);
      }

      if (strlen(objCode) > 0) {
        strcat(textRecord, "^");
        strcat(textRecord, objCode);
        textLen += (int)strlen(objCode) / 2;
      }

      fprintf(listing, "%04X\t%s\t%s\t%s\t%s\n", loc, label, opcode, operand,
              objCode);
    }

    /* read next record for next iteration and check return value */
    ret =
        fscanf(intermediate, "%x %19s %19s %19s", &loc, label, opcode, operand);
  }

  /* after loop: flush remaining textRecord if any */
  if (textLen > 0)
    fprintf(objectcode, "^%02X%s\n", textLen, textRecord);

  /* if we ended on an END record that was read successfully, write E record
   * using start */
  fprintf(objectcode, "E^%06X\n", start);
  if (ret == 4 && strcmp(opcode, "END") == 0) {
    fprintf(listing, "    \t%s\t%s\t%s\n", label, opcode, operand);
  }

  fclose(intermediate);
  fclose(objectcode);
  fclose(listing);

  printf("✅ Pass 2 completed successfully!\n");
  return 0;
}
