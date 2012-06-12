#include "StreamServer.h"

StreamServer::StreamServer(int port) {
    m_iThreadCount = 0;
    m_pThreads = NULL;

    m_pServerSocket = new TCPServerSocket(port);
}

StreamServer::~StreamServer() {
    for (int i = 0; i < m_iThreadCount; i++)
        SAFE_DELETE(m_pThreads[i]);
    SAFE_DELETE_ARRAY(m_pThreads);

    SAFE_DELETE(m_pServerSocket);
}

void StreamServer::listenLoop() {
    int socketId = 0;

    Logger::getInstance()->log(m_pServerSocket->getSocketId(), "SERVER STARTED", LOG_LEVEL_INFO);

    while (true) {
        if (m_pServerSocket->isOpened()) {
            socketId = m_pServerSocket->acceptSocket();
            Logger::getInstance()->log(m_pServerSocket->getSocketId(), "Is new socketId", LOG_LEVEL_INFO);
            reallocThreads();
            StreamServerThread* newThread = new StreamServerThread(m_iThreadCount, socketId);
            m_pThreads[m_iThreadCount - 1] = newThread;
            newThread->RunThread();
        } else {
            Logger::getInstance()->log(m_pServerSocket->getSocketId(), "Socket is not openned.", LOG_LEVEL_FATAL);
            exit(1);
        }
        Logger::getInstance()->log(m_pServerSocket->getSocketId(), "New count of clients", LOG_LEVEL_INFO);
    }

    for (int i = 0; i < m_iThreadCount; i++)
        m_pThreads[i]->WaitForThreadToExit();

    //	closeSocket();        
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
    Logger::getInstance()->log(m_pServerSocket->getSocketId(), "Closing socket", LOG_LEVEL_INFO);
    m_pServerSocket->closeSocket();
}

