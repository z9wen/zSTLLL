#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    Logger(LogLevel level) : logLevel(level) {}

    void debug(const std::string& message) {
        if (logLevel <= DEBUG) log("DEBUG", message);
    }

    void info(const std::string& message) {
        if (logLevel <= INFO) log("INFO", message);
    }

    void warn(const std::string& message) {
        if (logLevel <= WARN) log("WARN", message);
    }

    void error(const std::string& message) {
        if (logLevel <= ERROR) log("ERROR", message);
    }

private:
    LogLevel logLevel;

    void log(const std::string& level, const std::string& message) {
        // 获取当前时间
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        std::string strTime = oss.str();

        // 输出日志
        std::cout << "[" << strTime << "] " << level << ": " << message << std::endl;
    }
};


// 使用示例
int main() {
    Logger logger(DEBUG); // 设置日志级别

    logger.debug("This is a debug message.");
    logger.info("This is an info message.");
    logger.warn("This is a warning message.");
    logger.error("This is an error message.");

    return 0;
}
