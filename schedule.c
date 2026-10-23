#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int is_completed;
} Process;
void sortByArrival(Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}
void fcfs(Process p[], int n) {
    printf("=== First Come First Served (FCFS) ===\n");
    sortByArrival(p, n);
    int current_time = 0;
    float total_wt = 0, total_tat = 0, total_rt = 0;
    printf("Gantt Chart: ");
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time; // CPU Idle
        }
        p[i].response_time = current_time - p[i].arrival_time;
        p[i].waiting_time = p[i].response_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        current_time += p[i].burst_time;
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        total_rt += p[i].response_time;
        printf("|   P%d   ", p[i].pid);
    }
    printf("|\n");
    printf("%-7s %-6s %-6s %-6s %-6s %-6s\n", "PID", "AT", "BT", "WT", "TAT", "RT");
    for (int i = 0; i < n; i++) {
        printf("%-7d %-6d %-6d %-6d %-6d %-6d\n", 
               p[i].pid, p[i].arrival_time, p[i].burst_time, 
               p[i].waiting_time, p[i].turnaround_time, p[i].response_time);
    }
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Response Time: %.2f\n\n", total_rt / n);
}
void sjf(Process p[], int n) {
    printf("=== Shortest Job First (SJF) ===\n");
    int current_time = 0, completed = 0;
    float total_wt = 0, total_tat = 0, total_rt = 0;
    for (int i = 0; i < n; i++) {
        p[i].is_completed = 0;
    }
    printf("Gantt Chart: ");
    Process exec_order[100];
    int exec_index = 0;
    while (completed != n) {
        int idx = -1;
        int min_burst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].is_completed == 0) {
                if (p[i].burst_time < min_burst) {
                    min_burst = p[i].burst_time;
                    idx = i;
                } else if (p[i].burst_time == min_burst) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }
        if (idx != -1) {
            p[idx].response_time = current_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].response_time;
            p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
            p[idx].is_completed = 1;
            total_wt += p[idx].waiting_time;
            total_tat += p[idx].turnaround_time;
            total_rt += p[idx].response_time;
            current_time += p[idx].burst_time;
            completed++;
            exec_order[exec_index++] = p[idx];
            printf("|   P%d   ", p[idx].pid);
        } else {
            current_time++;
        }
    }
    printf("|\n");
    printf("%-7s %-6s %-6s %-6s %-6s %-6s\n", "PID", "AT", "BT", "WT", "TAT", "RT");
    for (int i = 0; i < n; i++) {
        printf("%-7d %-6d %-6d %-6d %-6d %-6d\n", 
               exec_order[i].pid, exec_order[i].arrival_time, exec_order[i].burst_time, 
               exec_order[i].waiting_time, exec_order[i].turnaround_time, exec_order[i].response_time);
    }
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Response Time: %.2f\n", total_rt / n);
}
int main() {
    int n;
    printf("Enter the number of processes: ");
    if(scanf("%d", &n) != 1) return 1;
    Process *p_fcfs = (Process *)malloc(n * sizeof(Process));
    Process *p_sjf = (Process *)malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        p_fcfs[i].pid = i + 1;
        p_sjf[i].pid = i + 1;
        
        printf("Enter the arrival time and burst time for process %d: ", i + 1);
        scanf("%d %d", &p_fcfs[i].arrival_time, &p_fcfs[i].burst_time);
        
        p_sjf[i].arrival_time = p_fcfs[i].arrival_time;
        p_sjf[i].burst_time = p_fcfs[i].burst_time;
    }
    printf("\n");
    fcfs(p_fcfs, n);
    sjf(p_sjf, n);
    free(p_fcfs);
    free(p_sjf);
    return 0;
}