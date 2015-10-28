#ifndef RWLock_h
#define RWLock_h
#include <iostream>

class RWLock {
    public:
        RWLock();
        void rlock();
        void wlock();
        void runlock();
        void wunlock();

    private:

        int readers;
        bool writing;
        pthread_cond_t lock;
        pthread_mutex_t mutex;
        pthread_mutex_t turnstile;

};

#endif
