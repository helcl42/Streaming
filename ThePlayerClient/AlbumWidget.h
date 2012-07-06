/* 
 * File:    AlbumWidget.h
 * Project: ThePlayerClient
 * Author:  lubos
 *
 * Created on June 24, 2012, 6:28 PM
 */

#ifndef ALBUMWIDGET_H
#define	ALBUMWIDGET_H

#include <QLabel>
#include <QPixmap>
#include <QMap>
#include <iostream>

#include "CoverDownloader.h"

class AlbumWidget : public QWidget {
    Q_OBJECT
private:
    CoverDownloader m_cd;
    QMap<QString, QImage*> m_cache;
    QPixmap m_pixmap;
    QString m_nowPlaying;

    QImage* fromCache(QString name);
    QImage* fromDirCache(QString name);
    void    fromInternet(QString artist, QString album);

    void cache(QString name, QImage* img);
    
private Q_SLOTS:
    void updatePixmap(const QImage& image);

protected:
    void paintEvent(QPaintEvent *event);

public:
    AlbumWidget(QWidget* parent = NULL);
    virtual ~AlbumWidget();

    void LoadImage(QString album, QString artist);    
};


#endif	/* ALBUMWIDGET_H */

