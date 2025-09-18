/*
 * recovery.h - Header for recovery module
 */

#ifndef RECOVERY_H
#define RECOVERY_H

void detect_and_fix_deadlock();
void detect_and_fix_cpu_overload();
void detect_and_fix_thrashing();

#endif // RECOVERY_H
