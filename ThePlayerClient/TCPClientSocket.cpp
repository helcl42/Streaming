#include "TCPClientSocket.h"

TCPClientSocket::TCPClientSocket(char* hostname, int port) {
    m_cHostname = hostname;
    m_iPort = port;
    if (!connectSocket()) {
        Logger::getInstance()->log(m_iSocketId, "Can not connect to server.", LOG_LEVEL_FATAL);
        exit(1);
    }
}

TCPClientSocket::~TCPClientSocket() {   
}

bool TCPClientSocket::connectSocket() {
    struct hostent* server;
    struct sockaddr_in addr_server;

    m_iSocketId = socket(AF_INET, SOCK_STREAM, 0); // vytvori socket 

    if (m_iSocketId < 0) {
        Logger::getInstance()->log(m_iSocketId, "while creating socket.", LOG_LEVEL_ERROR);
        return false;
    }

    server = gethostbyname(m_cHostname);
    if (!server) {
        Logger::getInstance()->log(m_iSocketId, "Host does not exist.", LOG_LEVEL_ERROR);
        return false;
    }

    bzero((char *) &addr_server, sizeof (addr_server));
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(m_iPort);
    bcopy((char *) server->h_addr, (char *) &addr_server.sin_addr.s_addr, server->h_length);

    if (connect(m_iSocketId, (struct sockaddr *) &addr_server, sizeof (addr_server)) < 0) {
        Logger::getInstance()->log(m_iSocketId, "While connecting to the server.", LOG_LEVEL_FATAL);
        return false;
    }
    return true;
}

int TCPClientSocket::getSocketId() const {
    return m_iSocketId;
}

void TCPClientSocket::closeSocket() {
    close(m_iSocketId);
}
