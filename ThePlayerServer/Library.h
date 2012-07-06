/* 
 * File:    Library.h
 * Project: ThePlayerServer
 * Author:  lubos
 *
 * Created on June 20, 2012, 4:55 PM
 */

#ifndef LIBRARY_H
#define	LIBRARY_H


#include <string>
#include <vector>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/driver.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>

#include "Utils.h"
#include "Types.h"

class Library {
private:    
    sql::Connection*        m_connection;
    std::vector<Artist *>   m_library;

    sql::PreparedStatement* getArtistNameById;

    sql::PreparedStatement* getSongById;
    sql::PreparedStatement* getAlbumById;
    sql::PreparedStatement* getAlbumNameById;

    sql::PreparedStatement* search_query;
    sql::PreparedStatement* getAll;   

    std::string getArtistName(int id);
    std::string getAlbumName(int id);

    std::string replacePercent(std::string from);

    void deletePtrs();
    void clearLibrary();

public:
    Library();
    ~Library();
   
    bool connectToDB(std::string host, std::string user, std::string password, std::string database);    
    bool search(std::string pattern);
    
    std::string convertResultToString();    
    
    void printResult();
    
    std::vector<Album *>  getAlbums(int artistId);   
    std::vector<Song *>   getSongs(int artistId);   
    Album*                getAlbum(int albumId);   
    Song*                 getSong(int songId);
    std::vector<Artist *> getArtists();       
    std::vector<Song *>   getSongs();   
    std::vector<Album *>  getAlbums();          
};


#endif	/* LIBRARY_H */

