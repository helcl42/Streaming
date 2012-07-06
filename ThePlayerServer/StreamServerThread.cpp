#include "StreamServerThread.h"

StreamServerThread::StreamServerThread(int id, int socketId) : m_iId(id), m_iSocketId(socketId) {
    m_library = new Library();

    if (m_library->connectToDB("localhost", "root", "", "thePlayer")) {
        Logger::getInstance()->log(m_iSocketId, "SUCCESSFULLY CONNECTED TO DB.", LOG_LEVEL_INFO);
    } else {
        Logger::getInstance()->log(m_iSocketId, "COULD NOT CONNECT TO DB.", LOG_LEVEL_ERROR);
    }
}

StreamServerThread::~StreamServerThread() {
}

void StreamServerThread::sendRawData(char* buffer, int dataLength) {
    int sendDataLength = 0;

    do {
        sendDataLength += write(m_iSocketId, static_cast<void*> (buffer), dataLength);
        if (sendDataLength < 0) {
            Logger::getInstance()->log(m_iSocketId, "Content of send message has invalid size.", LOG_LEVEL_ERROR);
        }        
    } while (sendDataLength < dataLength);
}

char* StreamServerThread::receiveRawData() {
    char tmpBuffer[BUFFSIZE];
    char tmpByte;
    int partSize = 0;
    int dataLength = 0;

    while (true) {        
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

Message* StreamServerThread::receiveMessage() {
    int partSize = 0;    
    int shift = 0;
    int dataLength = sizeof (Message);

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
                    break;
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
    while (true) {
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
            case MESSAGE_DOWNLOAD_START:
            {
                Logger::getInstance()->log(m_iSocketId, "MESSAGE DOWNLOAD RECEIVED", LOG_LEVEL_INFO);
                Logger::getInstance()->logData(m_iSocketId, message->data, message->dataSize);
                int songId = atoi(message->data);
                if (songId > 0) {
                    Song* song = m_library->getSong(songId);
                    if (song != NULL) {
                        std::string songString = song->toString();
                        int songSize = songString.size();
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
                std::string query = std::string(message->data);
                Message mediaMessage;
                if (m_library->search(query)) {
                    sprintf(mediaMessage.data, "%s", "OK");
                } else {
                    sprintf(mediaMessage.data, "%s", "NO");
                }
                mediaMessage.type = MESSAGE_QUERY_RESULT;
                mediaMessage.dataSize = 2;
                sendMessage(&mediaMessage);

                if (sendQueryResult()) {
                    Logger::getInstance()->log(m_iSocketId, "QUERY RESULT WAS SEND SUCCESSFULLY", LOG_LEVEL_INFO);
                } else {
                    Logger::getInstance()->log(m_iSocketId, "SOMETHING WENT WRONG WHILE SENDING QUERY RESULT", LOG_LEVEL_ERROR);
                }
                break;
            }
            case MESSAGE_DISCONNECT:
                Logger::getInstance()->log(m_iSocketId, "DISCONNECT MESSAGE RECEIVED", LOG_LEVEL_INFO);
                Logger::getInstance()->logData(m_iSocketId, message->data, message->dataSize);                
                if(std::atoi(message->data) >= 0) {                    
                    setRunning(false);
                    Logger::getInstance()->log(m_iSocketId, "INVALIDATED", LOG_LEVEL_INFO);
                    SAFE_DELETE(message);
                    return;
                }                
                break;
            default:
            {
                Logger::getInstance()->log(m_iSocketId, "UNKNOWN MESSAGE RECEIVED", LOG_LEVEL_FATAL);
                break;
            }
        }
        SAFE_DELETE(message);
    }
    setRunning(false);
}

/**
 * sends binary file located by param path
 * returns true, if send was successfull
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
            //printf("PieceId: %d\n", pieceId);
            //usleep(10000);            
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

/**
 * alpha version :)
 * sends query result tree to client
 * @return bool
 */
bool StreamServerThread::sendQueryResult() {
    m_library->printResult();
    Message* message = new Message();
    std::vector<Artist*> artists = m_library->getArtists();
    for (std::vector<Artist*>::iterator artIt = artists.begin(); artIt != artists.end(); ++artIt) {
        Artist* artist = (*artIt);
        std::string artistString = artist->toString();
        message->type = MESSAGE_QUERY_RESULT_ARTIST;
        sprintf(message->data, "%s", artistString.c_str());
        message->dataSize = artistString.size();
        sendMessage(message);
        std::vector<Album*> albums = artist->getAlbums();
        for (std::vector<Album*>::iterator albIt = albums.begin(); albIt != albums.end(); ++albIt) {
            Album* album = (*albIt);
            std::string albumString = album->toString();
            message->type = MESSAGE_QUERY_RESULT_ALBUM;
            sprintf(message->data, "%s", albumString.c_str());
            message->dataSize = albumString.size();
            sendMessage(message);
            std::vector<Song*> songs = album->getSongs();
            for (std::vector<Song*>::iterator songIt = songs.begin(); songIt != songs.end(); ++songIt) {
                Song* song = (*songIt);
                std::string songString = song->toString();
                message->type = MESSAGE_QUERY_RESULT_SONG;
                sprintf(message->data, "%s", songString.c_str());
                message->dataSize = songString.size();
                sendMessage(message);
            }
            message->type = MESSAGE_QUERY_RESULT_SONG_FINISHED;
            message->dataSize = 0;
            sprintf(message->data, "%s", "SONG FINISHED");
            sendMessage(message);
        }
        message->type = MESSAGE_QUERY_RESULT_ALBUM_FINISHED;
        message->dataSize = 0;
        sprintf(message->data, "%s", "ALBUM FINISHED");
        sendMessage(message);
    }
    message->type = MESSAGE_QUERY_RESULT_ARTIST_FINISHED;
    message->dataSize = 0;
    sprintf(message->data, "%s", "ARTIST FINISHED");
    sendMessage(message);
    return true;
}

int StreamServerThread::getId() const {
    return m_iId;
}

int StreamServerThread::getSocketId() const {
    return m_iSocketId;
}


