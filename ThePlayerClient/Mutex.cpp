#include "Mutex.h"

Mutex::Mutex() : m_isLocked(false) {
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
}

Mutex::~Mutex() {
}

void Mutex::lock() {    
    pthread_mutex_lock(&m_mutex);
}

void Mutex::unlock() {
    pthread_mutex_unlock(&m_mutex);    
}

bool Mutex::trylock() {
    if(pthread_mutex_trylock(&m_mutex) == 0) {
        return true;
    }
    return false;
}

