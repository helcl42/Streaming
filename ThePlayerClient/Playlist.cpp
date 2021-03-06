#include <qt4/QtCore/qmetatype.h>
#include <qt4/QtGui/qtablewidget.h>
#include <qt4/QtCore/qvariant.h>
#include <qt4/QtGui/qheaderview.h>

#include "Playlist.h"
#include "Player.h"

Playlist::Playlist(QWidget* parent) : QTableWidget(0, 2, parent), m_position(0) {
    QStringList labels;
    labels << tr("Name") << tr("Length");
    setHorizontalHeaderLabels(labels);
    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    setColumnWidth(1, 50);
    horizontalHeader()->setContentsMargins(0, 0, 0, 0);
    verticalHeader()->setDefaultSectionSize(18);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setShowGrid(false);
}

Playlist::~Playlist() {
}

Song* Playlist::getActual() {
    if (!isEmpty()) {
        return &m_playlist[m_position];
    } else {
        return NULL;
    }
}

Song* Playlist::getNext() {
    if (!isEmpty()) {
        if (m_position < m_playlist.size() - 1) {
            m_position++;
        } else {
            m_position = 0;
        }
        return &m_playlist[m_position];
    } else {
        return NULL;
    }
}

int Playlist::getActualPosition() const {
    return m_position;
}

Song* Playlist::getPrevious() {
    if (!isEmpty()) {
        if (m_position == 0) {
            m_position = m_playlist.size() - 1;
        } else {
            m_position--;
        }
        return &m_playlist[m_position];
    } else {
        return NULL;
    }
}

void Playlist::insert(Song* song) {    
    std::string artistName = Library::getInstance()->getArtistName(song->getId());
    std::string albumName = Library::getInstance()->getAlbumTitle(song->getId());
    
    song->setArtistName(artistName);
    song->setAlbumName(albumName);
    m_playlist.push_back(*song);
    insertTableRow(song);
    //print();
}

void Playlist::insert(std::vector<Song*> & songVect) {
    std::vector<Song*>::iterator iter;
    std::string artistName;
    std::string albumName;
    for (iter = songVect.begin(); iter != songVect.end(); ++iter) {        
        artistName = Library::getInstance()->getArtistName((*iter)->getId());
        albumName = Library::getInstance()->getAlbumTitle((*iter)->getId());
        (*iter)->setArtistName(artistName);
        (*iter)->setAlbumName(albumName);
        m_playlist.push_back(**iter);
        insertTableRow(*iter);
    }
    //print();
}

bool Playlist::isEmpty() const {
    return m_playlist.empty();
}

void Playlist::removeSelected() {
    QList<QTableWidgetItem*> selected = selectedItems();
    for (int i = 0, row = 0; i < selected.size(); i++) {
        row = selected.at(i)->row();
        removeRow(row);
        m_playlist.erase(m_playlist.begin() + row);
    }
}

bool Playlist::isInPlaylist(Song* song) const {
    std::vector<Song>::const_iterator iter;
    for (iter = m_playlist.begin(); iter != m_playlist.end(); ++iter) {
        if ((*iter).getId() == song->getId()) {
            return true;
        }
    }
    return false;
}

void Playlist::print() {
    std::vector<Song>::iterator iter;
    for (iter = m_playlist.begin(); iter != m_playlist.end(); ++iter) {
        std::cout << "album: " << (*iter).getAlbum() << " title: "
                << (*iter).getTitle() << " length: " << (*iter).getLength() << std::endl;
    }
}

void Playlist::insertTableRow(Song* song) {
    int row = rowCount();
    QTableWidgetItem* nameItem = new QTableWidgetItem();
    nameItem->setText(QString::fromStdString(song->getTitle()));
    QTableWidgetItem* lengthItem = new QTableWidgetItem();
    //TODO            
    lengthItem->setText("--:--");
    insertRow(row);
    setItem(row, 0, nameItem);
    setItem(row, 1, lengthItem);
}

void Playlist::mouseDoubleClickEvent(QMouseEvent* /*event*/) {    
    m_position = currentRow();        
    MediaApp::getInstance()->openFile(&m_playlist[m_position]);
}







