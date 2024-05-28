#include "log.h"

#include <iomanip>
#include <sstream>

std::string Log::GetCurrentServerTime() {
    std::time_t time = std::time(nullptr);
    std::tm* now = std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(now, "[Date: %Y-%m-%d Time: %H:%M:%S]");

    return oss.str();
}