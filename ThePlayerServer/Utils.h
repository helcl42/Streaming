/* 
 * File:    Utils.h
 * Project: ThePlayerServer
 * Author:  lubos
 *
 * Created on May 18, 2012, 2:35 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p );   ( p ) = NULL; } }

class Util {
public:

    static void deleteNewLines(std::string& s) {
        int n = 0;
        for (unsigned int i = 0; i < s.length(); i++) {
            if (s[i] == '\n') {
                n++;
            } else {
                s[i - n] = s[i];
            }
        }
        s.resize(s.length() - n);
    }

};

#endif	/* UTILS_H */

