#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

char* searchOptab(char opcode[]) {
    static char code[10];
    FILE *optab;
    char mnemonic[10], value[10];
    optab = fopen("optab.txt", "r");
    if (optab == NULL) {
        printf("Error: Cannot open optab.txt\n");
        exit(1);
    }
    while (fscanf(optab, "%s %s", mnemonic, value) != EOF) {
        if (strcmp(opcode, mnemonic) == 0) {
            strcpy(code, value);
            fclose(optab);
            return code;
        }
    }
    fclose(optab);
    return NULL;
}

char* searchSymtab(char symbol[]) {
    static char addr[10];
    FILE *symtab;
    char sym[10], value[10];
    symtab = fopen("symtab.txt", "r");
    if (symtab == NULL) {
        printf("Error: Cannot open symtab.txt\n");
        exit(1);
    }
    while (fscanf(symtab, "%s %s", sym, value) != EOF) {
        if (strcmp(sym, symbol) == 0) {
            strcpy(addr, value);
            fclose(symtab);
            return addr;
        }
    }
    fclose(symtab);
    return NULL;
}

int main() {
    FILE *intermediate, *listing;
    char label[20], opcode[20], operand[20];
    char objCode[20];
    int loc;

    intermediate = fopen("intermediate.txt", "r");
    listing = fopen("listing.txt", "w");

    if (intermediate == NULL || listing == NULL) {
        printf("Error: Cannot open necessary files.\n");
        exit(1);
    }

    if (fscanf(intermediate, "%x %s %s %s", &loc, label, opcode, operand) != 4) {
        printf("Error reading first line.\n");
        return 1;
    }

    if (strcmp(opcode, "START") == 0) {
        fprintf(listing, "%04X\t%s\t%s\t%s\n", loc, label, opcode, operand);
        if (fscanf(intermediate, "%x %s %s %s", &loc, label, opcode, operand) != 4)
            return 1;
    }

    while (strcmp(opcode, "END") != 0) {
        if (opcode[0] != '.') {
            char *code = searchOptab(opcode);
            char *addr;

            if (code != NULL) {
                if (strcmp(operand, "**") != 0 && strcmp(operand, "") != 0)
                    addr = searchSymtab(operand);
                else
                    addr = "0000";
                if (addr == NULL)
                    addr = "0000";
                sprintf(objCode, "%s%s", code, addr);
            } else if (strcmp(opcode, "WORD") == 0) {
                int val = atoi(operand);
                sprintf(objCode, "%06X", val);
            } else if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'C') {
                    char temp[20];
                    int i;
                    strcpy(temp, operand);
                    strcpy(objCode, "");
                    for (i = 2; i < strlen(temp) - 1; i++) {
                        char hex[5];
                        sprintf(hex, "%X", temp[i]);
                        strcat(objCode, hex);
                    }
                } else if (operand[0] == 'X') {
                    strncpy(objCode, operand + 2, strlen(operand) - 3);
                    objCode[strlen(operand) - 3] = '\0';
                }
            } else {
                strcpy(objCode, "");
            }

            fprintf(listing, "%04X\t%s\t%s\t%s\t%s\n", loc, label, opcode, operand, objCode);
        }

        if (fscanf(intermediate, "%x %s %s %s", &loc, label, opcode, operand) != 4)
            break;
    }

    fprintf(listing, "    \t%s\t%s\t%s\n", label, opcode, operand);
    fclose(intermediate);
    fclose(listing);

    printf("✅ Listing file generated successfully!\n");
    return 0;
}

