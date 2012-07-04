/* 
 * File:   Types.h
 * Author: lubos
 *
 * Created on June 18, 2012, 2:42 PM
 */

#ifndef TYPES_H
#define	TYPES_H


#include <iostream>
#include <vector>
#include <sstream>
#include <QtCore/QTime>

#include "Utils.h"

class Artist;
class Album;

class Song {
private:
    friend class Library;
    int m_id;
    std::string m_title;
    std::string m_length;
    std::string m_url;
    Album*      m_album;
    
    bool        m_fromLibrary;
    bool        m_isAudio;
    std::string m_albumName;
    std::string m_artistName;

public:

    Song() {}

    Song(int id, std::string title, std::string length, std::string url, Album* album = NULL, bool fromLib = false, bool audio = false)
    : m_id(id), m_title(title), m_length(length), m_url(url), m_album(album), m_fromLibrary(fromLib), m_isAudio(audio) {}

    Song(std::string title, std::string url) : m_title(title), m_url(url) {}

    ~Song() {}

    int getId() const {
        return m_id;
    }

    void setId(int id) {
        m_id = id;
    }

    std::string getTitle() {
        return m_title;
    }

    void setTitle(std::string title) {
        m_title = title;
    }

    std::string getLength() {
        return m_length;
    }

    void setLength(std::string len) {
        m_length = len;
    }

    std::string getUrl() {
        return m_url;
    }

    void setUrl(std::string url) {
        m_url = url;
    }

    Album* getAlbum() {
        return m_album;
    }

    void setAlbum(Album* album) {
        m_album = album;
    }
    
    void setAlbumName(std::string album) {
        m_albumName = album;
    }
    
    std::string getAlbumName() const {
        return m_albumName;
    }
    
    void setArtistName(std::string artist) {
        m_artistName = artist;
    }
    
    std::string getArtistName() const {
        return m_artistName;
    }

    bool isFromLibrary() const {
        return m_fromLibrary;
    }

    void setFromLibrary(bool fromLib) {
        m_fromLibrary = fromLib;
    }

    bool isAudio() {
        return m_isAudio;
    }

    void setAudio(bool isAudio) {
        m_isAudio = isAudio;
    }

    std::string getAsString() {
        std::stringstream ss;
        ss << m_id;
        ss << ":";
        ss << m_title;
        ss << ":";
        ss << m_length;
        ss << ":";
        ss << m_url;
        return ss.str();
    }

    static Song* getSongFromString(std::string songString) {
        Song* song = NULL;
        std::vector<std::string> strs = Util::split(songString, ':');
        if (strs.size() > 3) {
            song = new Song();
            song->setId(atoi(strs[0].c_str()));
            song->setTitle(strs[1]);
            song->setLength(strs[2]);
            song->setUrl(strs[3]);
            std::cout << "URL: " << strs[3] << std::endl;
        } else {
            return NULL;
        }
        return song;
    }

    static bool findOutIfAudioMedia(Song* song) {
        if (!song->m_url.empty()) {
            unsigned int size = song->m_url.size();
            std::string fileType = song->m_url.substr(size - 4, size - 1);
            std::cout << "TYPE: " << fileType << std::endl;

            if (fileType.compare(".mp3") == 0) {
                return true;
            } else if (fileType.compare(".wma") == 0) {
                return true;
            } else if (fileType.compare(".wav") == 0) {
                return true;
            } else if (fileType.compare("flac") == 0) {
                return true;
                //  } else if(fileType.compare(".ogg")) {
                //    return true;
            }
        }
        return false;
    }

    static std::string getSongTitleFromPath(std::string path) {
        std::string res;
        std::vector<std::string> strs = Util::split(path, '/');
        return strs.back();
    }

    std::string toString() {
        std::stringstream ss;
        ss << m_id;
        ss << ":";
        ss << m_title;
        ss << ":";
        ss << m_length;
        ss << ":";
        ss << m_url;
        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& out, const Song& song) {
        out << "SongId: " << song.m_id;
        out << "\nTitle: " << song.m_title;
        out << "\nUrl: " << song.m_url;
        out << "\nLength: " << song.m_length;
        out << "\n---------------------------\n";
        return out;
    }
};

class Album {
private:
    friend class Library;
    int                 m_id;
    std::string         m_title;
    Artist*             m_artist;
    std::vector<Song *> m_songs;  

public:
     Album() {}

     Album(int id, std::string title, Artist * artist) : m_id(id), m_title(title), m_artist(artist) {}

    ~Album() {
        for (std::vector<Song*>::iterator it = m_songs.begin(); it != m_songs.end(); it++) {
            delete *it;
        }
    }

    void setTitle(std::string title) {
        m_title = title;
    }

    std::string getTitle() const {
        return m_title;
    }

    void setId(int id) {
        m_id = id;
    }

    int getId() const {
        return m_id;
    }

    Artist* getArtist() const {
        return m_artist;
    }

    void setArtist(Artist* artist) {
        m_artist = artist;
    }

    void addSong(Song* song) {
        m_songs.push_back(song);
    }

    std::vector<Song *> getSongs() {
        return m_songs;
    }

    static Album* getAlbumFromString(std::string albumString) {
        Album* album = NULL;
        std::vector<std::string> strs = Util::split(albumString, ':');
        if (strs.size() > 1) {
            album = new Album();
            album->setId(atoi(strs[0].c_str()));
            album->setTitle(strs[1]);
        } else {
            return NULL;
        }
        return album;
    }

    std::string toString() {
        std::stringstream ss;
        ss << m_id;
        ss << ":";
        ss << m_title;
        return ss.str();
    }
};

class Artist {
private:
    friend class Library;
    int                  m_id;
    std::string          m_name;
    std::vector<Album *> m_albums;

public:
    Artist() {}
    
    Artist(int id, std::string name) : m_id(id), m_name(name) {}

    ~Artist() {
        for (std::vector<Album *>::iterator it = m_albums.begin(); it != m_albums.end(); it++) {
            delete *it;
        }
    }

    void setName(std::string name) {
        m_name = name;
    }

    std::string getName() {
        return m_name;
    }

    void setId(int id) {
        m_id = id;
    }

    int getId() const {
        return m_id;
    }

    std::vector<Album *> getAlbums() {
        return m_albums;
    }

    void addAlbum(Album* album) {
        m_albums.push_back(album);
    }

    static Artist* getAlbumFromString(std::string artistString) {
        Artist* artist = NULL;
        std::vector<std::string> strs = Util::split(artistString, ':');
        if (strs.size() > 1) {
            artist = new Artist();
            artist->setId(atoi(strs[0].c_str()));
            artist->setName(strs[1]);
        } else {
            return NULL;
        }
        return artist;
    }

    std::string toString() {
        std::stringstream ss;
        ss << m_id;
        ss << ":";
        ss << m_name;
        return ss.str();
    }
};


#endif	/* TYPES_H */

