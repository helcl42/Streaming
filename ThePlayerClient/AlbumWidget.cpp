#include <QtCore/QDir>
#include <QtGui>
#include <qt4/QtGui/qwidget.h>
#include <qt4/QtCore/qmetatype.h>

#include "AlbumWidget.h"
#include "CoverDownloader.h"

AlbumWidget::AlbumWidget(QWidget* parent) : QWidget(parent) {
    qRegisterMetaType<QImage > ("QImage");
    connect(&m_cd, SIGNAL(renderedImage(QImage)),
            this, SLOT(updatePixmap(QImage)));

    resize(300, 300);
}

AlbumWidget::~AlbumWidget() {
    for (QMap<QString, QImage *>::iterator i = m_cache.begin(); i != m_cache.end(); ++i) {
        delete *i;
    }    
}

void AlbumWidget::updatePixmap(const QImage& image) {
    m_pixmap = QPixmap::fromImage(image);
    update();
}

void AlbumWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (m_pixmap.isNull()) {
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, tr("Downloading cover, please wait..."));
        return;
    }

    painter.drawPixmap(0, 0, m_pixmap);
}

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
        std::cout << "Internet" << std::endl;        
        return;
    }

    if (img != NULL) {
        resize(img->width(), img->height());
        m_pixmap = QPixmap::fromImage(*img);
        update();
    } else {
        resize(300, 300);
        std::cout << "NULL" << std::endl;
    }
}

QImage* AlbumWidget::fromCache(QString name) {
    QMap<QString, QImage *>::iterator it = m_cache.find(name);
    if (it != m_cache.end()) {
        return *it;
    }
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

void AlbumWidget::fromInternet(QString artist, QString album) {
    m_cd.setAlbum(album.toStdString());
    m_cd.setArtist(artist.toStdString());
    m_cd.downloadCover();
}

void AlbumWidget::cache(QString name, QImage* img) {
    m_cache.insert(name, img);
}

