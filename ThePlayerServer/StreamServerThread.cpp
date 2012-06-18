#include "StreamServerThread.h"

StreamServerThread::StreamServerThread(int id, int socketId) {
    m_iId = id;
    m_iSocketId = socketId;
}

StreamServerThread::~StreamServerThread() {
}

int StreamServerThread::getId() const {
    return m_iId;
}

int StreamServerThread::getSocketId() const {
    return m_iSocketId;
}

void StreamServerThread::sendRawData(char* buffer, int dataLength) {
    int sendDataLength = 0;

    do {
        sendDataLength = write(m_iSocketId, static_cast<void*> (buffer), dataLength);
        if (sendDataLength < 0) {
            Logger::getInstance()->log(m_iSocketId, "Content of send message has invalid size.", LOG_LEVEL_ERROR);
        }
        sendDataLength += sendDataLength;
    } while (sendDataLength < dataLength);
}

char* StreamServerThread::receiveRawData() {
    char tmpBuffer[BUFFSIZE];
    char tmpByte;
    int partSize = 0;
    int dataLength = 0;

    while (true) {
        tmpByte = 0;
        partSize = read(m_iSocketId, static_cast<char*> (&tmpByte), 1);

        if (partSize < 0) {
            Logger::getInstance()->log(m_iSocketId, "Response is empty.", LOG_LEVEL_ERROR);
        }
        if (tmpByte == '\n') {
            break;
        }

        dataLength += partSize;

        if (dataLength <= BUFFSIZE) {
            tmpBuffer[dataLength - 1] = tmpByte;
        } else {
            Logger::getInstance()->log(m_iSocketId, "Response is too large.", LOG_LEVEL_ERROR);
            return NULL;
        }
    }

    char* message = new char[dataLength + 1];
    for (int i = 0; i < dataLength; i++)
        message[i] = tmpBuffer[i];
    message[dataLength] = '\0';

    return message;
}

/*
Message* StreamServerThread::receiveMessage() {
    int partSize = 0;
    int dataLength = sizeof (Message);

    Message* message = new Message();
    do {
        partSize = read(m_iSocketId, static_cast<Message*> (message), dataLength);
        if (partSize < dataLength) {
            Logger::getInstance()->log(m_iSocketId, "Message could be incomplete.", LOG_LEVEL_ERROR);
        }
    } while (partSize < dataLength);
    return message;
}
*/

Message* StreamServerThread::receiveMessage() {
    int partSize = 0;
    int dataLength = sizeof (Message);
    int shift = 0;

    char byteMessage[dataLength];
    char* dataIndex = byteMessage;
    do {
        char tmpData[dataLength];
        partSize = read(m_iSocketId, static_cast<void*> (tmpData), dataLength);
        memcpy((dataIndex + shift), tmpData, partSize);
        if (partSize < 0) {
            Logger::getInstance()->log(m_iSocketId, "Message could be incomplete.", LOG_LEVEL_ERROR);
        }
        dataLength -= partSize;
        shift += partSize;        
    } while (dataLength > 0);

    Message* message = new Message();
    memcpy(message, byteMessage, sizeof (Message));
    printf("type: %d\n", message->type);
    printf("DataSize: %d\n", message->dataSize);
    return message;
}


void StreamServerThread::sendMessage(Message* message) {
    int sendDataLength = 0;
    int shift = 0;
    int dataLength = sizeof (Message);
    int sizeToSend = dataLength;

    if (message != NULL) {
        void* tmp = static_cast<void*> (message);
        char* data = static_cast<char*> (tmp);
        if (data != NULL) {
            do {
                sendDataLength = write(m_iSocketId, (data + shift), sizeToSend);
                if (sendDataLength < 0) {
                    Logger::getInstance()->log(m_iSocketId, "Content of send message has invalid size.", LOG_LEVEL_ERROR);
                }
                sizeToSend -= sendDataLength;
                shift += sendDataLength;
            } while (sizeToSend > 0);
        }
    } else {
        Logger::getInstance()->log(m_iSocketId, "Message pointer is NULL.", LOG_LEVEL_ERROR);
    }
}


void StreamServerThread::ThreadProcedure() {
    int cnt = 0;
    while (cnt < 2) {
        Message* message = receiveMessage();

        switch (message->type) {
            case MESSAGE_CONNECT:
            {
                Logger::getInstance()->log(m_iSocketId, "CONNECT MESSAGE RECEIVED", LOG_LEVEL_INFO);
                Logger::getInstance()->logData(m_iSocketId, message->data, message->dataSize);
                Message initMessage;
                initMessage.type = MESSAGE_CONNECT;
                sprintf(initMessage.data, "%d", m_iId);
                sendMessage(&initMessage);
                break;
            }
            case MESSAGE_DOWNLOAD:
            {
                Logger::getInstance()->log(m_iSocketId, "MESSAGE DOWNLOAD RECEIVED", LOG_LEVEL_INFO);
                Logger::getInstance()->logData(m_iSocketId, message->data, message->dataSize);
                int songId = atoi(message->data);
                if (songId > 0) {
                    Song* song = getSong(songId);
                    if (song != NULL) {
                        std::string songString = song->getAsString();
                        int songSize = strlen(songString.c_str());
                        Message mediaMessage;
                        memcpy(mediaMessage.data, songString.c_str(), songSize);
                        mediaMessage.dataSize = songSize;
                        mediaMessage.type = MESSAGE_DOWNLOAD;
                        sendMessage(&mediaMessage);
                        sendBinaryFile(song->getUrl());
                        SAFE_DELETE(song);
                    } else {
                        Logger::getInstance()->log(m_iSocketId, "NON-INITIALIZED SONG OBJECT", LOG_LEVEL_INFO);
                    }
                } else {
                    Logger::getInstance()->log(m_iSocketId, "INVALID SONG ID", LOG_LEVEL_ERROR);
                }
                break;
            }
            case MESSAGE_QUERY:
            {
                Logger::getInstance()->log(m_iSocketId, "MESSAGE QUERY RECEIVED", LOG_LEVEL_INFO);
                Logger::getInstance()->logData(m_iSocketId, message->data, message->dataSize);
                //call library to query message->data identifier
                break;
            }
            default:
                Logger::getInstance()->log(m_iSocketId, "UNKNOWN MESSAGE RECEIVED", LOG_LEVEL_FATAL);
                break;
        }
        SAFE_DELETE(message);
        cnt++;
        delete message;
    }
    setRunning(false);
}

Song* StreamServerThread::getSong(int songId) {
    //query library here
    Song* song = new Song(songId, "Led_Zeppelin_04_No_Quarter.flac", 200, "Led_Zeppelin_04_No_Quarter.flac", NULL);
    return song;
}

/**
 * function , whitch sends binary file, by path
 * returns true, if send is successfull
 * @param path
 * @return bool
 */
bool StreamServerThread::sendBinaryFile(std::string path) {
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    Message dataMessage;
    if (file.is_open()) {
        dataMessage.dataSize = BUFFSIZE;
        dataMessage.type = MESSAGE_DATA;
        for (int pieceId = 0; !file.eof(); ++pieceId) {
            file.read(dataMessage.data, BUFFSIZE);
            sendMessage(&dataMessage);
            printf("PieceId: %d\n", pieceId);
            //printf("%s\n", dataMessage.data);          
        }
        file.close();
    } else {
        Logger::getInstance()->log(m_iSocketId, "UNABLE TO OPEN FILE", LOG_LEVEL_FATAL);
        return false;
    }
    dataMessage.type = MESSAGE_DATA_FINISHED;
    sprintf(dataMessage.data, "%s", "0");
    dataMessage.dataSize = 0;
    sendMessage(&dataMessage);
    Logger::getInstance()->log(m_iSocketId, "SEND MEDIA FINISHED", LOG_LEVEL_INFO);
    return true;
}





