/* 
 * File:   main.cpp
 * Author: lubos
 *
 * Created on June 11, 2012, 1:12 PM
 */

#include <QtGui/QApplication>
#include <QtGStreamer/QGst/Init>
#include "StreamClient.h"
#include "MediaApp.h"
#include "CoverDownloader.h"
#include "StreamMediaClient.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGst::init(&argc, &argv);

    MediaApp* media = MediaApp::getInstance();
    media->show();

    if (argc == 2) {
        Song* sn = new Song();
        sn->setUrl(std::string(argv[1]));
        media->openFile(sn);
    }

    app.exec();
    SAFE_DELETE(media);
    return 0;
}

/*
int main() {
    //CoverDownloader coverDownloader("Cher", "Believe");            
    CoverDownloader coverDownloader("Led Zeppelin", "Houses of the Holy");    
    coverDownloader.RunThread();
    coverDownloader.WaitForThreadToExit();    
    return 0;
}
*/