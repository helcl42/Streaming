/* 
 * File:   Playlist.h
 * Author: lubos
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

class Playlist : public QTableWidget
{
public:
    Playlist(QWidget* parent);
    ~Playlist();

private:
    std::vector<Song> m_playlist;
    unsigned int m_position;    

public:
    Song* getNext();    
    int   getActualPosition() const;
    Song* getPrevious();
    void  insert(Song *pli);
    void  insert(std::vector<Song*> & vec);
    void  deleteSong(int id);
    bool  isInPlaylist(Song* song);
    void  print();
    void  insertTableRow(Song* song);
    void  mouseDoubleClickEvent(QMouseEvent* event);   
    bool  isEmpty();
    Song* getActual();
};


#endif	/* PLAYLIST_H */

