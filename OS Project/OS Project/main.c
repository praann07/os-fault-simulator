/*
 * main.c - Program entry point with enhanced UI
 */

#include <stdio.h>
#include <stdlib.h>
#include "process_manager.h"
#include "fault_injection.h"
#include "recovery.h"
#include "visualization.h"
#include "algorithms.h"
#include "ui.h"

int main() {
    // Initialize enhanced UI
    enable_console_colors();
    display_welcome_screen();
    
    // Initialize process manager and fetch process list
    init_process_manager();
    
    printf("\n✅ System initialized successfully!\n");
    pause_with_message("Press Enter to continue...");

    int choice = 0;
    while(1) {
        // Display enhanced system monitor
        display_system_monitor();
        display_main_menu_enhanced();
        
        if (scanf("%d", &choice) != 1) {
            printf("[X] Invalid input! Please enter a number.\n");
            while(getchar() != '\n'); // Clear input buffer
            pause_with_message("Press Enter to continue...");
            continue;
        }
        
        switch(choice) {
            case 1:
                display_fault_injection_animation("DEADLOCK");
                inject_deadlock();
                display_recovery_animation("DEADLOCK RESOLUTION");
                detect_and_fix_deadlock();
                break;
                
            case 2:
                display_fault_injection_animation("CPU OVERLOAD");
                inject_cpu_overload();
                display_recovery_animation("CPU OPTIMIZATION");
                detect_and_fix_cpu_overload();
                break;
                
            case 3:
                display_fault_injection_animation("MEMORY THRASHING");
                inject_thrashing();
                display_recovery_animation("MEMORY OPTIMIZATION");
                detect_and_fix_thrashing();
                break;
                
            case 4:
                printf("\n[@] Displaying Algorithm Comparisons...\n");
                display_scheduling_comparison();
                display_page_replacement_comparison();
                break;
                
            case 5:
                printf("\n[?] Running Complete System Analysis...\n");
                bankers_algorithm();
                detect_deadlock_cycle();
                display_gantt_chart_enhanced();
                display_memory_map_enhanced();
                display_resource_graph();
                break;
                
            case 6:
                printf("\n[#] Performance Dashboard\n");
                display_gantt_chart_enhanced();
                display_memory_map_enhanced();
                display_status_panel();
                break;
                
            case 7:
                clear_screen_enhanced();
                printf("==========================================\n");
                printf("                                          \n");
                printf("    Thank you for using the              \n");
                printf("       OS Fault Simulator!               \n");
                printf("                                          \n");
                printf("    Educational simulation                \n");
                printf("       completed successfully             \n");
                printf("                                          \n");
                printf("==========================================\n");
                exit(0);
                
            default:
                printf("[X] Invalid choice! Please select 1-7.\n");
        }

        // Pause before returning to menu
        pause_with_message("Press Enter to return to main menu...");
    }

    return 0;
}
