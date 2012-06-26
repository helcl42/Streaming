/* 
 * File:   ClientLibrary.h
 * Author: lubos
 *
 * Created on June 18, 2012, 2:39 PM
 */

#ifndef CLIENTLIBRARY_H
#define	CLIENTLIBRARY_H


#include <iostream>
#include <QTreeView>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QStyle>
#include <QtCore/QTimer>
#include <QtGui>
#include <QtCore/QAbstractItemModel>

#include "Utils.h"
#include "Types.h"
#include "Playlist.h"
#include "MediaApp.h"
#include "StreamMediaClient.h"

class Player;
class Library;
class Playlist;
class MediaApp;

class Library : public QWidget
{
    Q_OBJECT
public:           
    static Library* getInstance();
        
    virtual ~Library();
    
    void connectToServer(std::string url, int port);
    void enableSearch(bool val);
    void printQueryResutl();
    void clearData();
    
    std::string getAlbumTitle(int songId);
    std::string getArtistName(int songId);
    
    void setSatusMessage(std::string str);
    
private Q_SLOTS:
    void search();    
    void selectFromTree(const QItemSelection&, const QItemSelection&);     
    void download(Song* song);
    void downloadCallback();

private:
    friend class StreamMediaClient;
    Library();
    
    QToolButton* initButton(QStyle::StandardPixmap icon, const QString & tip, QObject *dstobj, const char *slot_method, QLayout *layout);
    void         createUI(QBoxLayout *appLayout);
    void         createModel();

    QToolButton*         m_searchButton;
    QLabel*              m_searchLabel;
    QLineEdit*           m_inputMessage;
    QStandardItemModel*  m_model;
    QTreeView*           m_viewData;
    QLabel*              m_statusMessasge;
    
    StreamMediaClient*   m_streamClient;
    Player*              m_player;
    Playlist*            m_playlist;            
    
    std::vector<Song*>   m_songs;
    std::vector<Album*>  m_albums;
    std::vector<Artist*> m_artists;            
    
    static Library* m_instance; 
};


#endif	/* CLIENTLIBRARY_H */

