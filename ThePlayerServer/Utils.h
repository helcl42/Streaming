/* 
 * File:   Utils.h
 * Author: lubos
 *
 * Created on May 18, 2012, 2:35 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p );   ( p ) = NULL; } }


class Utils
{
public:
    
};

#endif	/* UTILS_H */

