/**
 * @file myps.c
 * @brief Implementation of functions related to process status.
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>
#include "myps.h"

unsigned long total_memory()
{
    unsigned long mem;
    FILE *f = fopen("/proc/meminfo", "r");
    if (f == NULL)
    {
        perror("fopen");
        return 0;
    }
    if (fscanf(f, "MemTotal: %lu kB", &mem) != 1)
    {
        perror("fscanf");
        fclose(f);
        return 0;
    }
    fclose(f);
    return mem;
}

unsigned long uptime()
{
    double uptime;
    FILE *f = fopen("/proc/uptime", "r");
    if (f == NULL)
    {
        perror("fopen");
        return 0;
    }
    if (fscanf(f, "%lf", &uptime) != 1)
    {
        perror("fscanf");
        fclose(f);
        return 0;
    }
    fclose(f);
    return (unsigned long)uptime;
}

char *get_user(struct stat stats)
{
    struct passwd *pwd = getpwuid(stats.st_uid);
    return pwd ? pwd->pw_name : "unknown";
}

double get_cpu_usage(unsigned long utime, unsigned long stime)
{
    unsigned long total_time = utime + stime;
    unsigned long seconds = uptime();
    return 100.0 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
}

double get_mem_usage(unsigned long rss)
{
    unsigned long total_mem = total_memory();
    return 100.0 * rss / total_mem;
}

void print_process_info(char *user, long pid, double cpu_usage, double mem_usage, unsigned long vsz, unsigned long rss, char stat, char *start_str, unsigned long hours, unsigned long minutes, unsigned long time_seconds, char *command)
{
    printf("%-8s %-5ld %-5.1f %-5.1f %-8lu %-8lu %-8s %-5c %-8s %02lu:%02lu:%02lu %-s\n", user, pid, cpu_usage, mem_usage, vsz * getpagesize() / 1024, rss * getpagesize() / 1024, "??", stat, start_str, hours, minutes, time_seconds, command);
}

char *get_start_time(unsigned long starttime)
{
    time_t boot_time = time(NULL) - uptime();
    time_t start_time = boot_time + starttime / sysconf(_SC_CLK_TCK);
    struct tm *tm_info = localtime(&start_time);

    static char start_str[6];
    snprintf(start_str, sizeof(start_str), "%02d.%02d", tm_info->tm_mon + 1, tm_info->tm_mday);

    return start_str;
}

void get_time(unsigned long utime, unsigned long stime, unsigned long *hours, unsigned long *minutes, unsigned long *seconds)
{
    unsigned long total_time = utime + stime;
    total_time = total_time / sysconf(_SC_CLK_TCK); // Convert to seconds

    *hours = total_time / 3600;
    *minutes = (total_time % 3600) / 60;
    *seconds = total_time % 60;
}

void command_myps()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");
    if (!dir)
    {
        perror("opendir");
        return;
    }

    printf("%-8s %-5s %-5s %-5s %-8s %-8s %-8s %-5s %-8s %-8s %-8s\n",
           "USER", "PID", "%CPU", "%MEM", "VSZ", "RSS", "TTY", "STAT", "START", "TIME", "COMMAND");

    while ((entry = readdir(dir)) != NULL)
    {
        char *endptr;
        long pid = strtol(entry->d_name, &endptr, 10);
        if (*endptr == '\0')
        {
            char path[256];
            struct stat stats;

            snprintf(path, sizeof(path), "/proc/%ld", pid);
            if (stat(path, &stats) == -1)
            {
                perror("stat");
                continue;
            }

            char *user = get_user(stats);

            FILE *stat_file;
            char stat_path[256];
            snprintf(stat_path, sizeof(stat_path), "/proc/%ld/stat", pid);

            stat_file = fopen(stat_path, "r");
            if (!stat_file)
            {
                perror("fopen");
                continue;
            }

            char command[256];
            char stat;
            unsigned long utime, stime, cutime, cstime, starttime;
            int result = fscanf(stat_file, "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %lu %lu %lu %lu %*d %*d %*d %*d %*d %*d %lu", command, &stat, &utime, &stime, &cutime, &cstime, &starttime);

            fclose(stat_file);

            if (result != 7)
            {
                perror("fscanf");
                continue;
            }

            double cpu_usage = get_cpu_usage(utime, stime);

            unsigned long vsz, rss;
            FILE *statm;
            snprintf(path, sizeof(path), "/proc/%ld/statm", pid);
            statm = fopen(path, "r");
            if (statm)
            {
                fscanf(statm, "%lu %lu", &vsz, &rss);
                fclose(statm);
            }
            double mem_usage = get_mem_usage(rss);
            char *start_str = get_start_time(starttime);
            unsigned long hours, minutes, time_seconds;
            get_time(utime, stime, &hours, &minutes, &time_seconds);

            print_process_info(user, pid, cpu_usage, mem_usage, vsz, rss, stat, start_str, hours, minutes, time_seconds, command);
        }
    }

    closedir(dir);
}
