/*
 * ui.c - Enhanced UI implementation with colors and animations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#endif
#include "ui.h"
#include "process_manager.h"

void enable_console_colors() {
#ifdef _WIN32
    // Enable ANSI escape sequences in Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
#endif
}

void clear_screen_enhanced() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    enable_console_colors();
}

void draw_border(int width, int height) {
    printf(CYAN "╔");
    for(int i = 0; i < width-2; i++) printf("═");
    printf("╗\n");
    
    for(int i = 0; i < height-2; i++) {
        printf("║");
        for(int j = 0; j < width-2; j++) printf(" ");
        printf("║\n");
    }
    
    printf("╚");
    for(int i = 0; i < width-2; i++) printf("═");
    printf("╝" RESET "\n");
}

void draw_loading_bar(int progress, int total) {
    int bar_width = 30;
    int filled = (progress * bar_width) / total;
    
    printf(YELLOW "[");
    for(int i = 0; i < bar_width; i++) {
        if(i < filled) printf(GREEN "█");
        else printf(DIM "░");
    }
    printf(YELLOW "] %d%%" RESET, (progress * 100) / total);
}

void display_loading_animation(const char* text) {
    clear_screen_enhanced();
    printf(CYAN "\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                                                          ║\n");
    printf("║          🖥️  OPERATING SYSTEM SIMULATOR  🖥️             ║\n");
    printf("║                                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n" RESET);
    
    printf(YELLOW "\n%s\n" RESET, text);
    
    for(int i = 0; i <= 10; i++) {
        printf("\r" CYAN "Progress: ");
        draw_loading_bar(i, 10);
        fflush(stdout);
#ifdef _WIN32
        Sleep(200);
#else
        usleep(200000);
#endif
    }
    printf("\n");
}

void display_status_panel() {
    printf("+-------------- SYSTEM STATUS --------------+\n");
    printf("| [*] System:     ONLINE                    |\n");
    printf("| [*] Processes:  %2d active                |\n", process_count);
    
    // Calculate average CPU usage
    float avg_cpu = 0;
    for(int i = 0; i < process_count; i++) {
        avg_cpu += pcb_table[i].cpu_usage;
    }
    avg_cpu /= process_count;
    
    printf("| [*] CPU Usage:  %.1f%%                    |\n", avg_cpu);
    
    time_t now = time(0);
    char* time_str = ctime(&now);
    time_str[strlen(time_str)-1] = '\0'; // Remove newline
    printf("| [*] Time:       %s |\n", time_str + 11); // Just show time part
    printf("+-------------------------------------------+\n");
}

void display_process_table_enhanced() {
    printf("\n");
    printf("+======================================================+\n");
    printf("|                 PROCESS TABLE                        |\n");
    printf("+======================================================+\n");
    printf("| PID  | Process Name      | CPU%%  | MEM%%  | State   |\n");
    printf("+------+-------------------+-------+-------+---------+\n");
    
    for (int i = 0; i < process_count && i < 8; i++) {
        printf("| %4d | %-17s | %5.1f | %5.1f | ", 
               pcb_table[i].pid, 
               pcb_table[i].name, 
               pcb_table[i].cpu_usage, 
               pcb_table[i].mem_usage);
        
        switch(pcb_table[i].state) {
            case 0: printf("READY   "); break;
            case 1: printf("RUNNING "); break;
            case 2: printf("WAITING "); break;
            default: printf("UNKNOWN "); break;
        }
        printf("|\n");
    }
    
    if(process_count > 8) {
        printf("| ... and %d more processes ...                      |\n", process_count - 8);
    }
    
    printf("+======================================================+\n");
}

void display_fault_injection_animation(const char* fault_type) {
    printf("\n[!] INJECTING %s [!]\n", fault_type);
    
    const char* animations[] = {"|", "/", "-", "\\", "|", "/", "-", "\\"};
    
    for(int i = 0; i < 16; i++) {
        printf("\r%s Simulating system fault...", animations[i % 8]);
        fflush(stdout);
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
    }
    printf("\n[!] FAULT INJECTED SUCCESSFULLY [!]\n");
}

void display_recovery_animation(const char* recovery_type) {
    printf("\n[+] STARTING RECOVERY: %s [+]\n", recovery_type);
    
    const char* animations[] = {"|", "/", "-", "\\", "|", "/", "-", "\\"};
    
    for(int i = 0; i < 12; i++) {
        printf("\r%s Running recovery algorithms...", animations[i % 8]);
        fflush(stdout);
#ifdef _WIN32
        Sleep(150);
#else
        usleep(150000);
#endif
    }
    printf("\n[+] RECOVERY COMPLETED SUCCESSFULLY [+]\n");
}

void display_welcome_screen() {
    clear_screen_enhanced();
    
    printf("=====================================\n");
    printf("     OPERATING SYSTEM SIMULATOR     \n");
    printf("=====================================\n");
    printf("\n");
    printf("Features:\n");
    printf("  [✓] Process Management\n");
    printf("  [✓] CPU Scheduling\n");
    printf("  [✓] Memory Management\n");
    printf("  [✓] Deadlock Detection\n");
    printf("\n");
    printf("Loading real system processes...\n");
    
    // Simple loading animation
    for(int i = 0; i <= 10; i++) {
        printf("\rProgress: [");
        for(int j = 0; j < 20; j++) {
            if(j < i * 2) printf("=");
            else printf(" ");
        }
        printf("] %d%%", i * 10);
        fflush(stdout);
#ifdef _WIN32
        Sleep(200);
#else
        usleep(200000);
#endif
    }
    printf("\n");
}

void display_main_menu_enhanced() {
    printf("\n");
    printf("+==========================================================+\n");
    printf("|                    CONTROL PANEL                        |\n");
    printf("+==========================================================+\n");
    printf("|  1. [!] Inject Deadlock                                 |\n");
    printf("|  2. [!] Inject CPU Overload                             |\n");
    printf("|  3. [!] Inject Memory Thrashing                         |\n");
    printf("|  4. [@] Show Algorithm Comparisons                      |\n");
    printf("|  5. [?] Run System Analysis                             |\n");
    printf("|  6. [#] Performance Dashboard                           |\n");
    printf("|  7. [X] Exit Simulation                                 |\n");
    printf("+==========================================================+\n");
    printf("Enter your choice (1-7): ");
}

void pause_with_message(const char* message) {
    printf(DIM "\n%s" RESET, message);
    getchar();
}

void display_system_monitor() {
    clear_screen_enhanced();
    display_status_panel();
    display_process_table_enhanced();
}

void display_gantt_chart_enhanced() {
    printf(BLUE BOLD "\n╔═══════════════ GANTT CHART ═══════════════╗\n" RESET);
    printf(BLUE "║ " RESET "Time:  ");
    
    // Timeline
    for (int i = 0; i <= 20; i += 5) {
        printf(CYAN "%2d" RESET, i);
        if (i < 20) printf("   ");
    }
    printf(BLUE " ║\n");
    
    printf("║ " RESET "CPU:   ");
    for (int i = 0; i < process_count && i < 4; i++) {
        // Color code based on process priority
        if (pcb_table[i].priority <= 2) printf(RED);
        else if (pcb_table[i].priority <= 3) printf(YELLOW);
        else printf(GREEN);
        
        printf("P%d", pcb_table[i].pid % 100);
        for (int j = 0; j < pcb_table[i].burst_time && j < 4; j++) {
            printf("█");
        }
        printf(RESET " ");
    }
    printf(BLUE "║\n");
    printf("╚═══════════════════════════════════════════╝\n" RESET);
}

void display_memory_map_enhanced() {
    printf(GREEN BOLD "\n╔════════════ MEMORY ALLOCATION MAP ════════════╗\n" RESET);
    
    int total_memory = 1000;
    int used_memory = 0;
    
    for (int i = 0; i < process_count && i < 10; i++) {
        used_memory += (int)pcb_table[i].mem_usage;
    }
    
    printf(GREEN "║ " RESET "Total Memory: %d MB | Used: %d MB | Free: %d MB" GREEN " ║\n" RESET, 
           total_memory, used_memory, total_memory - used_memory);
    
    printf(GREEN "║ " RESET "Memory: [");
    
    int blocks_used = (used_memory * 40) / total_memory;
    for (int i = 0; i < 40; i++) {
        if (i < blocks_used) {
            if (i < blocks_used * 0.8) printf(GREEN "█");
            else printf(YELLOW "█");
        } else {
            printf(DIM "░");
        }
    }
    printf(RESET "]" GREEN " ║\n");
    
    // Show top memory consumers
    printf("║ " RESET "Top Memory Users:" GREEN, used_memory);
    for(int i = 0; i < 22; i++) printf(" ");
    printf("║\n");
    
    for (int i = 0; i < process_count && i < 3; i++) {
        printf(GREEN "║ " RESET "  %-12s: %4.1f MB ", 
               pcb_table[i].name, pcb_table[i].mem_usage);
        
        int bars = (int)(pcb_table[i].mem_usage / 5.0);
        for (int j = 0; j < bars && j < 8; j++) {
            if (j < 3) printf(GREEN "▓");
            else if (j < 6) printf(YELLOW "▓");
            else printf(RED "▓");
        }
        printf(RESET);
        for (int j = bars; j < 12; j++) printf(" ");
        printf(GREEN "║\n" RESET);
    }
    
    printf(GREEN "╚═══════════════════════════════════════════════╝\n" RESET);
}
