#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
  FILE *fp;
  int i, addr1, l, j, staddr1;  //NOLINT
  char name[10], line[50], name1[10], addr[10], rec[10], ch, staddr[10]; //NOLINT
  printf("enter program name:");
  scanf("%s", name);
  fp = fopen("objectcode.txt", "r");
  fscanf(fp, "%s", line);
  for (i = 2, j = 0; i < 8, j < 6; i++, j++) //NOLINT
    name1[j] = line[i];
  name1[j] = '\0';
  printf("name from obj. %s\n", name1);
  if (strcmp(name, name1) == 0) {
    fscanf(fp, "%s", line);
    do {
      if (line[0] == 'T') {
        for (i = 2, j = 0; i < 8, j < 6; i++, j++) //NOLINT
          staddr[j] = line[i];
        staddr[j] = '\0';
        staddr1 = atoi(staddr);
        i = 12; //NOLINT
        while (line[i] != '$') {
          if (line[i] != '^') {
            printf("00%d \t %c%c\n", staddr1, line[i], line[i + 1]);
            staddr1++;
            i = i + 2;
          } else
            i++;
        }
      } else if (line[0] = 'E') //NOLINT
        printf("jump to execution address:%s", &line[2]);
      fscanf(fp, "%s", line);
    } while (!feof(fp));
  }
  fclose(fp);
  return 0;
}
