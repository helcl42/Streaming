/* 
 * File:   Utils.h
 * Author: lubos
 *
 * Created on June 11, 2012, 1:43 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p );   ( p ) = NULL; } }

#include <vector>
#include <sstream>
#include <iostream>

class Util {
public:
    static std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        return split(s, delim, elems);
    }
};

#endif	/* UTILS_H */

