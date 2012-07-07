#include "TCPServerSocket.h"

TCPServerSocket::TCPServerSocket(int port) : m_iPort(port) {
    m_bIsOpenned = openSocket();
    if (!m_bIsOpenned) {
        Logger::getInstance()->log(m_iSocketListen, "Socket is not openned -> shutting down", LOG_LEVEL_FATAL);
        exit(1);
    }
}

TCPServerSocket::~TCPServerSocket() {
}

bool TCPServerSocket::openSocket() {
    int opt = 1;

    m_iSocketListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_iSocketListen < 0) {
        Logger::getInstance()->log(m_iSocketListen, "while creating socket.", LOG_LEVEL_ERROR);
        return false;
    }

    memset((void *) &m_listenAddress, 0, sizeof (m_listenAddress));
    m_listenAddress.sin_family = AF_INET;
    m_listenAddress.sin_port = htons(m_iPort);
    m_listenAddress.sin_addr.s_addr = INADDR_ANY;

    setsockopt(m_iSocketListen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (int));

    if (bind(m_iSocketListen, (struct sockaddr*) &m_listenAddress, sizeof (m_listenAddress)) < 0) {
        Logger::getInstance()->log(m_iSocketListen, "While trying to bind, maybe this port is not free.", LOG_LEVEL_ERROR);
        return false;
    }

    if (listen(m_iSocketListen, 5) < 0) {
        Logger::getInstance()->log(m_iSocketListen, "Listen failed.", LOG_LEVEL_ERROR);
        return false;
    }
    return true;
}

int TCPServerSocket::acceptSocket() {
    int socketId = 0;
    int addressSize = sizeof (m_listenAddress);

    socketId = accept(m_iSocketListen, (struct sockaddr*) &m_listenAddress, (socklen_t*) & addressSize);
    if (socketId < 0) {
        Logger::getInstance()->log(m_iSocketListen, "ACCEPT FAILED.", LOG_LEVEL_FATAL);
        exit(1);
    }
    return socketId;
}

bool TCPServerSocket::isOpened() const {
    return m_bIsOpenned;
}

void TCPServerSocket::closeSocket() {
    close(m_iSocketListen);
}

int TCPServerSocket::getSocketId() const {
    return m_iSocketListen;
}