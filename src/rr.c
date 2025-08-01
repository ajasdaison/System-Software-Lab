#include <stdio.h>

int main() {
  int i, n, t, bu[10], wa[10], tat[10], ct[10], remaining[10];
  float awt = 0, att = 0;
  int time = 0, done;

  printf("Enter the number of processes -- ");
  scanf("%d", &n);

  for (i = 0; i < n; i++) {
    printf("Enter Burst Time for process %d -- ", i + 1);
    scanf("%d", &bu[i]);
    remaining[i] = bu[i];
  }

  printf("Enter the size of time slice -- ");
  scanf("%d", &t);

  while (1) {
    done = 1;
    for (i = 0; i < n; i++) {
      if (remaining[i] > 0) {
        done = 0;
        if (remaining[i] <= t) {
          time += remaining[i];
          remaining[i] = 0;
          tat[i] = time;
        } else {
          remaining[i] -= t;
          time += t;
        }
      }
    }
    if (done == 1)
      break;
  }

  for (i = 0; i < n; i++) {
    ct[i] = bu[i];
    wa[i] = tat[i] - bu[i];
    awt += wa[i];
    att += tat[i];
  }

  printf("\n\tPROCESS\t BURST TIME \t WAITING TIME\tTURNAROUND TIME\n");
  for (i = 0; i < n; i++) {
    printf("\t%d \t %d \t\t %d \t\t %d \n", i + 1, ct[i], wa[i], tat[i]);
  }

  printf("\nThe Average Turnaround Time is -- %f", att / n);
  printf("\nThe Average Waiting Time is -- %f\n", awt / n);

  return 0;
}
