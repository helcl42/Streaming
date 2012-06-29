#include "Library.h"
#include "StreamMediaClient.h"

Library* Library::m_instance = NULL;

Library* Library::getInstance() {
    if (!m_instance) {
        m_instance = new Library();
    }
    return m_instance;
}

Library::Library() : QWidget() {
    m_playlist = MediaApp::getInstance()->getPlaylistInstance();
    m_streamClient = NULL;

    QVBoxLayout* vLayout = new QVBoxLayout();
    m_viewData = new QTreeView(this);
    m_viewData->setHeaderHidden(true);
    m_model = new QStandardItemModel();

    m_viewData->setModel(m_model);

    QItemSelectionModel *selectionModel = m_viewData->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectFromTree(const QItemSelection &, const QItemSelection &)));

    vLayout->addWidget(m_viewData);
    vLayout->setContentsMargins(2, 2, 2, 2);
    createUI(vLayout);
    setLayout(vLayout);
    resize(300, 300);
    createModel();
}

Library::~Library() {
    SAFE_DELETE(m_searchLabel);
    SAFE_DELETE(m_searchButton);
    SAFE_DELETE(m_inputMessage);
    SAFE_DELETE(m_model);
    SAFE_DELETE(m_viewData);
    SAFE_DELETE(m_statusMessasge);
    clearData();
}

QToolButton* Library::initButton(QStyle::StandardPixmap icon, const QString& tip, QObject* dstobj,
        const char* slot_method, QLayout* layout) {
    QToolButton* button = new QToolButton();
    button->setIcon(style()->standardIcon(icon));
    button->setIconSize(QSize(20, 20));
    button->setToolTip(tip);
    connect(button, SIGNAL(clicked()), dstobj, slot_method);
    layout->addWidget(button);

    return button;
}

void Library::createUI(QBoxLayout* appLayout) {
    QHBoxLayout *inputLayout = new QHBoxLayout();

    m_searchLabel = new QLabel("Search:");
    m_statusMessasge = new QLabel("Unkonwn status.");
    m_inputMessage = new QLineEdit();
    m_inputMessage->setContentsMargins(2, 2, 2, 2);
    m_searchButton = initButton(QStyle::SP_ArrowForward, tr("Search"), this, SLOT(search()), inputLayout);
    inputLayout->addWidget(m_searchButton);
    inputLayout->addWidget(m_searchLabel);
    inputLayout->addWidget(m_inputMessage);
    inputLayout->addWidget(m_statusMessasge);
    appLayout->addLayout(inputLayout);
}

void Library::connectToServer(std::string url, int port) {
    if (m_streamClient == NULL) {
        m_streamClient = new StreamMediaClient(url.c_str(), port);
    } else {
        Logger::getInstance()->log(m_streamClient->getId(), "ALREADY CONNECTED.", LOG_LEVEL_INFO);
        return;
    }

    m_streamClient->setOperation(OP_CONNECT);
    m_streamClient->RunThread();
}

void Library::search() {
    clearData();
    m_statusMessasge->setText("Querying");
    m_searchButton->setEnabled(false);
    m_streamClient->setOperation(OP_QUERY);
    m_streamClient->setQueryString(m_inputMessage->text().toStdString());
    m_streamClient->RunThread();

}

void Library::searchCallback(bool success) {
    if (success) {
        createModel();
        m_statusMessasge->setText("Query finished");
    } else {
        m_statusMessasge->setText("Query failed");
    }
    m_searchButton->setEnabled(true);
}

void Library::createModel() {
    m_model->clear();
    printQueryResutl();

    for (std::vector<Artist*>::iterator art = m_artists.begin(); art != m_artists.end(); ++art) {
        Artist* artist = (*art);
        QString str = QString::fromStdString(artist->getName());
        QStandardItem* art = new QStandardItem(str);
        std::cout << artist->getName() << std::endl;
        std::vector<Album *> vct = artist->getAlbums();
        for (std::vector<Album*>::iterator alb = vct.begin(); alb != vct.end(); ++alb) {
            Album* album = (*alb);
            str = QString::fromStdString(album->getTitle());
            std::cout << "\t" << album->getTitle() << std::endl;
            QStandardItem* alb = new QStandardItem(str);
            std::vector<Song*> songs = album->getSongs();
            for (std::vector<Song*>::iterator sng = songs.begin(); sng != songs.end(); ++sng) {
                Song* song = (*sng);
                str = QString::fromStdString(song->getTitle());
                std::cout << "\t\t" << song->getTitle() << std::endl;
                QStandardItem* sng = new QStandardItem(str);
                m_songs.push_back(song);
                alb->appendRow(sng);
            }
            art->appendRow(alb);
            m_albums.push_back(album);
        }
        //m_artists.push_back(artist);
        m_model->appendRow(art);
    }
}

void Library::selectFromTree(const QItemSelection &, const QItemSelection &) {
    const QModelIndex index = m_viewData->selectionModel()->currentIndex();
    QString selectedText = index.data(Qt::DisplayRole).toString();

    int hierarchyLevel = 1;
    QModelIndex seekRoot = index;

    while (seekRoot.parent() != QModelIndex()) {
        seekRoot = seekRoot.parent();
        hierarchyLevel++;
    }

    switch (hierarchyLevel) {
        case 1: //Artist
        {
            /*
            for (std::vector<Artist*>::iterator jj = m_artists.begin(); jj != m_artists.end(); ++jj) {
                if ((((*jj)->getName()).compare(selectedText.toStdString())) == 0)
                    for (std::vector<Album* >::iterator ii = m_albums.begin(); ii != m_albums.end(); ++ii) {
                        if ((*ii)->getArtist()->getId() == (*jj)->getId())
                            for (std::vector<Song*>::iterator iter = m_songs.begin(); iter != m_songs.end(); ++iter) {
                                if ((*iter)->getAlbum()->getId() == (*ii)->getId()) {                                   
                                    m_playlist->insert(*iter);
                                }
                            }
                    }
            }
             */
            break;
        }
        case 2: //Album
        {
            /*
            std::vector<Album* >::iterator ii;
            for (ii = m_albums.begin(); ii != m_albums.end(); ++ii) {
                if ((((*ii)->getTitle()).compare(selectedText.toStdString())) == 0)
                    for (std::vector<Song*>::iterator iter = m_songs.begin(); iter != m_songs.end(); ++iter) {
                        if ((*iter)->getAlbum()->getId() == (*ii)->getId())
                            m_playlist->insert(*iter);
                    }
            }
             */
            break;
        }
        case 3: //Song
        {
            Song* selectedSong = NULL;
            for (std::vector<Song*>::iterator iter = m_songs.begin(); iter != m_songs.end(); ++iter) {
                if ((((*iter)->getTitle()).compare(selectedText.toStdString())) == 0) {
                    selectedSong = (*iter);
                    break;
                }
            }
            download(selectedSong);
            break;
        }
        default:
            std::cout << "Unknown type selected." << std::endl;
    }
}

void Library::download(Song* song) {
    if (song != NULL) {
        if (!m_playlist->isInPlaylist(song)) {
            if (!m_streamClient->isRunning()) {
                m_statusMessasge->setText("Downloading");
                m_searchButton->setEnabled(false);
                m_streamClient->setOperation(OP_DOWNLOAD);
                m_streamClient->setDownloadSong(song);
                m_streamClient->RunThread();
            }
        } else {
            Logger::getInstance()->log(m_streamClient->getId(), "ALREADY IN PLAYLIST", LOG_LEVEL_INFO);
        }
    } else {
        Logger::getInstance()->log(m_streamClient->getId(), "SELECTED SONG IS NULL", LOG_LEVEL_FATAL);
    }
}

void Library::downloadCallback(bool success) {
    if (success) {
        m_statusMessasge->setText("Download Finished");
    } else {
        m_statusMessasge->setText("Download Failed");
    }
    m_searchButton->setEnabled(true);
}

void Library::printQueryResutl() {
    std::cout << "DB QUERY RESULT:" << std::endl;
    for (std::vector<Artist *>::iterator it = m_artists.begin(); it != m_artists.end(); ++it) {
        std::cout << (*it)->getName() << "\n";
        for (std::vector<Album *>::iterator alb = (*it)->m_albums.begin(); alb != (*it)->m_albums.end(); ++alb) {
            std::cout << "\t" << (*alb)->getTitle() << "\n";
            for (std::vector<Song *>::iterator sng = (*alb)->m_songs.begin(); sng != (*alb)->m_songs.end(); ++sng) {

                std::cout << "\t\t" << (*sng)->getTitle() << "\n";
            }
        }
    }
}

std::string Library::getArtistName(int songId) {
    for (std::vector<Artist *>::iterator it = m_artists.begin(); it != m_artists.end(); ++it) {
        for (std::vector<Album *>::iterator alb = (*it)->m_albums.begin(); alb != (*it)->m_albums.end(); ++alb) {
            for (std::vector<Song *>::iterator sng = (*alb)->m_songs.begin(); sng != (*alb)->m_songs.end(); ++sng) {
                if ((*sng)->getId() == songId) {
                    std::cout << "FOUND: " << (*it)->getName() << std::endl;
                    return (*it)->getName();
                }
            }
        }
    }
    std::cout << "NOT FOUND " << std::endl;
    return "";
}

std::string Library::getAlbumTitle(int songId) {
    for (std::vector<Artist *>::iterator it = m_artists.begin(); it != m_artists.end(); ++it) {
        for (std::vector<Album *>::iterator alb = (*it)->m_albums.begin(); alb != (*it)->m_albums.end(); ++alb) {
            for (std::vector<Song *>::iterator sng = (*alb)->m_songs.begin(); sng != (*alb)->m_songs.end(); ++sng) {
                if ((*sng)->getId() == songId) {
                    std::cout << "FOUND ALBUM: " << (*alb)->getTitle() << std::endl;
                    return (*alb)->getTitle();
                }
            }
        }
    }
    std::cout << "NOT FOUND " << std::endl;
    return "";
}

void Library::setSatusMessage(std::string str) {
    m_statusMessasge->setText(QString::fromStdString(str));
}

//TODO leaks

void Library::clearData() {
    /*     
        for (std::vector<Artist* >::iterator art = m_artists.begin(); art != m_artists.end(); ++art) {
            for (std::vector<Album* >::iterator alb = (*art)->m_albums.begin(); alb != (*art)->m_albums.end(); ++alb) {
                for (std::vector<Song*>::iterator sng = (*alb)->m_songs.begin(); sng != (*alb)->m_songs.end(); ++sng) {
                    SAFE_DELETE(*sng);
                }
                (*alb)->m_songs.clear();
                SAFE_DELETE(*alb);
            }
            (*art)->m_albums.clear();
            SAFE_DELETE(*art);
        }
     */
    m_artists.clear();
}

void Library::enableSearch(bool val) {
    m_searchButton->setEnabled(val);
}