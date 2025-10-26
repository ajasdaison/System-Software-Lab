#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

char* searchOptab(char opcode[]) {
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
    if (found) return code;
    else return NULL;
}

char* searchSymtab(char symbol[]) {
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
    if (found) return addr;
    else return NULL;
}

int main() {
    FILE *intermediate, *objectcode, *listing;
    char label[20], opcode[20], operand[20];
    char objCode[20], startAddr[10], textRecord[80] = "";
    int loc, start = 0, textStart = 0, textLen = 0;

    intermediate = fopen("intermediate.txt", "r");
    objectcode = fopen("objectcode.txt", "w");
    listing = fopen("listing.txt", "w");

    if (intermediate == NULL || objectcode == NULL || listing == NULL) {
        printf("Error: Cannot open necessary files.\n");
        exit(1);
    }

    fscanf(intermediate, "%x %s %s %s", &loc, label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        strcpy(startAddr, operand);
        start = (int)strtol(startAddr, NULL, 16);
        fprintf(objectcode, "H^%-6s^%06X^", label, start);
        fscanf(intermediate, "%x %s %s %s", &loc, label, opcode, operand);
    }

    textStart = loc;
    fprintf(objectcode, "%06X\n", 0);
    fprintf(objectcode, "T^%06X^", textStart);

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
            }
            else if (strcmp(opcode, "WORD") == 0) {
                int val = atoi(operand);
                sprintf(objCode, "%06X", val);
            }
            else if (strcmp(opcode, "BYTE") == 0) {
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
            }
            else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
                if (textLen > 0) {
                    fprintf(objectcode, "^%02X%s\n", textLen, textRecord);
                    textRecord[0] = '\0';
                    textLen = 0;
                }
                fscanf(intermediate, "%x %s %s %s", &loc, label, opcode, operand);
                continue;
            }
            else {
                strcpy(objCode, "");
            }

            if (strlen(textRecord) + strlen(objCode) + 1 > 60) {
                fprintf(objectcode, "^%02X%s\n", textLen, textRecord);
                strcpy(textRecord, "");
                textLen = 0;
                fprintf(objectcode, "T^%06X^", loc);
            }

            if (strlen(objCode) > 0) {
                strcat(textRecord, "^");
                strcat(textRecord, objCode);
                textLen += strlen(objCode) / 2;
            }

            fprintf(listing, "%04X\t%s\t%s\t%s\t%s\n", loc, label, opcode, operand, objCode);
        }

        fscanf(intermediate, "%x %s %s %s", &loc, label, opcode, operand);
    }

    if (textLen > 0)
        fprintf(objectcode, "^%02X%s\n", textLen, textRecord);

    fprintf(objectcode, "E^%06X\n", start);
    fprintf(listing, "    \t%s\t%s\t%s\n", label, opcode, operand);

    fclose(intermediate);
    fclose(objectcode);
    fclose(listing);

    printf("✅ Pass 2 completed successfully!\n");
    return 0;
}

