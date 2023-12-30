//
// Created by XuZY on 2023/12/2.
//

#ifndef EDU_1S_C_PROJECT_MYSHELL_SRC_MYPS_H_
#define EDU_1S_C_PROJECT_MYSHELL_SRC_MYPS_H_

#include <stdio.h>

static long clk_tck;
static long page_size;

/**
 * Initializes the constants used in the program.
 */
void init_constants();

/**
 * Calculates the total memory available on the system.
 *
 * @return The total memory in kilobytes.
 */
unsigned long total_memory();

/**
 * Retrieves the system uptime.
 *
 * @return The system uptime in seconds.
 */
unsigned long uptime();

/**
 * Retrieves the username associated with a given file's owner.
 *
 * @param stats The file statistics.
 * @return The username if found, otherwise "unknown".
 */
char *get_user(struct stat stats);

/**
 * Calculates the CPU usage percentage for a process.
 *
 * @param utime The user CPU time.
 * @param stime The system CPU time.
 * @return The CPU usage percentage.
 */
double get_cpu_usage(unsigned long utime, unsigned long stime);

/**
 * Calculates the memory usage percentage for a process.
 *
 * @param rss The resident set size (RSS) of the process.
 * @return The memory usage percentage.
 */
double get_mem_usage(unsigned long rss);

/**
 * Prints the information of a process.
 *
 * @param user The username of the process owner.
 * @param pid The process ID.
 * @param cpu_usage The CPU usage percentage.
 * @param mem_usage The memory usage percentage.
 * @param vsz The virtual memory size (VSZ) of the process.
 * @param rss The resident set size (RSS) of the process.
 * @param stat The process status.
 * @param start_str The start time of the process.
 * @param hours The CPU usage hours.
 * @param minutes The CPU usage minutes.
 * @param time_seconds The CPU usage seconds.
 * @param command The command associated with the process.
 */
void print_process_info(char *user, long pid, double cpu_usage, double mem_usage, unsigned long vsz,
                        unsigned long rss, char stat, char *start_str, unsigned long hours, unsigned long minutes,
                        unsigned long time_seconds, char *command, int setGreenColor);

/**
 * Retrieves the start time of a process.
 *
 * @param starttime The start time of the process.
 * @return The formatted start time string.
 */
char *get_start_time(unsigned long starttime);

/**
 * Calculates the CPU usage time in hours, minutes, and seconds.
 *
 * @param utime The user CPU time.
 * @param stime The system CPU time.
 * @param hours The output variable for CPU usage hours.
 * @param minutes The output variable for CPU usage minutes.
 * @param seconds The output variable for CPU usage seconds.
 */
void get_time(unsigned long utime, unsigned long stime, unsigned long *hours, unsigned long *minutes, unsigned long *seconds);

/**
 * Retrieves and prints the information of all running processes.
 * @return 0 if successful.
 */
int command_myps();

#endif // EDU_1S_C_PROJECT_MYSHELL_SRC_MYPS_H_
