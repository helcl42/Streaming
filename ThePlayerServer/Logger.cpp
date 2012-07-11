#include "Logger.h"
#include "Message.h"
#include "Utils.h"

Logger* Logger::m_pInstance = NULL;

Logger::Logger() : m_bStdOutput(STD_DEBUG) {
    m_outFile.open("log.log");
}

Logger* Logger::getInstance() {
    if (!m_pInstance) {
        m_pInstance = new Logger();
    }
    return m_pInstance;
}

Logger::~Logger() {
    m_outFile.close();
}

void Logger::log(int socket, const std::string msg, LogType level) {
    std::string mess = createMessage(socket, msg, level);
    writeResult(mess);
}

void Logger::log(const std::string msg, LogType level) {
    log(0, msg, level);
}

void Logger::logData(const char* data, int size, LogType level) {
    logData(0, data, size, level);
}

void Logger::logData(int socket, const char* data, int size, LogType level) {
    std::stringstream out;
    out << createMessage(socket, std::string(data), level);
    out << " Size: ";
    out << size;

    writeResult(out.str());
}

void Logger::writeResult(std::string message) {
    if (m_bStdOutput) {
        m_stdMutex.lock();
        std::cout << message << std::endl;
        m_stdMutex.unlock();
    }

    m_fileMutex.lock();
    if (!m_outFile.is_open()) {
        m_outFile.open("log.log");
    }
    m_outFile.flush();
    m_outFile << message << std::endl;
    m_fileMutex.unlock();
}

std::string Logger::createMessage(int socket, const std::string msg, LogType level) {
    std::stringstream out;

    out << getTime();
    switch (level) {
        case LOG_LEVEL_INFO:
            out << " [ INFO ]:\t";
            break;
        case LOG_LEVEL_WARNING:
            out << " [ WARNING ]:\t";
            break;
        case LOG_LEVEL_ERROR:
            out << " [ ERROR ]:\t";
            break;
        case LOG_LEVEL_FATAL:
            out << " [ FATAL ]:\t";
            break;
        case LOG_LEVEL_DATA:
            out << " [ DATA ]:\t";
            break;
        default:
            out << " [ UNKNOWN ]:\t";
            break;
    }
    out << "Socket: ";
    out << socket;
    out << "\t";
    out << msg;
    return out.str();
}

std::string Logger::getTime() {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    std::string time(asctime(timeinfo));
    Util::deleteNewLines(time);
    return time;
}

