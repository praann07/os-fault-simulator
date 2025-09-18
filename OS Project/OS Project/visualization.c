/*
 * visualization.c - Implementation for visualization module
 */

#include <stdio.h>
#include <stdlib.h>
#include "visualization.h"
#include "process_manager.h"

// Cross-platform clear screen function
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Enhanced visualization function with multiple views
void display_visualization() {
    clear_screen();
    printf("\n========== OS SIMULATION VISUALIZATION ==========\n");
    
    // Display the updated process table
    display_process_table();
    
    // Display additional visualizations
    printf("\n");
    display_gantt_chart();
    printf("\n");
    display_memory_map();
    printf("\n");
    display_resource_graph();
    
    printf("\n=================================================\n");
    printf("Press Enter to continue...");
    getchar();
}

// Display Gantt chart for CPU scheduling
void display_gantt_chart() {
    printf("--- CPU SCHEDULING GANTT CHART ---\n");
    printf("Time: ");
    for (int i = 0; i < 40; i++) printf("-");
    printf("\n");
    
    printf("      |");
    for (int i = 0; i < process_count && i < 8; i++) {
        printf("P%d|", pcb_table[i].pid % 100);
        for (int j = 0; j < pcb_table[i].burst_time - 1 && j < 4; j++) {
            printf("  |");
        }
    }
    printf("\n");
    
    printf("Time: ");
    int time = 0;
    for (int i = 0; i < process_count && i < 8; i++) {
        printf("%2d", time);
        time += pcb_table[i].burst_time;
        for (int j = 0; j < pcb_table[i].burst_time - 1 && j < 4; j++) {
            printf("   ");
        }
    }
    printf("\n");
}

// Display memory allocation map
void display_memory_map() {
    printf("--- MEMORY ALLOCATION MAP ---\n");
    printf("Memory: [");
    
    int total_memory = 1000; // Simulated total memory in MB
    int used_memory = 0;
    
    for (int i = 0; i < process_count && i < 10; i++) {
        used_memory += (int)pcb_table[i].mem_usage;
    }
    
    // Display memory blocks
    int blocks_used = (used_memory * 20) / total_memory;
    for (int i = 0; i < blocks_used && i < 20; i++) {
        printf("█");
    }
    for (int i = blocks_used; i < 20; i++) {
        printf("░");
    }
    
    printf("] %d/%d MB\n", used_memory, total_memory);
    
    // Show individual process memory usage
    printf("Process Memory Usage:\n");
    for (int i = 0; i < process_count && i < 5; i++) {
        printf("  %-12s: %4.1f MB ", pcb_table[i].name, pcb_table[i].mem_usage);
        int bars = (int)(pcb_table[i].mem_usage / 10.0);
        for (int j = 0; j < bars && j < 10; j++) printf("▓");
        printf("\n");
    }
}

// Display resource allocation graph for deadlock detection
void display_resource_graph() {
    printf("--- RESOURCE ALLOCATION GRAPH ---\n");
    printf("Processes and Resources:\n");
    
    // Simulate resource allocation
    char resources[4] = {'1', '2', '3', '4'};
    
    for (int i = 0; i < process_count && i < 4; i++) {
        printf("Process P%d ", pcb_table[i].pid % 100);
        
        // Show resource allocation based on process state
        if (pcb_table[i].state == 2) { // Waiting state indicates potential deadlock
            printf("---> R%d (WAITING) ", (i + 1) % 4 + 1);
            printf("❌ DEADLOCK DETECTED");
        } else if (pcb_table[i].state == 1) { // Running state
            printf("---> R%d (ALLOCATED) ", (i % 4) + 1);
            printf("✓ RUNNING");
        } else {
            printf("---> R%d (READY) ", (i % 4) + 1);
            printf("○ READY");
        }
        printf("\n");
    }
    
    // Show resource status
    printf("\nResource Status:\n");
    for (int i = 0; i < 4; i++) {
        printf("R%d: ", i + 1);
        int allocated = 0;
        for (int j = 0; j < process_count && j < 4; j++) {
            if ((j % 4) == i && pcb_table[j].state != 0) {
                printf("Allocated to P%d ", pcb_table[j].pid % 100);
                allocated = 1;
                break;
            }
        }
        if (!allocated) printf("Available");
        printf("\n");
    }
}
