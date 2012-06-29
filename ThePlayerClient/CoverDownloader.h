/* 
 * File:   CoverDowloader.h
 * Author: lubos
 *
 * Created on June 22, 2012, 3:07 PM
 */

#ifndef COVERDOWNLOADER_H
#define	COVERDOWNLOADER_H

#include <string>
#include <iostream>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "TCPClientSocket.h"

#define API_KEY "d9a04dd513bfe567dcd1e8411d9ed69a"

class QImage;

class CoverDownloader : public QThread {
    Q_OBJECT
    
private:    
    QWaitCondition m_condition;
    
    std::string m_artist;
    std::string m_albumName;   

    static std::string escapeStrings(std::string in);

protected:
     void run();

public:
    CoverDownloader(QObject *parent = 0);   
    virtual ~CoverDownloader();

    void download();
    void saveImage(std::string url, std::string path);

    void setArtist(std::string art);
    std::string getArtist() const;

    void setAlbum(std::string album);
    std::string getAlbum() const;   
    
    QImage* getImage(QString name);

    void downloadCover();
    
signals:
     void renderedImage(const QImage& image);
};


#endif	/* COVERDOWNLOADER_H */

