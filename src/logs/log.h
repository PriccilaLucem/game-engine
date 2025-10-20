#ifndef LOG_H
#define LOG_H

#include <time.h>

typedef enum LogLevel {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
} LogLevel;

typedef struct Log {
    LogLevel level;
    const char* message;
    time_t timestamp;
    const char* file;
}Log;

void log_message(LogLevel level, const char* message, const char* file);



#endif