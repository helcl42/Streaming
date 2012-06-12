/* 
 * File:   Thread.h
 * Author: lubos
 *
 * Created on May 17, 2012, 11:40 PM
 */

#ifndef THREAD_H
#define	THREAD_H

#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "Utils.h"

class Thread
{
private:
   	static void* ThreadProcedure(void* This) {
		((Thread *)This)->ThreadProcedure(); 
                return NULL;
	}

   	pthread_t      m_thread;
	pthread_attr_t m_attr;

protected:
   	virtual void ThreadProcedure() = 0;

public:
   	Thread() {}
   	virtual ~Thread() {}

   	// Returns true if the thread was successfully started
      bool RunThread() {
		pthread_attr_init(&m_attr);
		pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_JOINABLE);
		return (pthread_create(&m_thread, &m_attr, ThreadProcedure, this) == 0);
   	}

        // waits until threadProcedure is finished, true == success_join
   	bool WaitForThreadToExit() {
		int rcCount;
		rcCount = pthread_join(m_thread, NULL);
		pthread_attr_destroy(&m_attr);
		return (rcCount == 0);
   	}
};


class ThreadGroup
{
private:
   	static void* ThreadProcedure(void* This) {
		((ThreadGroup *)This)->ThreadProcedure(); 
                return NULL;
	}

   	pthread_t*     m_threads;
	int		   m_iSize;
	pthread_attr_t m_attr;

protected:
   	virtual void ThreadProcedure() = 0;

public:
	ThreadGroup(int count) {		
		if(count > 0) {
			m_iSize = count;
			m_threads = new pthread_t [count];
		}
	}

   	virtual ~ThreadGroup() {
		SAFE_DELETE_ARRAY(m_threads);
	}

   	// Returns true if the threads were successfully started
	bool RunThreads() {
		int rcCount = 0;
		pthread_attr_init(&m_attr);
		pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_JOINABLE);

		for(int i = 0; i < m_iSize; i++) {
			rcCount = pthread_create(&m_threads[i], &m_attr, ThreadProcedure, this);
			if(rcCount) {
				std::cout << "create_error" << std::endl;
				return false;
			}
		}
		return true;
	}

      // waits until threadProcedures are finished, true == success_join of all threads
	bool WaitForThreadToExit() {
		int rcCount = 0;
		for(int i = 0; i < m_iSize; i++) {
			rcCount = pthread_join(m_threads[i], NULL);
			if(rcCount) {
				std::cout << "join_error" << std::endl;
				return false;
			}
		}
		pthread_attr_destroy(&m_attr);
		return true;
	}
};

#endif	/* THREAD_H */

