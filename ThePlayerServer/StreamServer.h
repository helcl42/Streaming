/* 
 * File:   StreamServer.h
 * Author: lubos
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

#define DEBUG


class StreamServer {
protected:
    TCPServerSocket* m_pServerSocket;
    CleanUpThread* m_pCleaner;
    StreamServerThread** m_pThreads;
    int m_iThreadCount;

private:    
    friend class CleanUpThread;
    StreamServer(const StreamServer& ss);
    StreamServer& operator=(const StreamServer& ss) {
        return *this;
    };

    void reallocThreads();

public:
    StreamServer(int portNumber);
    ~StreamServer();

    void shutdown();
    void startServer();   
};

#endif	/* STREAMSERVER_H */

