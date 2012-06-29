#include "StreamServer.h"

CleanUpThread::CleanUpThread(StreamServer* ins) : m_pServerInstance(ins) {
}

void CleanUpThread::ThreadProcedure() {
    while (true) {       
        Logger::getInstance()->log(m_pServerInstance->m_pServerSocket->getSocketId(), "RUNNING CLEANER", LOG_LEVEL_INFO);
        clean();
        sleep(60);
    }
}

void CleanUpThread::clean() {
    StreamServerThread** threads = m_pServerInstance->m_pThreads;
    int threadCount = m_pServerInstance->m_iThreadCount;
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
    StreamServerThread** threads = m_pServerInstance->m_pThreads;
    int threadCount = m_pServerInstance->m_iThreadCount;

    int newSize = threadCount - toDelete;
    StreamServerThread** newThreads = new StreamServerThread*[newSize];

    for (int j = 0, i = 0; i < threadCount; i++) {
        if (threads[i] != NULL) {
            newThreads[j] = threads[i];
            j++;
        }
    }

    m_pServerInstance->m_iThreadCount = newSize;
    m_pServerInstance->m_pThreads = newThreads;
    printf("Now living threads: %d\n", newSize);
}
