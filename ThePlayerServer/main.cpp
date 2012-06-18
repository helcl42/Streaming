/* 
 * File:   main.cpp
 * Author: lubos
 *
 * Created on May 17, 2012, 11:36 PM
 */

#include "StreamServer.h"

void printUsage() {
    std::cout << "Usage: ./server <portNumber>" << std::endl;
}

int main(int argc, char** argv) {
    if (argc == 2) {
        int port;
        if (argv[1] != NULL) {
            port = atoi(argv[1]);
        } else {
            printUsage();
            return 0;
        }
        StreamServer* ss = new StreamServer(port);
        ss->startServer();
        ss->shutdown();
        SAFE_DELETE(ss);
    } else {
        printUsage();
    }
    return 0;
}



