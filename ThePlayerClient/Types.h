/* 
 * File:   Types.h
 * Author: lubos
 *
 * Created on June 11, 2012, 1:15 PM
 */

#ifndef TYPES_H
#define	TYPES_H


#include <iostream>
#include <vector>


class Artist;
class Album;
class Library;

class Song {
   // friend class Library;

    int m_id;
    std::string m_title;
    unsigned int m_length;
    std::string m_url;
    Album * m_album;

public:
    Song() {}

    Song(int id, std::string title, unsigned int length, std::string url, Album * album) : m_id(id), m_title(title),
    m_length(length), m_url(url), m_album(album) {
    }

    Song(std::string title, std::string url) : m_title(title), m_url(url)
    {}

    ~Song() {}

    int getId() const {
        return m_id;
    }

    std::string getTitle() {
        return m_title;
    }

    unsigned int getLength() {
        return m_length;
    }

    std::string getUrl() {
        return m_url;
    }

    Album * getAlbum() {
        return m_album;
    }

};

class Album {
    //friend class Library;
    std::string m_title;
    int m_id;
    Artist * m_artist;
    std::vector<Song *> m_songs;

    Album(int id, std::string title, Artist * artist) : m_title(title), m_id(id),
    m_artist(artist) {
    }

public:

    ~Album() {
        for (std::vector<Song*>::iterator it = m_songs.begin(); it != m_songs.end(); it++) {
            delete *it;
        }
    }

    std::string getTitle() {
        return m_title;
    }

    int getId() const {
        return m_id;
    }

    Artist * getArtist() {
        return m_artist;
    }

    std::vector<Song *> getSongs() {
        return m_songs;
    }
};

class Artist {
  //  friend class Library;
    std::string m_name;
    int m_id;
    std::vector<Album *> m_albums;

    Artist(int id, std::string name) : m_name(name), m_id(id) {
    }

public:

    ~Artist() {
        for (std::vector<Album *>::iterator it = m_albums.begin(); it != m_albums.end(); it++) {
            delete *it;
        }
    }

    std::string getName() {
        return m_name;
    }

    int getId() const {
        return m_id;
    }

    std::vector<Album *> getAlbums() {
        return m_albums;
    }
};

//#include "Library.h"


#endif	/* TYPES_H */

