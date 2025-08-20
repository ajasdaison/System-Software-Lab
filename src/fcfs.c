#include <stdio.h>

#define MAX_PROCESSES 20

int main() {
  int bt[MAX_PROCESSES];
  int wt[MAX_PROCESSES];
  int tat[MAX_PROCESSES];
  int i;
  int n;
  float wtavg;
  float tatavg;

  printf("\nEnter the number of processes -- ");
  scanf("%d", &n);

  for (i = 0; i < n; i++) {
    printf("\nEnter Burst Time for Process %d -- ", i);
    scanf("%d", &bt[i]);
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
  for (i = 0; i < n; i++)
    printf("\tP%d\t\t%d\t\t%d\t\t%d\n", i, bt[i], wt[i], tat[i]);

  printf("\nAverage Waiting Time -- %f", wtavg / n);
  printf("\nAverage Turnaround Time -- %f\n", tatavg / n);

  return 0;
}
