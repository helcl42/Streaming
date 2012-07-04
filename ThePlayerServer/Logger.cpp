#include "Logger.h"
#include "Message.h"

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

void Logger::log(int socket, const std::string msg, LogType level) {
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

void Logger::log(const std::string msg, LogType level) {
    log(0, msg, level);
}

void Logger::logData(int socket, const char* data, int size, LogType level) {
    pthread_mutex_lock(&m_mutex);
    std::stringstream out;    
    out << createMessage(socket, std::string(data), level);    
    out << "\tSize: ";    
    out << size;    
    
    if (m_bStdOutput) {
        std::cout << out.str() << std::endl;
    }
    if (!m_outFile.is_open()) {
        m_outFile.open("log.log");
    }    
    m_outFile << out.str() << std::endl;
    pthread_mutex_unlock(&m_mutex);
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
    deleteNewLines(time);
    return time;
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



