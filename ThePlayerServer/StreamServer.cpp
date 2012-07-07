#include "StreamServer.h"

StreamServer::StreamServer(int port) {
    m_iThreadCount = 0;
    m_pThreads = NULL;    
    m_pServerSocket = new TCPServerSocket(port);
    m_pArrayMutex = new Mutex();
    m_pCleaner = new CleanUpThread(this);    
}

StreamServer::~StreamServer() {    
    m_pArrayMutex->lock();
    for (int i = 0; i < m_iThreadCount; i++) {
        SAFE_DELETE(m_pThreads[i]);
    }
    SAFE_DELETE_ARRAY(m_pThreads);
    m_pArrayMutex->unlock();   
    SAFE_DELETE(m_pCleaner);
    SAFE_DELETE(m_pArrayMutex);
    SAFE_DELETE(m_pServerSocket);    
}

void StreamServer::startServer() {
    int socketId = 0;
    
    Logger::getInstance()->log(m_pServerSocket->getSocketId(), "SERVER STARTED", LOG_LEVEL_INFO);
    m_pCleaner->RunThread();

    while (true) {
        if (m_pServerSocket->isOpened()) {
            socketId = m_pServerSocket->acceptSocket();
            Logger::getInstance()->log(m_pServerSocket->getSocketId(), "NEW CLIENT CONNECTED", LOG_LEVEL_INFO);
            reallocThreads();
            StreamServerThread* newThread = new StreamServerThread(m_iThreadCount, socketId);
            m_pArrayMutex->lock();
            m_pThreads[m_iThreadCount - 1] = newThread;
            m_pArrayMutex->unlock();
            newThread->setRunning(true);
            newThread->RunThread();
        } else {
            Logger::getInstance()->log(m_pServerSocket->getSocketId(), "SOCKET IS NOT OPENED.", LOG_LEVEL_FATAL);
            exit(1);
        }        
    }

    m_pArrayMutex->lock();
    for (int i = 0; i < m_iThreadCount; i++) {
        m_pThreads[i]->WaitForThreadToExit();
    }
    m_pArrayMutex->unlock();
 
    shutdown();
}

void StreamServer::reallocThreads() {
    StreamServerThread** tmp = m_pThreads;
    m_iThreadCount++;
    m_pThreads = new StreamServerThread* [m_iThreadCount];

    m_pArrayMutex->lock();
    for (int i = 0; i < m_iThreadCount - 1; i++) {
        m_pThreads[i] = tmp[i];
    }
    m_pArrayMutex->unlock(); 
    
    SAFE_DELETE_ARRAY(tmp);
}

void StreamServer::shutdown() {
    Logger::getInstance()->log(m_pServerSocket->getSocketId(), "CLOSING SERVER SOCKET", LOG_LEVEL_INFO);
    m_pServerSocket->closeSocket();    
}

StreamServerThread** StreamServer::getThreadArray() const {
    return m_pThreads;
}

void StreamServer::setThreadArray(StreamServerThread** array) {
    m_pThreads = array;
}

int StreamServer::getThreadCount() const {
    return m_iThreadCount;
}

void StreamServer::setThreadCount(int count) {
    m_iThreadCount = count;
}

TCPServerSocket* StreamServer::getServerSocket() const {
    return m_pServerSocket;
}

Mutex* StreamServer::getArrayMutex() const {
    return m_pArrayMutex;
}