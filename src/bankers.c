#include <stdio.h>

int main() {
  int n, m;
  printf("Enter number of processes: ");
  scanf("%d", &n);
  printf("Enter number of resources: ");
  scanf("%d", &m);

  int alloc[20][20], max[20][20], avail[20];
  int need[20][20], finish[20] = {0}, safeSeq[20];

  printf("Enter Allocation Matrix:\n");
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      scanf("%d", &alloc[i][j]);

  printf("Enter Max Matrix:\n");
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      scanf("%d", &max[i][j]);

  printf("Enter Available Resources:\n");
  for (int i = 0; i < m; i++)
    scanf("%d", &avail[i]);

  printf("\nNeed Matrix:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      need[i][j] = max[i][j] - alloc[i][j];
      printf("%d ", need[i][j]);
    }
    printf("\n");
  }

  int work[20];
  for (int i = 0; i < m; i++)
    work[i] = avail[i];

  int count = 0;  

  while (count < n) {
    int found = 0;  

    for (int i = 0; i < n; i++) {
      if (!finish[i]) { 
        int can_run = 1;
        for (int j = 0; j < m; j++) {
          if (need[i][j] > work[j]) {
            can_run = 0;
            break;
          }
        }

        if (can_run) {
          for (int k = 0; k < m; k++)
            work[k] += alloc[i][k];

          safeSeq[count++] = i;
          finish[i] = 1;
          found = 1;
        }
      }
    }

    if (!found) {
      printf("\nSystem is NOT in a safe state.\n");
      return 0;
    }
  }

  printf("\nSystem is in a SAFE state.\nSafe Sequence: ");
  for (int i = 0; i < n; i++)
    printf("P%d ", safeSeq[i]);
  printf("\n");

  return 0;
}

