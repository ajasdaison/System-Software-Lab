#include <stdio.h>

int main() {
  int p[20], bt[20], wt[20], tat[20], i, k, n, temp;
  float wtavg = 0, tatavg = 0;

  printf("\nEnter the number of processes -- ");
  scanf("%d", &n);

  for (i = 0; i < n; i++) {
    p[i] = i;
    printf("Enter Burst Time for Process %d -- ", i);
    scanf("%d", &bt[i]);
  }

  // Sort based on burst time
  for (i = 0; i < n; i++) {
    for (k = i + 1; k < n; k++) {
      if (bt[i] > bt[k]) {
        // Swap burst times
        temp = bt[i];
        bt[i] = bt[k];
        bt[k] = temp;

        // Swap process IDs
        temp = p[i];
        p[i] = p[k];
        p[k] = temp;
      }
    }
  }

  wt[0] = 0;
  tat[0] = bt[0];
  wtavg = 0;
  tatavg = tat[0];

  for (i = 1; i < n; i++) {
    wt[i] = wt[i - 1] + bt[i - 1];
    tat[i] = wt[i] + bt[i];
    wtavg += wt[i];
    tatavg += tat[i];
  }

  printf("\n\tPROCESS\t\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\n");
  for (i = 0; i < n; i++) {
    printf("\tP%d\t\t%d\t\t%d\t\t%d\n", p[i], bt[i], wt[i], tat[i]);
  }

  printf("\nAverage Waiting Time -- %f", wtavg / n);
  printf("\nAverage Turnaround Time -- %f\n", tatavg / n);

  return 0;
}
