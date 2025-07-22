//Round Robin Scheduling Algorithm in C++

#include <stdio.h>

#define MAX 50

int queue[MAX], front = -1, rear = -1;

// Selection Sort based on Arrival Time while maintaining original indices
void select(int a[], int b[], int c[], int index[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int min = i;
        for (int j = i + 1; j < size; j++) {
            if (a[min] > a[j]) {
                min = j;
            }
        }
        // Swap arrival time
        int temp = a[min];
        a[min] = a[i];
        a[i] = temp;

        // Swap burst time
        temp = b[min];
        b[min] = b[i];
        b[i] = temp;

        // Swap process ID
        temp = c[min];
        c[min] = c[i];
        c[i] = temp;

        // Swap original index
        temp = index[min];
        index[min] = index[i];
        index[i] = temp;
    }
}

// Enqueue a process
void enque(int data) {
    if (rear == MAX - 1)
        printf("Queue Overflow\n");
    else {
        if (front == -1)
            front = 0;
        queue[++rear] = data;
    }
}

// Dequeue a process
int deque() {
    if (front == -1 || front > rear)
        return -1;
    return queue[front++];
}

int main() {
    int i, n, timeq;
    float twt = 0, ttat = 0;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    int at[n], bt[n], rbt[n], wt[n], tat[n], ct[n], pid[n], index[n];
    int final_ct[n], final_wt[n], final_tat[n], final_at[n], final_bt[n];  // Store results in original order

    printf("Enter process IDs: ");
    for (i = 0; i < n; i++) {
        scanf("%d", &pid[i]);
        index[i] = i; // Store original index
    }

    printf("Enter arrival times: ");
    for (i = 0; i < n; i++) {
        scanf("%d", &at[i]);
        final_at[i] = at[i]; // Store original arrival times
    }

    printf("Enter burst times: ");
    for (i = 0; i < n; i++) {
        scanf("%d", &bt[i]);
        final_bt[i] = bt[i]; // Store original burst times
    }

    printf("Enter time quantum: ");
    scanf("%d", &timeq);

    // Sort processes by arrival time while tracking original indices
    select(at, bt, pid, index, n);

    // Copy burst times for remaining burst time
    for (i = 0; i < n; i++)
        rbt[i] = bt[i];

    int current_time = 0, completed = 0, mark[n];
    
    for (i = 0; i < n; i++)
        mark[i] = 0;

    // Enqueue first process
    enque(0);
    mark[0] = 1;

    while (completed != n) {
        int bti = deque(); // Get process index

        if (bti == -1) {
            current_time++;
            continue;
        }

        // Execute process
        if (rbt[bti] > timeq) {
            current_time += timeq;
            rbt[bti] -= timeq;
        } else {
            current_time += rbt[bti];
            rbt[bti] = 0;
            completed++;
            ct[bti] = current_time;
            tat[bti] = ct[bti] - at[bti];
            wt[bti] = tat[bti] - bt[bti];

            ttat += tat[bti];
            twt += wt[bti];
        }

        // Check for new arrivals and enqueue them
        for (i = 0; i < n; i++) {
            if (rbt[i] > 0 && at[i] <= current_time && mark[i] == 0) {
                enque(i);
                mark[i] = 1;
            }
        }

        // If process is not completed, enqueue it again
        if (rbt[bti] > 0)
            enque(bti);
    }

    // Map values back to original order
    for (i = 0; i < n; i++) {
        final_ct[index[i]] = ct[i];
        final_tat[index[i]] = tat[i];
        final_wt[index[i]] = wt[i];
    }

    // Display Results in Original Order
    printf("\nP\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", i+1, final_at[i], final_bt[i], final_ct[i], final_tat[i], final_wt[i]);
    }

    printf("\nAverage Waiting Time: %.2f", twt / n);
    printf("\nAverage Turnaround Time: %.2f\n", ttat / n);

    return 0;
}