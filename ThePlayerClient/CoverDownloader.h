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

#include "TCPClientSocket.h"
#include "Thread.h"

#define API_KEY "d9a04dd513bfe567dcd1e8411d9ed69a"

class CoverDownloader : public Thread {
private:
    std::string m_artist;
    std::string m_albumName;
    
    std::string m_path;
    
    static std::string escapeStrings(std::string in);
    
protected:
    void ThreadProcedure();

public:
    CoverDownloader();
    CoverDownloader(std::string art, std::string album);
    virtual ~CoverDownloader();

    std::string download();    
    void saveImage(std::string url, std::string path);
    
    void setArtist(std::string art);
    std::string getArtist() const;
    
    void setAlbum(std::string album);
    std::string getAlbum() const;
    
    std::string getPath() const;        
};


#endif	/* COVERDOWNLOADER_H */

