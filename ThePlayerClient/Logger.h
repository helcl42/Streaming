/* 
 * File:   Logger.h
 * Author: lubos
 *
 * Created on June 11, 2012, 1:06 PM
 */

#ifndef LOGGER_H
#define	LOGGER_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <pthread.h>

typedef enum {
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_DATA
} logLevel;

#define STD_DEBUG true

class Logger {
private:
    static Logger* m_pInstance;
    std::ofstream m_outFile;
    bool m_bStdOutput;
    pthread_mutexattr_t m_attr;
    pthread_mutex_t m_mutex;

    Logger();

public:
    static Logger* getInstance();
    virtual ~Logger();

    void log(int socket, const std::string msg, logLevel level = LOG_LEVEL_WARNING);
    void logData(int socket, const char* data, int size, logLevel level = LOG_LEVEL_DATA);

    std::string createMessage(int socket, const std::string msg, logLevel level);
    std::string getTime() const;
    void deleteNewLines(std::string& s);
};

#endif	/* LOGGER_H */
