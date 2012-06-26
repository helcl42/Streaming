#include <bits/stringfwd.h>

#include "CoverDownloader.h"
#include "StreamClient.h"
#include "HttpClientSocket.h"
#include "MediaApp.h"

CoverDownloader::CoverDownloader() : Thread() {
}

CoverDownloader::CoverDownloader(std::string artist, std::string albumName) : Thread() {
    m_albumName = albumName;
    m_artist = artist;
}

CoverDownloader::~CoverDownloader() {
}

void CoverDownloader::ThreadProcedure() {
    //std::string name = m_artist + "_" + m_albumName;
    m_path = download();        
    //MediaApp::getInstance()->getAlbumWidgetInstance()->fromInternetCallback(name, path);
}

std::string CoverDownloader::escapeStrings(std::string in) {
    std::string tmp;

    for (unsigned int i = 0; i < in.size(); i++) {
        if (in[i] == ' ') {
            tmp = in.substr(0, i);
            tmp += "%20";
            tmp += in.substr(i + 1, in.size() - 1);
            in = tmp;
        }

    }
    std::cout << "IN: " << in << std::endl;
    return in;
}

std::string CoverDownloader::download() {
    HttpClientSocket httpSocket("ws.audioscrobbler.com", 80);

    if (m_artist.empty() || m_albumName.empty()) {
        Logger::getInstance()->log(httpSocket.getSocketId(), "BOTH ARTIST AND ALBUM MUST BE SET", LOG_LEVEL_ERROR);
        return "";
    }

    std::string albumTitle = CoverDownloader::escapeStrings(m_albumName);
    std::string artistName = CoverDownloader::escapeStrings(m_artist);

    std::string req = "GET /2.0/?method=album.getinfo&api_key=";
    req += API_KEY;
    req += "&artist=";
    req += albumTitle;
    req += "&album=";
    req += artistName;
    req += " HTTP/1.1\r\nHost: ws.audioscrobbler.com\r\n\r\n";

    std::cout << "REQUEST: " << req << std::endl;
    
    if (!httpSocket.sendRequest(req)) {
        Logger::getInstance()->log(httpSocket.getSocketId(), "REQUEST WAS NOT SUCCESSFULLY SENT.", LOG_LEVEL_ERROR);
        return "";
    }

    std::string response = httpSocket.getResponse();

    if (response.size() < 10) {
        Logger::getInstance()->log(httpSocket.getSocketId(), "RESPONSE HAS INVALID SIZE", LOG_LEVEL_ERROR);
        return "";
    }

    std::cout << "RESPONSE: " << response << std::endl;
    
    size_t pos1 = response.find("<image size=\"extralarge\">");
    size_t pos2 = response.find("</image>", pos1);    

    if (pos1 == response.npos || pos2 <= pos1)
        return "";

    std::string url = response.substr(pos1 + 25, pos2 - pos1 - 25);
    std::cout << "URL: " << url << std::endl;

    if (url.length() < 5) {
        Logger::getInstance()->log(httpSocket.getSocketId(), "PARSED URL IS INVALID", LOG_LEVEL_ERROR);
        return "";
    }

    std::string filename = m_artist + "_" + m_albumName;
    std::string path = "cache/" + filename + url.substr(url.length() - 4);

    saveImage(url, path);       
    
    return path;
}

void CoverDownloader::saveImage(std::string url, std::string path) {
    std::string host = HttpClientSocket::getHostFromUrl(url);
    std::string serverPath = HttpClientSocket::getServerPathFromUrl(url);

    HttpClientSocket httpSocket(host, 80);
    httpSocket.connectSocket();

    std::string req = "GET ";
    req += serverPath;
    req += " HTTP/1.1\r\nHost: ";
    req += host;
    req += "\r\n\r\n";

    std::cout << req << std::endl;

    if (!httpSocket.sendRequest(req)) {
        Logger::getInstance()->log(httpSocket.getSocketId(), "REQUEST FOR IMAGE WAS NOT SEND CORRECTLY", LOG_LEVEL_ERROR);
    }

    if (!httpSocket.saveResponseContent(path)) {
        Logger::getInstance()->log(httpSocket.getSocketId(), "COULD NOT SAVE IMAGE", LOG_LEVEL_ERROR);
    }
}


std::string CoverDownloader::getPath() const {
    return m_path;
}


std::string CoverDownloader::getAlbum() const {
    return m_albumName;
}

void CoverDownloader::setAlbum(std::string album) {
    m_albumName = album;
}

std::string CoverDownloader::getArtist() const {
    return m_artist;
}

void CoverDownloader::setArtist(std::string art) {
    m_artist = art;
}