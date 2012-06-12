/* 
 * File:   main.cpp
 * Author: lubos
 *
 * Created on June 11, 2012, 1:12 PM
 */
#include <cstdlib>
#include "StreamClient.h"

void printUsage() {
    std::cout << "Usage: ./server <IPaddress> <portNumber>" << std::endl;
}

int main(int argc, char** argv) {

    if (argc == 3) {
        int port = atoi(argv[2]);
        StreamClient sc(argv[1], port);
        sc.connectToServer();
        sc.downloadMedia(11234);
    }
    return 0;
}


