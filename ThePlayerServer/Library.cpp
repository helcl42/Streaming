#include "Library.h"

Library::Library() : m_connection(NULL) {
    search_query = getAll = getAlbumById = getAlbumNameById = getArtistNameById = NULL;
}

bool Library::connectToDB(std::string host, std::string user, std::string password, std::string database) {

    deletePtrs();

    try {
        sql::Driver* driver = sql::mysql::get_driver_instance();
        m_connection = driver->connect(host, user, password);
        m_connection->setSchema(database);

        search_query = m_connection->prepareStatement("SELECT artist.id as artist_id, album.id as album_id, song.* "
                "FROM Artist artist JOIN Album album ON(album.by_id = artist.id) JOIN Song song ON(song.is_on_id = album.id)"
                " WHERE album.title LIKE ? OR artist.name LIKE ? OR song.title LIKE ?");

        getAll = m_connection->prepareStatement("SELECT artist.id as artist_id, album.id as album_id, song.* "
                "FROM Artist artist JOIN Album album ON(album.by_id = artist.id) JOIN Song song ON(song.is_on_id = album.id)");

        getArtistNameById = m_connection->prepareStatement("SELECT name FROM Artist WHERE id = ?");
        
        getSongById = m_connection->prepareStatement("SELECT * FROM Song WHERE id = ?");

        getAlbumNameById = m_connection->prepareStatement("SELECT title FROM Album WHERE id = ?");

        getAlbumById = m_connection->prepareStatement("SELECT id, title, length, url FROM Song WHERE is_on_id = ?");

    } catch (sql::SQLException &e) {
        if (m_connection) {
            try {
                m_connection->close();
            } catch (...) {
            }
            SAFE_DELETE(m_connection);
        }
        return false;

    } catch (std::runtime_error &e) {
        if (m_connection) {
            try {
                m_connection->close();
            } catch (...) {
            }
            SAFE_DELETE(m_connection);
        }
        return false;
    }
    return true;
}

void Library::deletePtrs() {
    if (m_connection) {
        try {
            m_connection->close();
        } catch (...) {
        }
        SAFE_DELETE(m_connection);
    }

    SAFE_DELETE(search_query);
    SAFE_DELETE(getAll);
    SAFE_DELETE(getAlbumById);
    SAFE_DELETE(getAlbumNameById);
    SAFE_DELETE(getArtistNameById);
}

Library::~Library(void) {
    deletePtrs();    
    for (std::vector<Artist* >::iterator art = m_library.begin(); art != m_library.end(); ++art) {
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
    m_library.clear();
}

std::string Library::getArtistName(int id) {
    if (!m_connection) {
        return "";
    }

    try {
        getArtistNameById->setInt(1, id);
        sql::ResultSet * result = getArtistNameById->executeQuery();

        if (result == NULL) {
            std::cerr << "NULL artistname" << std::endl;
            return "";
        }
        if (!result->next())
            return "";

        return result->getString("name");

    } catch (sql::SQLException & e) {
        std::cerr << "getArtistName exception: " << e.what() << std::endl;
        return "";
    }
}

Song* Library::getSong(int songId) {
    if (!m_connection) {
        return NULL;
    }

    try {
        getSongById->setInt(1, songId);
        sql::ResultSet* result = getSongById->executeQuery();

        if (result == NULL) {
            std::cerr << "Song does not exist" << std::endl;
            return NULL;
        }

        if (!result->next()) {
            return NULL;
        }

        return new Song(result->getInt("id"), result->getString("title"), result->getUInt("length"), result->getString("url"), NULL);

    } catch (sql::SQLException & e) {
        std::cerr << "getSongById exception: " << e.what() << std::endl;
        return NULL;
    }
}


std::string Library::getAlbumName(int id) {
    if (!m_connection) {
        return "";
    }

    try {
        getAlbumNameById->setInt(1, id);
        sql::ResultSet* result = getAlbumNameById->executeQuery();

        if (result == NULL) {
            std::cerr << "NULL albumName" << std::endl;
            return "";
        }

        if (!result->next()) {
            return "";
        }

        return result->getString("title");

    } catch (sql::SQLException & e) {
        std::cerr << "getAlbumName exception: " << e.what() << std::endl;
        return "";
    }
}

bool Library::search(std::string pattern) {

    if (!m_connection) {
        return false;
    }
    sql::ResultSet* result = NULL;

    try {
        if (pattern.length() == 0) {
            result = getAll->executeQuery();
        } else {
            sql::mysql::MySQL_Connection* mysql_conn = dynamic_cast<sql::mysql::MySQL_Connection*> (m_connection);
            pattern = mysql_conn->escapeString(pattern);

            pattern = replacePercent(pattern);

            pattern = "%" + pattern + "%";

            search_query->setString(1, pattern);
            search_query->setString(2, pattern);
            search_query->setString(3, pattern);
            result = search_query->executeQuery();
        }
    } catch (sql::SQLException & e) {
        std::cerr << "Search exception: " << e.what() << std::endl;
        return false;
    }
    if (result == NULL) {
        std::cerr << "NULL search result" << std::endl;
        return false;
    }

    clearLibrary();

    int lastAlbumId = -1;
    int lastArtistId = -1;


    Artist * art = NULL;
    Album * alb = NULL;

    if(result->rowsCount() < 1) {
        return false;
    }
    
    while (result->next()) {
        int artistId = result->getInt("artist_id");
        if (artistId != lastArtistId) {
            art = new Artist(artistId, getArtistName(artistId));
            m_library.push_back(art);
            lastArtistId = artistId;
        }

        int albumId = result->getInt("album_id");
        if (albumId != lastAlbumId) {
            alb = new Album(albumId, getAlbumName(albumId), art);
            art->m_albums.push_back(alb);
            lastAlbumId = albumId;
        }

        alb->m_songs.push_back(new Song(result->getInt("id"), result->getString("title"), result->getUInt("length"), result->getString("url"), alb));
    }
    return true;
}

std::vector<Album *> Library::getAlbums(int artistId) {
    for (std::vector<Artist *> ::iterator it = m_library.begin(); it != m_library.end(); it++) {
        if ((*it)->getId() == artistId) {
            return (*it)->getAlbums();
        }
    }
    return std::vector<Album *>();
}

std::vector<Song *> Library::getSongs(int artistId) {
    std::vector<Song*> songs;
    for (std::vector<Artist *> ::iterator it = m_library.begin(); it != m_library.end(); it++) {
        if ((*it)->getId() == artistId) {
            for (std::vector<Album *>::iterator alb = (*it)->m_albums.begin(); alb != (*it)->m_albums.end(); alb++) {
                songs.insert(songs.end(), (*alb)->m_songs.begin(), (*alb)->m_songs.end());
            }
            break;
        }
    }
    return songs;
}

Album * Library::getAlbum(int albumId) {
    for (std::vector<Artist *> ::iterator it = m_library.begin(); it != m_library.end(); it++) {
        for (std::vector<Album *>::iterator alb = (*it)->m_albums.begin(); alb != (*it)->m_albums.end(); alb++) {
            if ((*alb)->getId() == albumId) {
                return *alb;
            }
        }
    }
    return NULL;
}

std::vector<Artist *> Library::getArtists() {
    return m_library;
}

std::vector<Song *> Library::getSongs() {
    std::vector<Song *> songs;
    std::vector<Album *> albums = getAlbums();
    for (std::vector<Album *> ::iterator it = albums.begin(); it != albums.end(); it++) {
        songs.insert(songs.end(), (*it)->m_songs.begin(), (*it)->m_songs.end());
    }
    return songs;
}

std::vector<Album *> Library::getAlbums() {
    std::vector<Album *> albums;
    for (std::vector<Artist *> ::iterator it = m_library.begin(); it != m_library.end(); it++) {
        albums.insert(albums.end(), (*it)->m_albums.begin(), (*it)->m_albums.end());
    }
    return albums;
}

void Library::printResult() {
    std::cout << "DB RESULT:" << std::endl;
    for (std::vector<Artist *>::iterator it = m_library.begin(); it != m_library.end(); it++) {
        std::cout << (*it)->getName() << "\n";
        for (std::vector<Album *>::iterator alb = (*it)->m_albums.begin(); alb != (*it)->m_albums.end(); alb++) {
            std::cout << "\t" << (*alb)->getTitle() << "\n";
            for (std::vector<Song *>::iterator sng = (*alb)->m_songs.begin(); sng != (*alb)->m_songs.end(); sng++) {
                std::cout << "\t\t" << (*sng)->getTitle() << "\n";
            }
        }
    }
}

std::string Library::replacePercent(std::string from) {

    size_t pos = from.find("%");
    while (pos != from.npos) {
        from.replace(pos, 1, "\\%");
        pos = from.find("%", pos + 2);
    }
    return from;
}

void Library::clearLibrary() {
    for (std::vector<Artist *>::iterator it = m_library.begin(); it != m_library.end(); it++) {
        delete *it;
    }
    m_library.clear();
}

std::string Library::convertResultToString() {
    std::cout << "RESULT STRING:" << std::endl;
    std::stringstream ss;
    for (std::vector<Artist *>::iterator artIt = m_library.begin(); artIt != m_library.end(); artIt++) {
        Artist* artist = (*artIt);
        ss << "[" << artist->getId() << ":" << artist->getName();
        for (std::vector<Album *>::iterator albIt = (*artIt)->m_albums.begin(); albIt != (*artIt)->m_albums.end(); albIt++) {
            Album* album = (*albIt);
            ss << "[" << album->getId() << ":" << album->getTitle();
            for (std::vector<Song *>::iterator sngIt = (*albIt)->m_songs.begin(); sngIt != (*albIt)->m_songs.end(); sngIt++) {
                Song* song = (*sngIt);
                ss << "[" << song->getId() << ":" << song->getTitle() << ":" << song->getLength() << ":" << song->getUrl() << "]\n";
            }
            ss << "]\n";
        }
        ss << "]\n";
    }
    return ss.str();
}
