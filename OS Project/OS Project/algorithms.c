/*
 * algorithms.c - Implementation of OS algorithms
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algorithms.h"
#include "process_manager.h"

// Global variables for algorithm simulation
static ResourceAllocation resources[20];
static PageFrame page_frames[10];
static int resource_count = 0;
static int frame_count = 5;
static int current_time = 0;

// Round Robin CPU Scheduling
void round_robin_scheduling() {
    printf("\n=== ROUND ROBIN SCHEDULING ===\n");
    int time_quantum = 3;
    int completed = 0;
    int current_process = 0;
    int total_time = 0;
    
    printf("Time Quantum: %d units\n", time_quantum);
    printf("Execution Order: ");
    
    // Create a copy of burst times for simulation
    int remaining_time[MAX_PROCESSES];
    for (int i = 0; i < process_count; i++) {
        remaining_time[i] = pcb_table[i].burst_time;
    }
    
    while (completed < process_count) {
        if (remaining_time[current_process] > 0) {
            printf("P%d ", pcb_table[current_process].pid % 100);
            
            if (remaining_time[current_process] > time_quantum) {
                total_time += time_quantum;
                remaining_time[current_process] -= time_quantum;
            } else {
                total_time += remaining_time[current_process];
                remaining_time[current_process] = 0;
                completed++;
            }
        }
        current_process = (current_process + 1) % process_count;
    }
    
    printf("\nTotal execution time: %d units\n", total_time);
}

// Priority Scheduling
void priority_scheduling() {
    printf("\n=== PRIORITY SCHEDULING ===\n");
    
    // Create array of indices sorted by priority
    int process_order[MAX_PROCESSES];
    for (int i = 0; i < process_count; i++) {
        process_order[i] = i;
    }
    
    // Sort by priority (lower number = higher priority)
    for (int i = 0; i < process_count - 1; i++) {
        for (int j = i + 1; j < process_count; j++) {
            if (pcb_table[process_order[i]].priority > pcb_table[process_order[j]].priority) {
                int temp = process_order[i];
                process_order[i] = process_order[j];
                process_order[j] = temp;
            }
        }
    }
    
    printf("Execution Order (by priority): ");
    int total_time = 0;
    for (int i = 0; i < process_count; i++) {
        int idx = process_order[i];
        printf("P%d(pri:%d) ", pcb_table[idx].pid % 100, pcb_table[idx].priority);
        total_time += pcb_table[idx].burst_time;
    }
    printf("\nTotal execution time: %d units\n", total_time);
}

// Display scheduling comparison
void display_scheduling_comparison() {
    printf("\n=== CPU SCHEDULING COMPARISON ===\n");
    round_robin_scheduling();
    priority_scheduling();
    printf("\nRecommendation: ");
    printf("Priority scheduling for system processes, Round Robin for interactive tasks.\n");
}

// Banker's Algorithm for Deadlock Detection
int bankers_algorithm() {
    printf("\n=== BANKER'S ALGORITHM ===\n");
    
    // Simplified banker's algorithm simulation
    int available_resources = 10;
    int total_need = 0;
    
    printf("Resource Allocation Analysis:\n");
    for (int i = 0; i < process_count && i < 5; i++) {
        int need = pcb_table[i].allocated_memory / 100; // Simplified need calculation
        total_need += need;
        printf("Process P%d: Allocated=%d, Need=%d, Safe=%s\n", 
               pcb_table[i].pid % 100, 
               pcb_table[i].allocated_memory / 100,
               need,
               (need <= available_resources) ? "YES" : "NO");
        available_resources -= (need / 2); // Simulate resource allocation
    }
    
    if (available_resources >= 0) {
        printf("RESULT: System is in SAFE state ✓\n");
        return 1;
    } else {
        printf("RESULT: System is in UNSAFE state ❌\n");
        return 0;
    }
}

// Detect deadlock cycles
int detect_deadlock_cycle() {
    printf("\n=== DEADLOCK CYCLE DETECTION ===\n");
    
    int deadlock_found = 0;
    printf("Checking for circular wait conditions...\n");
    
    for (int i = 0; i < process_count; i++) {
        if (pcb_table[i].state == 2) { // Waiting state
            printf("Process P%d is waiting for resource R%d\n", 
                   pcb_table[i].pid % 100, (i % 4) + 1);
            
            // Check if another process holds this resource and is also waiting
            for (int j = 0; j < process_count; j++) {
                if (i != j && pcb_table[j].state == 2 && ((j % 4) + 1) == ((i % 4) + 1)) {
                    printf("DEADLOCK DETECTED: P%d ↔ P%d (Resource R%d)\n", 
                           pcb_table[i].pid % 100, pcb_table[j].pid % 100, (i % 4) + 1);
                    deadlock_found = 1;
                }
            }
        }
    }
    
    if (!deadlock_found) {
        printf("No deadlock cycles detected ✓\n");
    }
    
    return deadlock_found;
}

// FIFO Page Replacement
void fifo_page_replacement() {
    printf("\n=== FIFO PAGE REPLACEMENT ===\n");
    
    // Simulate page requests
    int page_requests[] = {1, 3, 0, 3, 5, 6, 3, 0, 1, 2};
    int num_requests = 10;
    int page_faults = 0;
    int fifo_queue[5] = {-1, -1, -1, -1, -1};
    int front = 0, rear = 0;
    
    printf("Page requests: ");
    for (int i = 0; i < num_requests; i++) {
        printf("%d ", page_requests[i]);
    }
    printf("\n");
    
    printf("FIFO Frames: ");
    for (int i = 0; i < num_requests; i++) {
        int page = page_requests[i];
        int found = 0;
        
        // Check if page is already in frame
        for (int j = 0; j < 5; j++) {
            if (fifo_queue[j] == page) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            // Page fault - replace using FIFO
            fifo_queue[front] = page;
            front = (front + 1) % 5;
            page_faults++;
        }
        
        printf("[");
        for (int j = 0; j < 5; j++) {
            if (fifo_queue[j] != -1) printf("%d ", fifo_queue[j]);
            else printf("- ");
        }
        printf("] ");
        if (!found) printf("(FAULT)");
        printf("\n                ");
    }
    
    printf("\nFIFO Page Faults: %d/%d\n", page_faults, num_requests);
}

// LRU Page Replacement
void lru_page_replacement() {
    printf("\n=== LRU PAGE REPLACEMENT ===\n");
    
    // Simulate same page requests as FIFO for comparison
    int page_requests[] = {1, 3, 0, 3, 5, 6, 3, 0, 1, 2};
    int num_requests = 10;
    int page_faults = 0;
    int lru_frames[5] = {-1, -1, -1, -1, -1};
    int last_used[5] = {-1, -1, -1, -1, -1};
    
    printf("Page requests: ");
    for (int i = 0; i < num_requests; i++) {
        printf("%d ", page_requests[i]);
    }
    printf("\n");
    
    printf("LRU Frames:  ");
    for (int i = 0; i < num_requests; i++) {
        int page = page_requests[i];
        int found = -1;
        
        // Check if page is already in frame
        for (int j = 0; j < 5; j++) {
            if (lru_frames[j] == page) {
                found = j;
                break;
            }
        }
        
        if (found != -1) {
            // Page hit - update last used time
            last_used[found] = i;
        } else {
            // Page fault - find LRU frame
            int lru_index = 0;
            for (int j = 1; j < 5; j++) {
                if (last_used[j] < last_used[lru_index]) {
                    lru_index = j;
                }
            }
            
            lru_frames[lru_index] = page;
            last_used[lru_index] = i;
            page_faults++;
        }
        
        printf("[");
        for (int j = 0; j < 5; j++) {
            if (lru_frames[j] != -1) printf("%d ", lru_frames[j]);
            else printf("- ");
        }
        printf("] ");
        if (found == -1) printf("(FAULT)");
        printf("\n                ");
    }
    
    printf("\nLRU Page Faults: %d/%d\n", page_faults, num_requests);
}

// Display page replacement comparison
void display_page_replacement_comparison() {
    printf("\n=== PAGE REPLACEMENT COMPARISON ===\n");
    fifo_page_replacement();
    lru_page_replacement();
    printf("\nRecommendation: LRU typically performs better than FIFO\n");
    printf("but requires more overhead to track page usage.\n");
}
