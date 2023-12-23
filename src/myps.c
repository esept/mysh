#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "myps.h"
#include "main.h"

void truncateString(char *str, size_t maxLen) {
    size_t len = strlen(str);
    if (len > maxLen) {
        memmove(str + maxLen - 3, "..\0", 3);
    }
}

void truncateFloat(float *value, size_t maxLen) {
    char str[20];
    snprintf(str, sizeof(str), "%.1f", *value);

    size_t len = strlen(str);
    if (len > maxLen) {
        snprintf(str + maxLen - 3, sizeof(str) - maxLen + 2, "..");
        sscanf(str, "%f", value);
    }
}

void truncateUnsignedLong(unsigned long *value, size_t maxLen) {
    char str[20];
    snprintf(str, sizeof(str), "%lu", *value);

    size_t len = strlen(str);
    if (len > maxLen) {
        snprintf(str + maxLen - 3, sizeof(str) - maxLen + 3, "..");
        sscanf(str, "%lu", value);
    }
}

void printHeader() {
    printf("%-15s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %s\n",
           "USER", "PID", "%CPU", "%MEM", "VSZ", "RSS", "TTY", "STAT", "START", "COMMAND");
}

void printProcessInfo(struct ProcessInfo *info) {
    truncateString(info->user, MAX_DISPLAY_LEN);
	truncateFloat(&info->cpu, MAX_DISPLAY_LEN_FOR_FLOAT_AND_UNSIGNED);
    truncateFloat(&info->mem, MAX_DISPLAY_LEN_FOR_FLOAT_AND_UNSIGNED);
    truncateUnsignedLong(&info->vsz, MAX_DISPLAY_LEN_FOR_FLOAT_AND_UNSIGNED);
    truncateUnsignedLong(&info->rss, MAX_DISPLAY_LEN_FOR_FLOAT_AND_UNSIGNED);
    truncateString(info->tty, MAX_DISPLAY_LEN);
    truncateString(info->stat, MAX_DISPLAY_LEN);
    truncateString(info->start, MAX_DISPLAY_LEN);
    truncateString(info->command, MAX_DISPLAY_LEN);

    printf("%-15s %-8d %-8.1f %-8.1f %-8lu %-8lu %-8s %-8s %-8s %s\n",
           info->user, info->pid, info->cpu, info->mem, info->vsz, info->rss,
           info->tty, info->stat, info->start, info->command);
}

void command_myps() {
    DIR *dir;
    struct dirent *ent;
    FILE *file;
    char path[MAX_LINE_LEN];
    char line[MAX_LINE_LEN];

    dir = opendir("/proc");
    if (dir == NULL) {
        getError("opendir");
    }

    printHeader();

    while ((ent = readdir(dir)) != NULL) {
        if (isdigit(ent->d_name[0])) {
            snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
            file = fopen(path, "r");

            if (file != NULL) {
                struct ProcessInfo info;

                if (fgets(line, sizeof(line), file) != NULL) {
                    sscanf(line, "%s %d %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %f %f", 
                           info.stat, &info.pid, &info.cpu, &info.mem);
                }

                snprintf(path, sizeof(path), "/proc/%s/status", ent->d_name);
                file = fopen(path, "r");

                if (file != NULL) {
                    while (fgets(line, sizeof(line), file) != NULL) {
                        if (strncmp(line, "Name:", 5) == 0) {
                            sscanf(line, "Name:\t%s", info.command);
                            truncateString(info.command, MAX_DISPLAY_LEN); // Tronquer le chemin
                        } else if (strncmp(line, "Uid:", 4) == 0) {
                            int uid;
                            sscanf(line, "Uid:\t%d", &uid);
                            snprintf(path, sizeof(path), "/proc/%s/cmdline", ent->d_name);
                            FILE *cmdline = fopen(path, "r");
                            if (cmdline != NULL) {
                                fscanf(cmdline, "%s", info.user);
                                fclose(cmdline);
                            }
                        }
                    }
					// while (fgets(line, sizeof(line), file) != NULL) {
                    //     if (strncmp(line, "Uid:", 4) == 0) {
                    //         int uid;
                    //         sscanf(line, "Uid:\t%d", &uid);
                    //         struct passwd *pw = getpwuid(uid);
                    //         if (pw != NULL) {
                    //             strncpy(info.user, pw->pw_name, sizeof(info.user) - 1);
                    //             info.user[sizeof(info.user) - 1] = '\0';
                    //             truncateString(info.user, MAX_DISPLAY_LEN);
                    //         } else {
                    //             // Gérer le cas où getpwuid échoue
                    //             strncpy(info.user, "UNKNOWN", sizeof(info.user) - 1);
                    //             info.user[sizeof(info.user) - 1] = '\0';
                    //         }
                    //     }
                    // }
                    fclose(file);

                    snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
                    file = fopen(path, "r");

                    if (file != NULL) {
                        fgets(line, sizeof(line), file);
                        sscanf(line, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %*s", info.tty);
                        fclose(file);
                    }

                    snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
                    file = fopen(path, "r");

                    if (file != NULL) {
                        fgets(line, sizeof(line), file);
                        sscanf(line, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", info.start);
                        fclose(file);
                    }

                    snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
                    file = fopen(path, "r");

                    if (file != NULL) {
                        fgets(line, sizeof(line), file);
                        sscanf(line, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", info.time);
                        fclose(file);
                    }

                    printProcessInfo(&info);
                }
            }
        }
    }

    closedir(dir);
    return;
}
