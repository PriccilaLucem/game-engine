#include "./log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void save_to_file(Log* log_entry){
    FILE* file = fopen("engine.log", "a");
    if (file == NULL) {
        fprintf(stderr, "Could not open log file for writing.\n");
        return;
    }
    
    const char* level_str;
    switch (log_entry->level) {
        case LOG_LEVEL_INFO: level_str = "INFO"; break;
        case LOG_LEVEL_WARNING: level_str = "WARNING"; break;
        case LOG_LEVEL_ERROR: level_str = "ERROR"; break;
        default: level_str = "UNKNOWN";
    }

    char* timestamp = ctime(&log_entry->timestamp);
    timestamp[24] = '\0';

    fprintf(file, "[%s] [%s] (%s): %s\n", timestamp, level_str, log_entry->file, log_entry->message);
    fclose(file);
}

void log_message(LogLevel level, const char* message, const char* file) {
    const char* level_str;
    switch (level) {
        case LOG_LEVEL_INFO:
            level_str = "INFO";
            break;
        case LOG_LEVEL_WARNING:
            level_str = "WARNING";
            break;
        case LOG_LEVEL_ERROR:
            level_str = "ERROR";
            break;
        default:
            level_str = "UNKNOWN";
    }

    time_t now = time(NULL);
    char* timestamp = ctime(&now);
    // Remove the newline character from ctime output
    timestamp[24] = '\0';

    printf("[%s] [%s] (%s): %s\n", timestamp, level_str, file, message);
    
    Log log_entry = {level, message, now, file};
    save_to_file(&log_entry);
}

