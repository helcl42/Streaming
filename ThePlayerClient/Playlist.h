/* 
 * File:    Playlist.h
 * Project: ThePlayerClient
 * Author:  lubos
 *
 * Created on June 18, 2012, 2:44 PM
 */

#ifndef PLAYLIST_H
#define	PLAYLIST_H


#include <vector>
#include <iostream>
#include <QtGui/QListWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QString>

#include "Types.h"
#include "MediaApp.h"

class Song;
class Player;
class MediaApp;

class Playlist : public QTableWidget {
private:
    std::vector<Song> m_playlist;
    unsigned int      m_position;

protected:
    void insertTableRow(Song* song);

public:
    Playlist(QWidget* parent);
    ~Playlist();

    Song* getNext();
    Song* getActual();
    Song* getPrevious();

    int  getActualPosition() const;
    void insert(Song *pli);
    void insert(std::vector<Song*> & vec);
    void removeSelected();
    bool isInPlaylist(Song* song) const;
    void print();
    bool isEmpty() const;

    void mouseDoubleClickEvent(QMouseEvent* event);
};


#endif	/* PLAYLIST_H */

