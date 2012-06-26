/* 
 * File:   AlbumWidget.h
 * Author: lubos
 *
 * Created on June 24, 2012, 6:28 PM
 */

#ifndef ALBUMWIDGET_H
#define	ALBUMWIDGET_H

#include <QLabel>
#include <QMap>

#include "CoverDownloader.h"

class AlbumWidget : public QLabel {
private:
    QMap<QString, QImage*> m_cache;
    CoverDownloader* m_cd;
    
    QImage* fromCache(QString name);
    QImage* fromDirCache(QString name);
    QImage* fromInternet(QString artist, QString album);    
    
    void    cache(QString name, QImage * img);

public:
    AlbumWidget(QWidget * parent = NULL);
    void LoadImage(QString album, QString artist);        
    virtual ~AlbumWidget();
    
    void    fromInternetCallback(std::string name, std::string path);
};


#endif	/* ALBUMWIDGET_H */

