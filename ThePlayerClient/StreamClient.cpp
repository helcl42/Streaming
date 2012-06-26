#include "StreamClient.h"
#include "Library.h"

/**
 * 
 * @param hostname
 * @param port
 * Constructor
 */
StreamClient::StreamClient(const char* hostname, int port) {
    m_pClientSocket = new TCPClientSocket(hostname, port);
    if (!m_pClientSocket) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Socket was not created", LOG_LEVEL_FATAL);
    }
}

StreamClient::StreamClient(std::string hostname, int port) {
    m_pClientSocket = new TCPClientSocket(hostname.c_str(), port);
    if (!m_pClientSocket) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Socket was not created", LOG_LEVEL_FATAL);
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
        sendDataLength += write(m_pClientSocket->getSocketId(), static_cast<void*> (buffer), dataLength);
        if (sendDataLength < 0) {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Content of send message has invalid size.", LOG_LEVEL_ERROR);
        }
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
bool StreamClient::sendMessage(Message* message) {
    int sendDataLength = 0;
    int shift = 0;
    int dataLength = sizeof (Message);
    int sizeToSend = dataLength;

    if (message != NULL) {
        void* tmp = static_cast<void*> (message);
        char* data = static_cast<char*> (tmp);
        if (data != NULL) {
            do {
                sendDataLength = write(m_pClientSocket->getSocketId(), (data + shift), sizeToSend);
                if (sendDataLength < 0) {
                    Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Content of send message has invalid size.", LOG_LEVEL_ERROR);
                    return false;
                }
                sizeToSend -= sendDataLength;
                shift += sendDataLength;
            } while (sizeToSend > 0);
        }
    } else {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Message pointer is NULL.", LOG_LEVEL_ERROR);
        return false;
    }
    return true;
}

/**
 * receive message
 * @return poiter to received Message
 */
Message* StreamClient::receiveMessage() {
    int partSize = 0;
    int dataLength = sizeof (Message);
    int shift = 0;

    char byteMessage[dataLength];
    char* dataIndex = byteMessage;
    do {
        char tmpData[dataLength];
        partSize = read(m_pClientSocket->getSocketId(), static_cast<void*> (tmpData), dataLength);
        memcpy((dataIndex + shift), tmpData, partSize);
        if (partSize < 0) {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Message could be incomplete.", LOG_LEVEL_ERROR);
        }
        dataLength -= partSize;
        shift += partSize;        
    } while (dataLength > 0);

    Message* message = new Message();
    memcpy(message, byteMessage, sizeof (Message));
    
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
    sprintf(message->data, "%s", "connect me!");
    message->dataSize = 0;
    if (!sendMessage(message)) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "CONNECTION NOT ESTABLISHED", LOG_LEVEL_INFO);
        SAFE_DELETE(message);
        return false;
    }

    message = receiveMessage();
    if (message->type == MESSAGE_CONNECT) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "CONNECT MESSAGE RECIEVED", LOG_LEVEL_INFO);
        Logger::getInstance()->logData(m_pClientSocket->getSocketId(), message->data, message->dataSize);
        setId(atoi(message->data));
        setConnected(true);
        MediaApp::getInstance()->openLibraryCallback(true);
        SAFE_DELETE(message);
        return true;
    } else {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "UNKNOWN MESSAGE RECEIVED in connect to server.", LOG_LEVEL_FATAL);
        MediaApp::getInstance()->openLibraryCallback(false);
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
