/* 
 * File:    Mutex.h
 * Project: ThePlayerServer
 * Author:  lubos
 *
 * Created on July 7, 2012, 9:46 AM
 */

#ifndef MUTEX_H
#define	MUTEX_H

#include <pthread.h>

class Mutex {
private:
    pthread_mutexattr_t m_attr;
    pthread_mutex_t     m_mutex;
    bool                m_isLocked;
    
public:
    Mutex();
    ~Mutex();
    
    void lock();
    void unlock();
    bool trylock();    
};


#endif	/* MUTEX_H */

