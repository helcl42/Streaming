/* 
 * File:   StreamServerThread.h
 * Author: lubos
 *
 * Created on May 17, 2012, 11:39 PM
 */

#ifndef STREAMSERVERTHREAD_H
#define	STREAMSERVERTHREAD_H


#include <iostream>
#include <fstream>

#include "Utils.h"
#include "Thread.h"
#include "Types.h"
#include "Message.h"
#include "Logger.h"
#include "Library.h"

#define BUFFSIZE 1024

class StreamServerThread : public Thread {
private:
    int      m_iId;
    int      m_iSocketId;
    Library* m_library;    

protected:
    void ThreadProcedure();

public:
    StreamServerThread(int id, int socketId);
    ~StreamServerThread();
    
    void     sendRawData(char* data, int length);
    char*    receiveRawData();

    Message* receiveMessage();
    void     sendMessage(Message* message);
    
    bool     sendBinaryFile(std::string path);
    bool     sendQueryResult();

    int      getId() const;
    int      getSocketId() const;
};

#endif	/* STREAMSERVERTHREAD_H */

