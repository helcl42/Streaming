#include "StreamServer.h"

StreamServer::StreamServer(int port) {
    m_iThreadCount = 0;
    m_pThreads = NULL;

    m_pServerSocket = new TCPServerSocket(port);
    m_pCleaner = new CleanUpThread(this);
}

StreamServer::~StreamServer() {
    for (int i = 0; i < m_iThreadCount; i++) {
        SAFE_DELETE(m_pThreads[i]);
    }
    SAFE_DELETE_ARRAY(m_pThreads);
    SAFE_DELETE(m_pCleaner);
    SAFE_DELETE(m_pServerSocket);
}

void StreamServer::startServer() {
    int socketId = 0;

    m_pCleaner->RunThread();
    Logger::getInstance()->log(m_pServerSocket->getSocketId(), "SERVER STARTED", LOG_LEVEL_INFO);

    while (true) {
        if (m_pServerSocket->isOpened()) {
            socketId = m_pServerSocket->acceptSocket();
            Logger::getInstance()->log(m_pServerSocket->getSocketId(), "Is new socketId", LOG_LEVEL_INFO);
            reallocThreads();
            StreamServerThread* newThread = new StreamServerThread(m_iThreadCount, socketId);
            m_pThreads[m_iThreadCount - 1] = newThread;
            newThread->setRunning(true);
            newThread->RunThread();
        } else {
            Logger::getInstance()->log(m_pServerSocket->getSocketId(), "Socket is not openned.", LOG_LEVEL_FATAL);
            exit(1);
        }
        Logger::getInstance()->log(m_pServerSocket->getSocketId(), "New count of clients", LOG_LEVEL_INFO);
    }

    for (int i = 0; i < m_iThreadCount; i++)
        m_pThreads[i]->WaitForThreadToExit();

    shutdown();
}

void StreamServer::reallocThreads() {
    StreamServerThread** tmp = m_pThreads;
    m_iThreadCount++;
    m_pThreads = new StreamServerThread* [m_iThreadCount];

    for (int i = 0; i < m_iThreadCount - 1; i++)
        m_pThreads[i] = tmp[i];

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