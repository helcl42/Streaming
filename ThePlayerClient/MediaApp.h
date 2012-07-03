/* 
 * File:   MediaApp.h
 * Author: lubos
 *
 * Created on June 18, 2012, 1:40 PM
 */

#ifndef MEDIAAPP_H
#define	MEDIAAPP_H


#include <QtGui/QWidget>
#include <QtGui/QStyle>
#include <QtCore/QTimer>

#include "Library.h"
#include "Playlist.h"
#include "AlbumWidget.h"
#include "VolumeSlider.h"

class Player;
class Playlist;
class Library;
class QBoxLayout;
class QLabel;
class QSlider;
class QToolButton;
class QTimer;

class MediaApp : public QWidget
{
    Q_OBJECT    
public:    
    static MediaApp* getInstance();  
    
    ~MediaApp();    
    void openFile(Song* song);      
    void openLibraryCallback(bool val);
    
    bool isLibraryOpenned() const;
    bool isLibraryVisible() const;
    
    Playlist*    getPlaylistInstance() const;
    Player*      getPlayerInstance() const;
    AlbumWidget* getAlbumWidgetInstance() const;
    
protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent* close);
    
private Q_SLOTS:
    void open();
    void prev();
    void next();
    void openLibrary();
    void toggleFullScreen();        

    void onStateChanged();
    void onPositionChanged();

    void setPosition(int position);

    void showControls(bool show = true);
    void hideControls();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
     MediaApp();
    
    QToolButton* initButton(QStyle::StandardPixmap icon, const QString& tip, QObject *dstobj, const char* slot_method, QLayout* layout);
    void         createUI(QBoxLayout *appLayout);

    QString          m_baseDir;    
    QToolButton*     m_openButton;
    QToolButton*     m_fullScreenButton;
    QToolButton*     m_playButton;
    QToolButton*     m_pauseButton;
    QToolButton*     m_stopButton;
    QToolButton*     m_prevButton;
    QToolButton*     m_nextButton;
    QToolButton*     m_openLibrary;
    QSlider*         m_positionSlider;
    VolumeSlider*    m_volumeSlider;
    QLabel*          m_positionLabel;
    QLabel*          m_volumeLabel;
   
    QTimer           m_fullScreenTimer;
    
    AlbumWidget*     m_albumWidget;
    Player*          m_player;
    Playlist*        m_playlist;           
    bool             m_libraryOpened;   
    
    static MediaApp* m_instance;      
};


#endif	/* MEDIAAPP_H */

