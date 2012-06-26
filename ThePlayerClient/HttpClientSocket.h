/* 
 * File:   HttpClientSocket.h
 * Author: lubos
 *
 * Created on June 23, 2012, 3:27 PM
 */

#ifndef HTTPCLIENTSOCKET_H
#define	HTTPCLIENTSOCKET_H

#include "TCPClientSocket.h"
#include "StreamClient.h"

#define RESPONSE_SIZE 2048

class HttpClientSocket : public TCPClientSocket {
private:
    unsigned int getContentSize();

public:
    HttpClientSocket(std::string host, int port);
    HttpClientSocket(const char* host, int port);
    ~HttpClientSocket();

    static std::string getHostFromUrl(std::string url);
    static std::string getServerPathFromUrl(std::string url);

    bool sendRequest(std::string request);
    std::string getResponse();

    bool saveResponseContent(std::string path);
};

#endif	/* HTTPCLIENTSOCKET_H */

