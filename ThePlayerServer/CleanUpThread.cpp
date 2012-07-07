#include "StreamServer.h"

CleanUpThread::CleanUpThread(StreamServer* ins) : m_pServerInstance(ins) {
}

void CleanUpThread::ThreadProcedure() {
    while (true) {               
        Logger::getInstance()->log(m_pServerInstance->getServerSocket()->getSocketId(), "RUNNING CLEANER", LOG_LEVEL_INFO);
        m_pServerInstance->getArrayMutex()->lock();
        clean();
        m_pServerInstance->getArrayMutex()->unlock();
        sleep(60);
    }
}

void CleanUpThread::clean() {    
    StreamServerThread** threads = m_pServerInstance->getThreadArray();
    int threadCount = m_pServerInstance->getThreadCount();
    int tmpCount = 0;
    
    for (int i = 0; i < threadCount; i++) {
        if (!threads[i]->isRunning()) {
            SAFE_DELETE(threads[i]);
            threads[i] = NULL; //done twice
            tmpCount++;
        }
    }
    shringThreadArray(tmpCount);    
}

void CleanUpThread::shringThreadArray(int toDelete) {
    StreamServerThread** threads = m_pServerInstance->getThreadArray();
    int threadCount = m_pServerInstance->getThreadCount();

    int newSize = threadCount - toDelete;
    StreamServerThread** newThreads = new StreamServerThread*[newSize];
    
    for (int j = 0, i = 0; i < threadCount; i++, j++) {
        if (threads[i] != NULL) {
            newThreads[j] = threads[i];            
        }
    }

    m_pServerInstance->setThreadCount(newSize);
    m_pServerInstance->setThreadArray(newThreads);        
    Logger::getInstance()->logData(m_pServerInstance->getServerSocket()->getSocketId(), "NOW LIVING THREADS", newSize, LOG_LEVEL_INFO);
}
