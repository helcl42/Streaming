#include "Logger.h"

Logger* Logger::m_pInstance = NULL;

Logger::Logger() : m_bStdOutput(STD_DEBUG) {
    m_outFile.open("log.log");
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
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

void Logger::log(int socket, const std::string msg, logLevel level) {
    pthread_mutex_lock(&m_mutex);
    std::string mess = createMessage(socket, msg, level);
    if (m_bStdOutput) {
        std::cout << mess << std::endl;
    }
    if (!m_outFile.is_open()) {
        m_outFile.open("log.log");
    }
    m_outFile.flush();
    m_outFile << mess << std::endl;
    pthread_mutex_unlock(&m_mutex);
}

void Logger::logData(int socket, const char* data, int size, logLevel level) {
    pthread_mutex_lock(&m_mutex);
    std::string mess = createMessage(socket, std::string(data), level);
    deleteNewLines(mess);
    mess += "\tSize: ";
    std::stringstream out;
    out << size;
    mess += out.str();

    if (m_bStdOutput) {
        std::cout << mess << std::endl;
    }
    if (!m_outFile.is_open()) {
        m_outFile.open("log.log");
    }
    //m_outFile.flush();
    m_outFile << mess << std::endl;
    pthread_mutex_unlock(&m_mutex);
}

std::string Logger::createMessage(int socket, const std::string msg, logLevel level) {
    std::string message;

    message = getTime();
    deleteNewLines(message);
    switch (level) {
        case LOG_LEVEL_INFO:
            message += " [ INFO ]:\t";
            break;
        case LOG_LEVEL_WARNING:
            message += " [ WARNING ]:\t";
            break;
        case LOG_LEVEL_ERROR:
            message += " [ ERROR ]:\t";
            break;
        case LOG_LEVEL_FATAL:
            message += " [ FATAL ]:\t";
            break;
        case LOG_LEVEL_DATA:
            message += " [ DATA ]:\t";
            break;
        default:
            message += " [ UNKNOWN ]:\t";
            break;
    }
    message += "Socket: ";
    std::stringstream out;
    out << socket;
    message += out.str();
    message += "\t";
    message += msg;
    return message;
}

std::string Logger::getTime() const {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return std::string(asctime(timeinfo));
}

void Logger::deleteNewLines(std::string& s) {
    int n = 0;
    for (unsigned int i = 0; i < s.length(); i++) {
        if (s[i] == '\n') {
            n++;
        } else {
            s[i - n] = s[i];
        }
    }
    s.resize(s.length() - n);
}



