/*
 * process_manager.c - Implementation for process management module
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "process_manager.h"

// For simplicity, we use a static array to hold process information
#define MAX_PROCESSES 10
PCB pcb_table[MAX_PROCESSES];
int process_count = 0;

// Function to fetch real process data from Windows
void init_process_manager() {
    // Seed the random number generator
    srand((unsigned int)time(NULL));
    
    process_count = 0;
    
    // Use Windows tasklist command to get real processes
    FILE *fp = popen("tasklist /fo csv", "r");
    if (fp == NULL) {
        printf("Error: Cannot execute tasklist command. Using dummy data.\n");
        // Fallback to dummy data
        init_dummy_processes();
        return;
    }
    
    char buffer[1024];
    char *token;
    int line_count = 0;
    
    // Skip the header line
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        line_count++;
    }
    
    // Parse process data
    while (fgets(buffer, sizeof(buffer), fp) != NULL && process_count < MAX_PROCESSES) {
        if (strlen(buffer) > 10) { // Skip empty lines
            // Parse CSV format: "Image Name","PID","Session Name","Session#","Mem Usage"
            token = strtok(buffer, "\"");
            if (token != NULL) {
                token = strtok(NULL, "\""); // Get process name
                if (token != NULL) {
                    strncpy(pcb_table[process_count].name, token, sizeof(pcb_table[process_count].name) - 1);
                    pcb_table[process_count].name[sizeof(pcb_table[process_count].name) - 1] = '\0';
                    
                    token = strtok(NULL, "\""); // Skip comma
                    token = strtok(NULL, "\""); // Get PID
                    if (token != NULL) {
                        pcb_table[process_count].pid = atoi(token);
                        
                        // Skip session name and number
                        token = strtok(NULL, "\""); // Skip comma
                        token = strtok(NULL, "\""); // Skip session name
                        token = strtok(NULL, "\""); // Skip comma
                        token = strtok(NULL, "\""); // Skip session number
                        token = strtok(NULL, "\""); // Skip comma
                        token = strtok(NULL, "\""); // Get memory usage
                        
                        if (token != NULL) {
                            // Parse memory usage (remove K and commas)
                            char *mem_str = token;
                            char clean_mem[32];
                            int j = 0;
                            for (int i = 0; mem_str[i] != '\0' && j < 31; i++) {
                                if (isdigit(mem_str[i])) {
                                    clean_mem[j++] = mem_str[i];
                                }
                            }
                            clean_mem[j] = '\0';
                            
                            int mem_kb = atoi(clean_mem);
                            pcb_table[process_count].allocated_memory = mem_kb;
                            pcb_table[process_count].mem_usage = (mem_kb / 1024.0); // Convert to MB
                        }
                        
                        // Assign random values for simulation parameters
                        pcb_table[process_count].cpu_usage = (rand() % 50) / 10.0 + 1.0;
                        pcb_table[process_count].arrival_time = rand() % 10;
                        pcb_table[process_count].burst_time = rand() % 10 + 1;
                        pcb_table[process_count].priority = rand() % 5 + 1;
                        pcb_table[process_count].state = 0; // Ready state
                        
                        process_count++;
                    }
                }
            }
        }
    }
    
    pclose(fp);
    
    if (process_count == 0) {
        printf("No processes found. Using dummy data.\n");
        init_dummy_processes();
    } else {
        printf("Loaded %d real processes from system.\n", process_count);
    }
}

// Fallback function for dummy processes
void init_dummy_processes() {
    process_count = 5;
    
    for (int i = 0; i < process_count; i++) {
        pcb_table[i].pid = 100 + i;
        snprintf(pcb_table[i].name, sizeof(pcb_table[i].name), "Process_%c", 'A' + i);
        pcb_table[i].cpu_usage = (rand() % 50) / 10.0 + 5.0;
        pcb_table[i].mem_usage = (rand() % 50) / 10.0 + 5.0;
        pcb_table[i].arrival_time = rand() % 10;
        pcb_table[i].burst_time = rand() % 10 + 1;
        pcb_table[i].priority = rand() % 5 + 1;
        pcb_table[i].allocated_memory = rand() % 200 + 50;
        pcb_table[i].state = 0;
    }
}

// Function to display the process table
void display_process_table() {
    printf("\n----- Process Table -----\n");
    printf("PID\tName\t\tCPU%%\tMEM%%\tState\n");
    for (int i = 0; i < process_count; i++) {
        printf("%d\t%s\t\t%.1f\t%.1f\t%d\n",
               pcb_table[i].pid,
               pcb_table[i].name,
               pcb_table[i].cpu_usage,
               pcb_table[i].mem_usage,
               pcb_table[i].state);
    }
}
