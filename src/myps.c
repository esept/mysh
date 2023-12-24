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

            struct passwd *pwd = getpwuid(stats.st_uid);
            char *user = pwd ? pwd->pw_name : "unknown";

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

            // Calculate %CPU
            unsigned long total_time = utime + stime;
            unsigned long seconds = uptime();
            double cpu_usage = 100.0 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);

            unsigned long vsz, rss;
            FILE *statm;
            snprintf(path, sizeof(path), "/proc/%ld/statm", pid);
            statm = fopen(path, "r");
            if (statm)
            {
                fscanf(statm, "%lu %lu", &vsz, &rss);
                fclose(statm);
            }
            rss = rss * getpagesize() / 1024; // Convert to KB
            unsigned long total_mem = total_memory();
            double mem_usage = 100.0 * rss / total_mem;
            printf("%-8s %-5ld %-5.1f %-5.1f %-8lu %-8lu %-8s %-5c", user, pid, cpu_usage, mem_usage, vsz * getpagesize() / 1024, rss * getpagesize() / 1024, "??", stat);

            // Calculate TIME
            unsigned long total_time_seconds = total_time / sysconf(_SC_CLK_TCK);
            unsigned long hours = total_time_seconds / 3600;
            unsigned long minutes = (total_time_seconds % 3600) / 60;
            unsigned long time_seconds = total_time_seconds % 60;

            // Calculate START time
            time_t start_time = time(NULL) - starttime / sysconf(_SC_CLK_TCK);
            struct tm *tm_info = localtime(&start_time);
            char start_str[9];
            strftime(start_str, sizeof(start_str), "%m.%d", tm_info);

            printf(" %-8s %02lu:%02lu:%02lu %-s\n", start_str, hours, minutes, time_seconds, command);
        }
    }

    closedir(dir);
}
