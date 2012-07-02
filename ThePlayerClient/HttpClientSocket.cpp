#include "HttpClientSocket.h"


HttpClientSocket::HttpClientSocket(const char* host, int port) : TCPClientSocket(host, port) {
}

HttpClientSocket::HttpClientSocket(std::string host, int port) : TCPClientSocket(host.c_str(), port) {
}

HttpClientSocket::~HttpClientSocket() {
}

bool HttpClientSocket::sendRequest(std::string request) {
    int sendDataLength = 0;
    int shift = 0;
    int dataLength = request.size();
    int sizeToSend = dataLength;
    char data[BUFFSIZE];
    memcpy(data, request.c_str(), request.size());

    do {
        sendDataLength = write(m_iSocketId, (data + shift), sizeToSend);
        if (sendDataLength < 0) {
            Logger::getInstance()->log(m_iSocketId, "COULD NOT SEND REQUEST.", LOG_LEVEL_ERROR);
            return false;
        }
        sizeToSend -= sendDataLength;
        shift += sendDataLength;
    } while (sizeToSend > 0);

    return true;
}

std::string HttpClientSocket::getResponse() {
    int dataLength = RESPONSE_SIZE;
    int partSize = 0;    
    int shift = 0;

    char byteMessage[dataLength];
    char* dataIndex = byteMessage;
    while (shift < RESPONSE_SIZE - 1) {
        char tmpData[dataLength];
        partSize = read(m_iSocketId, static_cast<void*> (tmpData), dataLength);
        memcpy((dataIndex + shift), tmpData, partSize);
        if (partSize < 0) {
            Logger::getInstance()->log(m_iSocketId, "Message could be incomplete.", LOG_LEVEL_ERROR);
            break;
        }
        dataLength -= partSize;
        shift += partSize;
    }
    return std::string(byteMessage);
}

/**
 * method skips header
 * @return Content-Length attribute value
 */
unsigned int HttpClientSocket::getContentSize() {
    int partSize = 0;
    int shift = 0;
    bool found = false;

    char byteMessage[BUFFSIZE];
    char* dataIndex = byteMessage;
    do {
        char tmpData[1];
        partSize = read(m_iSocketId, static_cast<void*> (tmpData), 1);
        memcpy((dataIndex + shift), tmpData, partSize);
        if (partSize < 0) {
            Logger::getInstance()->log(m_iSocketId, "Recieved no HEADER DATA", LOG_LEVEL_ERROR);
            return 0;
        }

        shift += partSize;

        if (shift >= 4) {
            for (int i = shift - 4; i < shift; i++) {
                if (strncmp(byteMessage + i, "\r\n\r\n", 4) == 0) {
                    printf("Found END of Header\n");
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            break;
        }
    } while (shift < BUFFSIZE);

    for (int i = 0; i < BUFFSIZE; i++) {
        if (strncmp(byteMessage + i, "Content-Length: ", 16) == 0) {
            printf("%s\n", byteMessage + i);
            i += 16;
            std::string number;
            while (strncmp(byteMessage + i, "\r\n", 2) != 0) {
                number += *(byteMessage + i);
                i++;
            }

            return std::atoi(number.c_str());
        }
    }

    return 0;
}

bool HttpClientSocket::saveResponseContent(std::string path) {
    //skip header first
    unsigned int contentLength = getContentSize();
    
    std::cout << "CONTENT SIZE: " << contentLength << std::endl;
    if (contentLength > 0) {
        char data[BUFFSIZE];
        int partSize = 0;
        
        std::fstream file(path.c_str(), std::ios::out | std::ios::binary);
        while (contentLength > 0) {
            partSize = read(m_iSocketId, data, BUFFSIZE);
            if (partSize < 0)
                break;
            file.write(data, partSize);
            contentLength -= partSize;
        }
        Logger::getInstance()->log(m_iSocketId, "DOWNLOAD FINISHED", LOG_LEVEL_INFO);
        return true;
    } else {
        Logger::getInstance()->log(m_iSocketId, "Content-Length is <= 0", LOG_LEVEL_ERROR);
        return false;
    }
}

std::string HttpClientSocket::getHostFromUrl(std::string url) {
    std::string host;
    for (unsigned int i = 0, count = 0; i < url.size(); i++) {
        if (url[i] == '/') {
            count++;
        }
        if (count >= 3) {
            host = url.substr(7, i - 7);
            //std::cout << "NEW HOST: " << host << std::endl;
            break;
        }
    }
    return host;
}

std::string HttpClientSocket::getServerPathFromUrl(std::string url) {
    std::string serverPath;    
    for (unsigned int i = 0, count = 0; i < url.size(); i++) {
        if (url[i] == '/') {
            count++;
        }
        if (count >= 3) {
            serverPath = url.substr(i, url.size() - 1);
            //std::cout << "NEW SERVER PATH: " << serverPath << std::endl;
            break;
        }
    }
    return serverPath;
}

