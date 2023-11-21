#pragma once

#include <string>

void displayHelp(void);

typedef enum {
    D_DEBUG = 0,
    D_ERROR,
    D_CLI
} DEBUG_TYPE;
void logDebug(DEBUG_TYPE type, std::string s, bool doEndl);

#define LOG_ERROR(x) logDebug(D_ERROR, x, true)
#define LOG_DEBUG(x) logDebug(D_DEBUG, x, true)
#define LOG_NOLINE(x) logDebug(D_CLI, x, false)
#define LOG_CMD(x) logDebug(D_CLI, x, false)