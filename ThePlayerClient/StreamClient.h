/* 
 * File:    StreamClient.h
 * Project: ThePlayerClient
 * Author:  lubos
 *
 * Created on June 11, 2012, 1:13 PM
 */

#ifndef STREAMCLIENT_H
#define	STREAMCLIENT_H

#include <sys/stat.h>
#include <sys/types.h>


#include "Utils.h"
#include "TCPClientSocket.h"
#include "Logger.h"
#include "Message.h"
#include "Types.h"
#include "Thread.h"

#define BUFFSIZE 1024

class Library;

class StreamClient : public Thread {
protected:
    int              m_iId;
    bool             m_bConnected;
    TCPClientSocket* m_pClientSocket;

    void setId(int id);

public:
    StreamClient(std::string host, int port);
    StreamClient(const char* hostname, int port);
    ~StreamClient();

    bool connectToServer();
    bool disconnect();

    void  sendRawData(char* data, int length);
    char* receiveRawData();
    
    bool     sendMessage(Message* message);
    Message* receiveMessage();

    int getId() const;

    bool isConnected() const;
    void setConnected(bool conn);
};

#endif	/* STREAMCLIENT_H */

