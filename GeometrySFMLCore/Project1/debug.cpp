#include "debug.h"

#include <string>
#include <iostream>

void logDebug(DEBUG_TYPE type, std::string s, bool doEndl)
{
    std::string out;

    switch (type)
    {
    case D_DEBUG:
        out = "[+] " + s;
        break;
    case D_ERROR:
        out = "[!] " + s;
        break;
    case D_CLI:
        out = s;
        break;
    default:
        return;
    }

    if (doEndl) {
        std::cout << s << std::endl;
    }
    else {
        std::cout << s;
    }
}

void displayHelp(void)
{
    LOG_DEBUG("file-finder <dir> <substring1>[<substring2> [<substring3>]...]");
}