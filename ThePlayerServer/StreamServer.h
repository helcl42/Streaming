/* 
 * File:    StreamServer.h
 * Project: ThePlayerServer
 * Author:  lubos
 *
 * Created on May 17, 2012, 11:38 PM
 */

#ifndef STREAMSERVER_H
#define	STREAMSERVER_H


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "StreamServerThread.h"
#include "TCPServerSocket.h"
#include "CleanUpThread.h"
#include "Mutex.h"

#define DEBUG

class StreamServer {
private:
    friend class CleanUpThread;
    
    StreamServer(const StreamServer& ss);
    StreamServer& operator=(const StreamServer& ss) { return *this; }
    
protected:
    TCPServerSocket*     m_pServerSocket;
    CleanUpThread*       m_pCleaner;
    
    StreamServerThread** m_pThreads;
    int                  m_iThreadCount;
    Mutex*               m_pArrayMutex;

    void reallocThreads();

public:
    StreamServer(int portNumber);
    ~StreamServer();

    void shutdown();
    void startServer();

    StreamServerThread** getThreadArray() const;
    void                 setThreadArray(StreamServerThread** array);
    int                  getThreadCount() const;
    void                 setThreadCount(int count);

    TCPServerSocket*     getServerSocket() const;
    Mutex*               getArrayMutex() const;
};

#endif	/* STREAMSERVER_H */

