/* 
 * File:   TCPClientSocket.h
 * Author: lubos
 *
 * Created on June 11, 2012, 1:44 PM
 */

#ifndef TCPCLIENTSOCKET_H
#define	TCPCLIENTSOCKET_H

#include <cstdio>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Utils.h"
#include "Logger.h"
#include "Message.h"
#include "Types.h"

#define BUFFSIZE 1024

class TCPClientSocket {
private:
    int m_iSocketId;
    char* m_cHostname;
    int m_iPort;

public:
    TCPClientSocket(char* hostname, int port);
    ~TCPClientSocket();

    bool connectSocket();
    int getSocketId() const;
    void closeSocket();
};


#endif	/* TCPCLIENTSOCKET_H */

