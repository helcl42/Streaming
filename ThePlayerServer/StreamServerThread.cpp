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

Message* StreamServerThread::receiveMessage() {
    int partSize = 0;
    int dataLength = sizeof (Message);

    Message* message = new Message();

    partSize = read(m_iSocketId, static_cast<Message*> (message), dataLength);
    if (partSize != dataLength) {
        Logger::getInstance()->log(m_iSocketId, "Message could be incomplete.", LOG_LEVEL_ERROR);
    }
    return message;
}

void StreamServerThread::sendMessage(Message* message) {
    int sendDataLength = 0;
    int dataLength = sizeof (Message);

    do {
        sendDataLength = write(m_iSocketId, static_cast<void*> (message), sizeof (Message));
        if (sendDataLength < 0) {
            Logger::getInstance()->log(m_iSocketId, "Content of send message has invalid size.", LOG_LEVEL_ERROR);
        }
        sendDataLength += sendDataLength;
    } while (sendDataLength < dataLength);
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

                //query do db, ktera mi vrati celej objekt Song
                Song* song = new Song(songId, "NAME", 200, "/home/lubos/Documents/Projects/karel/", NULL);
                int songSize = sizeof (Song);

                Message mediaMessage;
                mediaMessage.type = MESSAGE_DOWNLOAD;
                // mediaMessage.dataSize = songSize;
                //memcpy(mediaMessage.data, static_cast<void*> (song), songSize);
                sendMessage(&mediaMessage);
                //sendBinaryFile("Led_Zeppelin_04_No_Quarter.flac");
                sendBinaryFile("Mana_Nothing_particular.ogg");
                //sendBinaryFile("09-koop-drum_rhythm_a.mp3");
                //sendBinaryFile("21.-Koop---Whenever-There-Is-You.mp3");
                break;
            }
            default:
                Logger::getInstance()->log(m_iSocketId, "UNKNOWN MESSAGE RECEIVED", LOG_LEVEL_FATAL);
                break;
        }
        cnt++;
        delete message;
    }
}

bool StreamServerThread::sendBinaryFile(std::string path) {
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    Message dataMessage;
    if (file.is_open()) {
        int pieceId = 0;
        while (!file.eof()) {
            dataMessage.dataSize = BUFFSIZE;
            dataMessage.type = MESSAGE_DATA;
            file.read(dataMessage.data, BUFFSIZE);
            sendMessage(&dataMessage);
            printf("PieceId: %d\n", pieceId);
            //printf("%s\n", dataMessage.data); 
            usleep(100000);
            pieceId++;
        }
        file.close();
    } else {
        std::cout << "Unable to open file";
        return false;
    }
    dataMessage.type = MESSAGE_DATA_FINISHED;
    sprintf(dataMessage.data, "%s", "0");
    dataMessage.dataSize = 0;
    sendMessage(&dataMessage);
    std::cout << "FINISHED" << std::endl;
    return true;
}





