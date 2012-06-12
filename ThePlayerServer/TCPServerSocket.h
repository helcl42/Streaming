/* 
 * File:   TCPClientSocket.h
 * Author: lubos
 *
 * Created on May 18, 2012, 2:41 AM
 */

#ifndef TCPCLIENTSOCKET_H
#define	TCPCLIENTSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Logger.h"
#include "Types.h"
#include "Message.h"

class TCPServerSocket {
private:
    int m_iPort;
    int m_iSocketListen;
    struct sockaddr_in m_listenAddress;
    bool m_bIsOpenned;

public:

    TCPServerSocket(int port);
    ~TCPServerSocket();

    bool openSocket();
    int acceptSocket();
    bool isOpened() const;
    void closeSocket();
    int getSocketId() const;
};


#endif	/* TCPCLIENTSOCKET_H */

