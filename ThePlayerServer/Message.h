/* 
 * File:    Message.h
 * Project: ThePlayerServer
 * Author:  lubos
 *
 * Created on June 11, 2012, 1:11 PM
 */

#ifndef MESSAGE_H
#define	MESSAGE_H

typedef enum {
    MESSAGE_CONNECT = 0,
    MESSAGE_DISCONNECT,
    MESSAGE_QUERY,
    MESSAGE_QUERY_RESULT,
    MESSAGE_QUERY_RESULT_ARTIST,
    MESSAGE_QUERY_RESULT_ARTIST_FINISHED,
    MESSAGE_QUERY_RESULT_ALBUM,
    MESSAGE_QUERY_RESULT_ALBUM_FINISHED,
    MESSAGE_QUERY_RESULT_SONG,
    MESSAGE_QUERY_RESULT_SONG_FINISHED,
    MESSAGE_WHAT_DOWNLOAD,
    MESSAGE_DOWNLOAD_START,
    MESSAGE_DOWNLOAD,
    MESSAGE_DOWNLOAD_FINISHED,
    MESSAGE_DATA,
    MESSAGE_DATA_FINISHED,
    MESSAGE_DATA_CONFIRM,
    MESSAGE_UNKNOWN
} MessageType;


#define BUFFSIZE 1024

struct Message {
    MessageType type;
    char        data[BUFFSIZE];
    int         dataSize;    

    Message() {}
    ~Message() {}
};

#endif	/* MESSAGE_H */

