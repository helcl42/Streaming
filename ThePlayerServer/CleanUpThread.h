/* 
 * File:    CleanUpThread.h
 * project: ThePlayerServer
 * Author:  lubos
 *
 * Created on June 18, 2012, 2:08 AM
 */

#ifndef CLEANUPTHREAD_H
#define	CLEANUPTHREAD_H

#include "Thread.h"
#include "StreamServerThread.h"

class StreamServer;

class CleanUpThread : public Thread {
private:
    StreamServer* m_pServerInstance;

public:
    CleanUpThread(StreamServer* ins);    
    ~CleanUpThread() {}
    
protected:
    void ThreadProcedure();
    
    void clean();
    void shringThreadArray(int toDelete);
};

#endif	/* CLEANUPTHREAD_H */

