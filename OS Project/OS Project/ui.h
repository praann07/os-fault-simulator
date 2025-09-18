/*
 * ui.h - Header for enhanced UI module
 */

#ifndef UI_H
#define UI_H

// Color codes for Windows console
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

// Background colors
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

// UI Functions
void enable_console_colors();
void clear_screen_enhanced();
void draw_border(int width, int height);
void draw_loading_bar(int progress, int total);
void display_status_panel();
void display_system_monitor();
void display_process_table_enhanced();
void display_gantt_chart_enhanced();
void display_memory_map_enhanced();
void display_fault_injection_animation(const char* fault_type);
void display_recovery_animation(const char* recovery_type);
void pause_with_message(const char* message);
void display_welcome_screen();
void display_main_menu_enhanced();

#endif // UI_H
