/*
 * algorithms.h - Header for OS algorithm implementations
 */

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "process_manager.h"

// CPU Scheduling Algorithms
void round_robin_scheduling();
void priority_scheduling();
void display_scheduling_comparison();

// Deadlock Detection and Prevention
int bankers_algorithm();
int detect_deadlock_cycle();
void display_banker_state();

// Memory Management
void fifo_page_replacement();
void lru_page_replacement();
void display_page_replacement_comparison();

// Resource structures
typedef struct {
    int process_id;
    int resource_id;
    int allocated;
    int requested;
} ResourceAllocation;

// Page replacement structures
typedef struct {
    int page_number;
    int frame_number;
    int last_used_time;
    int loaded_time;
} PageFrame;

#endif // ALGORITHMS_H
