/*
 * process_manager.h - Header for process management module
 */

#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

void init_process_manager();
void init_dummy_processes();
void display_process_table();

// Define the Process Control Block structure
typedef struct {
    int pid;
    char name[50];
    float cpu_usage;    // real CPU usage
    float mem_usage;    // real Memory usage
    int arrival_time;   
    int burst_time;     
    int priority;       
    int allocated_memory;
    int state;          // 0 = ready, 1 = running, 2 = waiting
} PCB;

#define MAX_PROCESSES 10
extern PCB pcb_table[MAX_PROCESSES];
extern int process_count;

#endif // PROCESS_MANAGER_H
