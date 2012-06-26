#include <QtCore/QDir>
#include <iostream>

#include "AlbumWidget.h"
#include "CoverDownloader.h"

AlbumWidget::AlbumWidget(QWidget * parent) : QLabel(parent) {
    m_cd = new CoverDownloader();
    resize(300, 300);
}

AlbumWidget::~AlbumWidget() {
    for (QMap<QString, QImage *>::iterator i = m_cache.begin(); i != m_cache.end(); ++i) {
        delete *i;
    }
    SAFE_DELETE(m_cd);
}

/*
void AlbumWidget::LoadImage(QString album, QString artist) {
    QString name = artist + "_" + album;
    QImage* img = NULL;

    if ((img = fromCache(name))) {
        std::cout << "Cache hit" << std::endl;
    } else if ((img = fromDirCache(name))) {
        std::cout << "Dir cache hit" << std::endl;
        cache(name, img);
    } else {        
        fromInternet(artist, album);
        return;
    }

    if (img != NULL) {
        resize(img->width(), img->height());
        setPixmap(QPixmap::fromImage(*img, 0));
    } else {
        resize(300, 300);
        clear();
        std::cout << "NULL" << std::endl;
    }
}
**/

void AlbumWidget::LoadImage(QString album, QString artist) {
    QString name = artist + "_" + album;
    QImage* img = NULL;

    if ((img = fromCache(name))) {
        std::cout << "Cache hit" << std::endl;
    } else if ((img = fromDirCache(name))) {
        std::cout << "Dir cache hit" << std::endl;
        cache(name, img);
    } else {        
        img = fromInternet(artist, album);
        if (img != NULL) {
            std::cout << "From internet" << std::endl;
            cache(name, img);
        }         
    }

    if (img != NULL) {
        resize(img->width(), img->height());
        setPixmap(QPixmap::fromImage(*img, 0));
    } else {
        resize(300, 300);
        clear();
        std::cout << "NULL" << std::endl;
    }
}
 
QImage* AlbumWidget::fromCache(QString name) {
    QMap<QString, QImage *>::iterator it = m_cache.find(name);
    if (it != m_cache.end())
        return *it;
    return NULL;
}

QImage* AlbumWidget::fromDirCache(QString name) {

    QDir d = QDir::current();

    std::cout << "Current dir " << d.absolutePath().toStdString() << std::endl;
    if (d.cd("cache")) {
        QImage * img = new QImage();
        QStringList filters;
        filters << name + ".*";
        QStringList files = d.entryList(filters, QDir::Readable | QDir::Files | QDir::NoSymLinks);
        for (QStringList::Iterator it = files.begin(); it != files.end(); ++it) {
            std::cout << it->toStdString() << std::endl;
            if (img->load("cache/" + *it)) {
                break;
            }
        }
        if (!img->isNull()) {
            return img;
        }
        SAFE_DELETE(img);
    }
    return NULL;
}


void AlbumWidget::fromInternetCallback(std::string name, std::string path) {
    QString fileName = QString::fromStdString(name);
    QString file = QString::fromStdString(path);

    QImage* img = new QImage();
    std::cout << "Image file " << file.toStdString() << std::endl;
    if(!img->load(file)) {
        std::cout << "CAN NOT LOAD IMAGE" << std::endl;
        SAFE_DELETE(img);
        return;
    }
    
    cache(fileName, img);

    resize(img->width(), img->height());
    setPixmap(QPixmap::fromImage(*img, 0)); 
}


QImage* AlbumWidget::fromInternet(QString artist, QString album) {
    
    m_cd->setAlbum(album.toStdString());
    m_cd->setArtist(artist.toStdString());
    m_cd->RunThread();
    //cant use Callback, its unsafe to call pixmap from another Thread    
    m_cd->WaitForThreadToExit();    
    
    QString file = QString::fromStdString(m_cd->getPath());
    if (file.size() > 0) {
        QImage * img = new QImage();
        std::cout << "Image file " << file.toStdString() << std::endl;
        if (img->load(file)) {
            return img;
        }
        SAFE_DELETE(img);
    }     
    return NULL;
}

/*
QImage* AlbumWidget::fromInternet(QString artist, QString album) {    
    m_cd->setAlbum(album.toStdString());
    m_cd->setArtist(artist.toStdString());
    m_cd->RunThread();    
    return NULL;
}
**/

void AlbumWidget::cache(QString name, QImage* img) {
    m_cache.insert(name, img);
}

