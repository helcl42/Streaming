/* 
 * File:   StreamMediaClient.h
 * Author: lubos
 *
 * Created on June 23, 2012, 2:31 PM
 */

#ifndef STREAMMEDIACLIENT_H
#define	STREAMMEDIACLIENT_H

#include "StreamClient.h"

typedef enum {
    OP_CONNECT = 0,
    OP_QUERY,
    OP_DOWNLOAD,
    OP_DISCONNECT
} Operation;

class StreamMediaClient : public StreamClient {
private:
    Song* m_songToDownload;
    Operation m_operation;
    std::string m_queryString;

protected:
    void ThreadProcedure();

public:
    StreamMediaClient(std::string host, int port);
    StreamMediaClient(const char* host, int port);
    ~StreamMediaClient();

    bool downloadMedia(Song* song);
    bool saveBinaryFile(std::string path);
    void changeSavePath(Song* song);
    void createFolderStructure(std::string path);

    bool queryLibrary();
    bool receiveQueryResult();

    void setDownloadSong(Song* song);
    Song* getDownloadSong() const;

    void setOperation(Operation op);
    Operation getOperation() const;

    void setQueryString(std::string str);
    std::string getQueryString() const;
};

#endif	/* STREAMMEDIACLIENT_H */

