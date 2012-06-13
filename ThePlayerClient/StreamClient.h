/* 
 * File:   StreamClient.h
 * Author: lubos
 *
 * Created on June 11, 2012, 1:13 PM
 */

#ifndef STREAMCLIENT_H
#define	STREAMCLIENT_H

#include "Utils.h"
#include "TCPClientSocket.h"
#include "Logger.h"
#include "Message.h"
#include "Types.h"

#define BUFFSIZE 1024

class StreamClient {
private:
    int m_iId;
    bool m_bConnected;
    TCPClientSocket* m_pClientSocket;

    void setId(int id);

public:
    StreamClient(char* hostname, int port);
    ~StreamClient();

    bool connectToServer();
    bool downloadMedia(int id);

    void sendRawData(char* data, int length);
    char* receiveRawData();
    void sendMessage(Message* message);
    Message* receiveMessage();
    bool saveBinaryFile(std::string path);
    bool queryLibrary(std::string name);
    Song* getSongFromString(std::string songString);

    void shutdown();
    int getId() const;
    bool isConnected() const;
    void setConnected(bool conn);
};

#endif	/* STREAMCLIENT_H */

