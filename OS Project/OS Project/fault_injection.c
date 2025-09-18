/*
 * fault_injection.c - Implementation for fault injection module
 */

#include <stdio.h>
#include "fault_injection.h"
#include "process_manager.h"
#include <stdlib.h>

// Dummy implementation of deadlock fault injection
void inject_deadlock() {
    printf("\n[Fault Injection] Simulating deadlock...\n");
    if (process_count < 2) {
        printf("Not enough processes to simulate deadlock.\n");
        return;
    }
    int idx1 = rand() % process_count;
    int idx2 = rand() % process_count;
    while(idx2 == idx1) {
        idx2 = rand() % process_count;
    }
    pcb_table[idx1].state = 2; // waiting
    pcb_table[idx2].state = 2; // waiting
    printf("Processes %d and %d marked as waiting to simulate deadlock.\n", pcb_table[idx1].pid, pcb_table[idx2].pid);
}

// Dummy implementation of CPU overload fault injection
void inject_cpu_overload() {
    printf("\n[Fault Injection] Simulating CPU overload...\n");
    if (process_count < 1) {
        printf("No processes available.\n");
        return;
    }
    int idx = rand() % process_count;
    pcb_table[idx].cpu_usage = 100.0;
    printf("Process %d CPU usage set to 100%% to simulate overload.\n", pcb_table[idx].pid);
}

// Dummy implementation of thrashing fault injection
void inject_thrashing() {
    printf("\n[Fault Injection] Simulating thrashing...\n");
    if (process_count < 1) {
        printf("No processes available.\n");
        return;
    }
    int idx = rand() % process_count;
    // Reduce allocated memory drastically to simulate constant swapping
    int original_memory = pcb_table[idx].allocated_memory;
    pcb_table[idx].allocated_memory = original_memory / 2;
    printf("Process %d allocated memory reduced from %d to %d to simulate thrashing.\n", pcb_table[idx].pid, original_memory, pcb_table[idx].allocated_memory);
}
