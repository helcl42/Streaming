#include "StreamMediaClient.h"
#include "Library.h"

StreamMediaClient::StreamMediaClient(std::string host, int port) : StreamClient(host, port), m_songToDownload(NULL) {
}

StreamMediaClient::StreamMediaClient(const char* host, int port) : StreamClient(host, port), m_songToDownload(NULL) {
}

StreamMediaClient::~StreamMediaClient() {
}

/**
 * @param id
 * @return bool
 * function to download media from server
 * returns true if download was successfull
 */
bool StreamMediaClient::downloadMedia(Song* sng) {
    Message* message = new Message();
    message->type = MESSAGE_DOWNLOAD_START;
    sprintf(message->data, "%d", sng->getId());
    message->dataSize = 0;
    sendMessage(message);
    SAFE_DELETE(message);
    message = receiveMessage();
    int socketId = m_pClientSocket->getSocketId();
    if (message->type == MESSAGE_DOWNLOAD) {
        Logger::getInstance()->log(socketId, "DOWNLOAD MESSAGE RECIEVED", LOG_LEVEL_INFO);
        Logger::getInstance()->logData(socketId, message->data, message->dataSize);        
        Song* song = Song::getSongFromString(std::string(message->data));
        song->setFromLibrary(true);
        song->setAudio(Song::findOutIfAudioMedia(song));
        changeSavePath(song);        
        MediaApp::getInstance()->getPlaylistInstance()->insert(song);        
        saveBinaryFile(song->getUrl());
        Library::getInstance()->downloadCallback(true);
        SAFE_DELETE(message);        
        return true;
    } else {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "UNKNOWN MESSAGE RECEIVED in download media.", LOG_LEVEL_FATAL);
        Library::getInstance()->downloadCallback(false);
        SAFE_DELETE(message);
        return false;
    }
}

void StreamMediaClient::createFolderStructure(std::string newPath) {
    std::string tmp;
    int status = 0;
    for (unsigned int i = 0; i < newPath.size(); i++) {
        if (newPath[i] == '/') {
            tmp = newPath.substr(0, i);
            std::cout << "NEW FOLDER: " << tmp << std::endl;
            status = mkdir(tmp.c_str(), 0777);
        }
    }
    if (status == -1) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "Could not Create directory, maybe already exists.", LOG_LEVEL_ERROR);
    }
}

void StreamMediaClient::changeSavePath(Song* song) {
    std::string oldPath = song->getUrl();
    std::string newPath;
    for (int i = (int) oldPath.size() - 1, count = 0; i > 0; i--) {
        if (oldPath[i] == '/') {
            count++;
        }
        if (count >= 3) {
            newPath = oldPath.substr(i + 1, oldPath.size());
            break;
        }
    }
    createFolderStructure(newPath);
    newPath = "/home/lubos/NetBeansProjects/ThePlayerClient/dist/Debug/GNU-Linux-x86/" + newPath;
    //newPath = "./" + newPath;
    std::cout << "NEW PATH: " << newPath << std::endl;
    song->setUrl(newPath);
}

/** 
 * @param string path
 * @return bool
 * returns true, if save of file was correct
 * with accepting MESSAEGE_DATA_FINISH message
 */
bool StreamMediaClient::saveBinaryFile(std::string path) {
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
        if (recvMessage->type != MESSAGE_DATA) {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "INVALID MESSAGE TYPE OR SIZE -> WHILE DOWNLOADING MEDIA.", LOG_LEVEL_ERROR);
            return false;
        }

        file.write(recvMessage->data, BUFFSIZE);
        //printf("PieceId: %d\n", pieceId);
        SAFE_DELETE(recvMessage);
    }
    Logger::getInstance()->log(m_pClientSocket->getSocketId(), "DOWNLOAD WAS NOT FINISHED CORRECTLY", LOG_LEVEL_ERROR);
    return false;
}

/**
 * @param name
 * @return true if query message sequence is correct
 * and player updates library list
 */
bool StreamMediaClient::queryLibrary() {    
    Message* message = new Message();
    message->type = MESSAGE_QUERY;
    sprintf(message->data, "%s", m_queryString.c_str());
    message->dataSize = strlen(m_queryString.c_str());
    sendMessage(message);
    SAFE_DELETE(message);

    message = receiveMessage();
    if (message->type == MESSAGE_QUERY_RESULT) {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "QUERT RESULT MESSAGE RECIEVED", LOG_LEVEL_INFO);
        Logger::getInstance()->logData(m_pClientSocket->getSocketId(), message->data, message->dataSize);
        if (strncmp(message->data, "OK", 2) == 0) {
            receiveQueryResult();
        } else {
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "NO SUITABLE DATA.", LOG_LEVEL_INFO);
        }
        Library::getInstance()->searchCallback(true);
        SAFE_DELETE(message);
        return true;
    } else {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "UNKNOWN MESSAGE RECEIVED WHILE QUERING DB.", LOG_LEVEL_FATAL);
        Library::getInstance()->searchCallback(false);
        SAFE_DELETE(message);
        return false;
    }
}

bool StreamMediaClient::receiveQueryResult() {
    Message* message = receiveMessage();
    if (message->type == MESSAGE_QUERY_RESULT_ARTIST) {
        while (true) {
            std::cout << message->data << std::endl;
            Artist* artist = Artist::getAlbumFromString(std::string(message->data));
            if (message->type == MESSAGE_QUERY_RESULT_ARTIST_FINISHED)
                break;
            Library::getInstance()->m_artists.push_back(artist);
            while (true) {
                message = receiveMessage();
                std::cout << message->data << std::endl;
                Album* album = Album::getAlbumFromString(std::string(message->data));
                if (message->type == MESSAGE_QUERY_RESULT_ALBUM_FINISHED)
                    break;
                artist->addAlbum(album);
                while (true) {
                    message = receiveMessage();
                    std::cout << message->data << std::endl;
                    Song* song = Song::getSongFromString(std::string(message->data));
                    if (message->type == MESSAGE_QUERY_RESULT_SONG_FINISHED)
                        break;
                    album->addSong(song);
                }
            }
            message = receiveMessage();
            std::cout << message->data << std::endl;
        }
    } else {
        Logger::getInstance()->log(m_pClientSocket->getSocketId(), "QUERY RESULT ARTIST EXPECTED HERE", LOG_LEVEL_ERROR);
        return false;
    }
    return true;
}

void StreamMediaClient::ThreadProcedure() {
    setRunning(true);
    switch (m_operation) {
        case OP_CONNECT:
            if (connectToServer()) {
                m_bConnected = true;                
            } else {
                m_bConnected = false;
            }
            break;
        case OP_QUERY:
            if (queryLibrary()) {
                Logger::getInstance()->log(m_pClientSocket->getSocketId(), "QUERY SUCCESSFULL.", LOG_LEVEL_INFO);
            } else {
                Logger::getInstance()->log(m_pClientSocket->getSocketId(), "SOMETHING WENT WRONG WHILE QUERYING", LOG_LEVEL_ERROR);
            }
            break;
        case OP_DOWNLOAD:
            if (downloadMedia(m_songToDownload)) {
                Logger::getInstance()->log(m_pClientSocket->getSocketId(), "DOWNLOAD SUCCESSFULL.", LOG_LEVEL_INFO);
            } else {
                Logger::getInstance()->log(m_pClientSocket->getSocketId(), "SOMETHING WENT WRONG WHILE DOWNLOADING", LOG_LEVEL_ERROR);
            }
            break;
        case OP_DISCONNECT:
            if(disconnect()) {
                Logger::getInstance()->log(m_pClientSocket->getSocketId(), "DISCONNECT SUCCESSFUL.", LOG_LEVEL_INFO);
            } else {
                Logger::getInstance()->log(m_pClientSocket->getSocketId(), "SOMETHING WENT WRONG WHILE DISCONNECTING.", LOG_LEVEL_INFO);
            }
            break;
        default:
            Logger::getInstance()->log(m_pClientSocket->getSocketId(), "UNKNOWN OPERATION", LOG_LEVEL_ERROR);
            break;
    }   
    setRunning(false);
}

void StreamMediaClient::setDownloadSong(Song* song) {
    m_songToDownload = song;
}

Song* StreamMediaClient::getDownloadSong() const {
    return m_songToDownload;
}

void StreamMediaClient::setOperation(Operation op) {
    m_operation = op;
}

Operation StreamMediaClient::getOperation() const {
    return m_operation;
}

void StreamMediaClient::setQueryString(std::string str) {
    m_queryString = str;
}

std::string StreamMediaClient::getQueryString() const {
    return m_queryString;
}
