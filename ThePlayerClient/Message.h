/* 
 * File:   Message.h
 * Author: lubos
 *
 * Created on June 11, 2012, 1:11 PM
 */

#ifndef MESSAGE_H
#define	MESSAGE_H

typedef enum {
    MESSAGE_CONNECT = 0,
    MESSAGE_QUERY,
    MESSAGE_QUERY_RESULT,
    MESSAGE_WHAT_DOWNLOAD,
    MESSAGE_DOWNLOAD,
    MESSAGE_DOWNLOAD_FINISHED,
    MESSAGE_DATA,
    MESSAGE_DATA_FINISHED,
    MESSAGE_DATA_CONFIRM,
    MESSAGE_UNKNOWN
} MessageType;


//-> poslu message z klienta na server INFO CONNECT
//<- server posle klientovi id, ktery bodou pouzivat celou session INFO CONNECT
//-> klient budto posle dotaz s textem nebo prazdny MESSAGE_QUERY DATA
//<- server posle XML obraz adresarovy struktury, ktera se musi natahnout do library MESSAGE_QUERY DATA
//-> ve chvili kdy klient klikne na nejako skladbu v library, posle zpravu MESSAGE_DOWNLOAD DATA s id songu
//<- server da dotaz na DB, najde song podle id a zacina streamovani bude se posilat asi po 1024B MESSAGE_DOWNLOAD DATA
//-> klient posle info message INFO MESSAGE_DOWNLOAD
//	...
//<- server posle download finished INFO DOWNLOAD_FINISHED
//-> po uzavreni library se klient odpoji a na servru se dealokuje

#define BUFFSIZE 1024

struct Message {
    MessageType type;
    char data[BUFFSIZE];
    int dataSize;
    int blockId;

    Message() {
    }

    ~Message() {
    }
};


#endif	/* MESSAGE_H */

