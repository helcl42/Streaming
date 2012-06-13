#include "StreamClient.h"

/**
 * 
 * @param hostname
 * @param port
 * Constructor
 */
StreamClient::StreamClient(char* hostname, int port) {
    m_pClientSocket = new TCPClientSocket(hostname, port);
    if (!m_pClientSocket) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Socket was not created", LOG_LEVEL_FATAL);
        exit(1);
    }
}

/**
 * Destructor
 */
StreamClient::~StreamClient() {
    SAFE_DELETE(m_pClientSocket);
};

/**
 * 
 * @param buffer
 * @param dataLength
 * function sends raw char* string of senconds parameters length
 */
void StreamClient::sendRawData(char* buffer, int dataLength) {
    int sendDataLength = 0;

    do {
        sendDataLength = write(m_pClientSocket->getSocketId(), static_cast<void*> (buffer), dataLength);
        if (sendDataLength < 0) {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Content of send message has invalid size.", LOG_LEVEL_ERROR);
        }
        sendDataLength += sendDataLength;
    } while (sendDataLength < dataLength);
}

/**
 * @return char*
 * function return raw char* string
 * received from server
 */
char* StreamClient::receiveRawData() {
    char tmpBuffer[BUFFSIZE];
    char tmpByte;
    int partSize;
    int dataLength = 0;

    while (true) {
        tmpByte = 0;
        partSize = read(m_pClientSocket->getSocketId(), static_cast<char*> (&tmpByte), 1);

        if (partSize < 0) {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Response is empty.", LOG_LEVEL_ERROR);
        }
        if (tmpByte == '\n') {
            break;
        }

        dataLength += partSize;

        if (dataLength <= BUFFSIZE) {
            tmpBuffer[dataLength - 1] = tmpByte;
        } else {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Response is to large.", LOG_LEVEL_ERROR);
            return NULL;
        }
    }

    char* message = new char[dataLength];
    for (int i = 0; i < dataLength; i++) {
        message[i] = tmpBuffer[i];
    }
    return message;
}

/**
 * 
 * @param message
 * function sends Message object to server
 */
void StreamClient::sendMessage(Message* message) {
    int sendDataLength = 0;
    int dataLength = sizeof (Message);
    void* data = static_cast<void*> (message);
    if (data != NULL) {
        do {
            sendDataLength = write(m_pClientSocket->getSocketId(), data, dataLength);
            if (sendDataLength < 0) {
                Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Content of send message has invalid size.", LOG_LEVEL_ERROR);
            }
            sendDataLength += sendDataLength;
        } while (sendDataLength < dataLength);
    }
}

/** 
 * @return Message*
 * function returns recieved messsage 
 * object from server
 */
Message* StreamClient::receiveMessage() {
    int partSize = 0;
    int dataLength = sizeof (Message);
    Message* message = new Message();
    partSize = read(m_pClientSocket->getSocketId(), static_cast<Message*> (message), dataLength);
    if (partSize != dataLength) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Message could be incomplete.", LOG_LEVEL_ERROR);
    }
    return message;
}

/** 
 * @return bool
 * function returns true if connection to
 * server was successfull
 */
bool StreamClient::connectToServer() {
    Message* message = new Message();
    message->type = MESSAGE_CONNECT;
    message->data[0] = '\0';
    message->dataSize = 0;
    sendMessage(message);
    SAFE_DELETE(message);

    message = receiveMessage();
    if (message->type == MESSAGE_CONNECT) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "CONNECT MESSAGE RECIEVED", LOG_LEVEL_INFO);
        Logger::getInstance()->logData(m_pClientSocket->getSocketId(), message->data, message->dataSize);
        setId(atoi(message->data));
        setConnected(true);
        SAFE_DELETE(message);
        return true;
    } else {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "UNKNOWN MESSAGE RECEIVED in connect to server.", LOG_LEVEL_FATAL);
        SAFE_DELETE(message);
        return false;
    }
}

/**
 * @param id
 * @return bool
 * function to download media from server
 * returns true if download was successfull
 */
bool StreamClient::downloadMedia(int id) {
    Message* message = new Message();
    message->type = MESSAGE_DOWNLOAD;
    sprintf(message->data, "%d", id);
    message->dataSize = 0;
    sendMessage(message);
    SAFE_DELETE(message);

    message = receiveMessage();
    if (message->type == MESSAGE_DOWNLOAD) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "DOWNLOAD MESSAGE RECIEVED", LOG_LEVEL_INFO);
        Logger::getInstance()->logData(m_pClientSocket->getSocketId(), message->data, message->dataSize);
        Song* song = getSongFromString(std::string(message->data));
        saveBinaryFile(song->getTitle());
        SAFE_DELETE(message);
        return true;
    } else {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "UNKNOWN MESSAGE RECEIVED in download media.", LOG_LEVEL_FATAL);
        SAFE_DELETE(message);
        return false;
    }
}

/** 
 * @param songString
 * @return Song*
 * function returns Song object pointer from object string
 */
Song* StreamClient::getSongFromString(std::string songString) {
    Song* song = new Song();
    std::vector<std::string> strs = Util::split(songString, ':');
    song->setId(atoi(strs[0].c_str()));
    song->setTitle(strs[1]);
    song->setLength(atoi(strs[2].c_str()));
    song->setUrl(strs[3]);
    return song;
}

/** 
 * @param string path
 * @return bool
 * returns true, if save of file was correct
 * with accepting MESSAEGE_DATA_FINISH message
 */
bool StreamClient::saveBinaryFile(std::string path) {
    std::fstream file(path.c_str(), std::ios::out | std::ios::binary);
    Message* recvMessage = NULL;
    for (int pieceId = 0;; ++pieceId) {
        recvMessage = receiveMessage();
        if (recvMessage->type == MESSAGE_DATA_FINISHED) {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "DOWNLOAD FINISHED SUCCESFULLY", LOG_LEVEL_INFO);
            SAFE_DELETE(recvMessage);
            file.close();
            return true;
        }
        if (recvMessage->type != MESSAGE_DATA || recvMessage->dataSize < 0) {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "INVALID MESSAGE TYPE OR SIZE -> WHILE DOWNLOADING MEDIA.", LOG_LEVEL_ERROR);
            return false;
        }
        file.write(recvMessage->data, BUFFSIZE);
        printf("PieceId: %d\n", pieceId);
        //printf("%s\n", recvMessage->data);
        SAFE_DELETE(recvMessage);
    }
    Logger::getInstance()->log(m_pClientSocket->getSocketId(), "DOWNLOAD WAS NOT FINISHED CORRECTLY", LOG_LEVEL_ERROR);
    return false;
}

/**
 * @param name
 * @return true if query message sequence is correct
 * and player apdates while library list
 */
bool StreamClient::queryLibrary(std::string name) {
    Message* message = new Message();
    message->type = MESSAGE_QUERY;
    sprintf(message->data, "%s", name.c_str());
    message->dataSize = strlen(name.c_str());
    sendMessage(message);
    SAFE_DELETE(message);
    
    message = receiveMessage();   
    if (message->type == MESSAGE_QUERY_RESULT) {        
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "QUERT RESULT MESSAGE RECIEVED", LOG_LEVEL_INFO);
        Logger::getInstance()->logData(m_pClientSocket->getSocketId(), message->data, message->dataSize);
        //Song* song = static_cast<Song*> (static_cast<void*> (message.data));                                 
        SAFE_DELETE(message);
        return true;
    } else {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "UNKNOWN MESSAGE RECEIVED in download media.", LOG_LEVEL_FATAL);
        SAFE_DELETE(message);
        return false;
    }
}

void StreamClient::setId(int id) {
    m_iId = id;
}

void StreamClient::shutdown() {
    m_pClientSocket->closeSocket();
}

int StreamClient::getId() const {
    return m_iId;
}

bool StreamClient::isConnected() const {
    return m_bConnected;
}

void StreamClient::setConnected(bool conn) {
    m_bConnected = conn;
}
