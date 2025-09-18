/*
 * fault_injection.h - Header for fault injection module
 */

#ifndef FAULT_INJECTION_H
#define FAULT_INJECTION_H

void inject_deadlock();
void inject_cpu_overload();
void inject_thrashing();

#endif // FAULT_INJECTION_H
