#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head, i, j, direction;
    int request[50], seek = 0;
    int temp, size = 200;  // assume disk size is 0–199
    float avg;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    printf("Enter request sequence: ");
    for (i = 0; i < n; i++)
        scanf("%d", &request[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter head movement direction (1 for high →, 0 for low ←): ");
    scanf("%d", &direction);

    // Add head and disk end positions
    request[n] = head;
    n++;

    // Sort the request array
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (request[j] > request[j + 1]) {
                temp = request[j];
                request[j] = request[j + 1];
                request[j + 1] = temp;
            }
        }
    }

    // Find position of head
    int pos;
    for (i = 0; i < n; i++) {
        if (request[i] == head) {
            pos = i;
            break;
        }
    }

    printf("\nSeek Sequence:\n");

    // SCAN logic
    if (direction == 1) { // move right first
        for (i = pos; i < n; i++) {
            printf("%d → ", request[i]);
            if (i != n - 1)
                seek += abs(request[i + 1] - request[i]);
        }
        // go to end of disk
        printf("199 → ");
        seek += abs(199 - request[n - 1]);

        // now move left
        for (i = pos - 1; i >= 0; i--) {
            printf("%d → ", request[i]);
            if (i != 0)
                seek += abs(request[i] - request[i - 1]);
        }
    } else { // move left first
        for (i = pos; i >= 0; i--) {
            printf("%d → ", request[i]);
            if (i != 0)
                seek += abs(request[i] - request[i - 1]);
        }
        // go to start of disk
        printf("0 → ");
        seek += abs(request[0] - 0);

        // now move right
        for (i = pos + 1; i < n; i++) {
            printf("%d → ", request[i]);
            if (i != n - 1)
                seek += abs(request[i + 1] - request[i]);
        }
    }

    printf("\nTotal Seek Time = %d", seek);
    avg = (float)seek / (n - 1);
    printf("\nAverage Seek Time = %.2f\n", avg);

    return 0;
}
