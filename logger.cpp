#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <memory>
#include <sstream>
#include <ctime>
#include <iomanip>


enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class LogPolicyInterface {
public:
    virtual void open_ostream(const std::string& name) = 0;
    virtual void close_ostream() = 0;
    virtual void write(const std::string& message) = 0;
    virtual ~LogPolicyInterface() {}
};

class FileLogPolicy : public LogPolicyInterface {
    std::unique_ptr<std::ofstream> outStream;

public:
    FileLogPolicy() : outStream(new std::ofstream) {}

    void open_ostream(const std::string& name) override {
        outStream->open(name, std::ofstream::out | std::ofstream::app);
        if (!outStream->is_open()) {
            throw std::runtime_error("Logger: Unable to open an output stream");
        }
    }

    void close_ostream() override {
        if (outStream) {
            outStream->close();
        }
    }

    void write(const std::string& message) override {
        (*outStream) << message << std::endl;
    }

    ~FileLogPolicy() {
        if (outStream) {
            close_ostream();
        }
    }
};

template<typename LogPolicy>
class Logger {
private:
    LogLevel logLevel;
    std::unique_ptr<LogPolicy> policy;
    std::mutex writeMutex;

    // 获取当前时间并格式化
    std::string get_time() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // 日志记录实现
    void log_impl(const std::string& level, const std::string& message) {
        std::lock_guard<std::mutex> lock(writeMutex);
        policy->write("[" + get_time() + "] " + level + ": " + message);
    }

public:
    Logger(const std::string& name, LogLevel level) : logLevel(level), policy(new LogPolicy) {
        policy->open_ostream(name);
    }

    void set_level(LogLevel level) {
        logLevel = level;
    }

    void debug(const std::string& message) {
        if (logLevel <= DEBUG) log_impl("DEBUG", message);
    }

    void info(const std::string& message) {
        if (logLevel <= INFO) log_impl("INFO", message);
    }

    void warn(const std::string& message) {
        if (logLevel <= WARN) log_impl("WARN", message);
    }

    void error(const std::string& message) {
        if (logLevel <= ERROR) log_impl("ERROR", message);
    }

    ~Logger() {
        if (policy) {
            policy->close_ostream();
        }
    }
};

// 使用示例
int main() {
    Logger<FileLogPolicy> fileLogger("log.txt", DEBUG);

    fileLogger.debug("This is a debug message.");
    fileLogger.info("This is an info message.");
    fileLogger.warn("This is a warning message.");
    fileLogger.error("This is an error message.");

    return 0;
}
