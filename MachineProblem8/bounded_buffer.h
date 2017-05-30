#include <mutex>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <stdio.h>
#include <string>
#include "semaphore.h"

#ifndef _bounded_buffer_H_
#define _bounded_buffer_H_

using namespace std;

class Bounded_buffer {
    Semaphore *full, *empty, *mux;
    queue<string> b_buffer;
    
public:
    Bounded_buffer(int size) {
        mux = new Semaphore(1);
        full = new Semaphore(0);
        empty = new Semaphore(size);
    }
    
    ~Bounded_buffer() {
        delete full, empty;
    }
    
    void push(string str) {
        empty->P();
        mux->P();
        b_buffer.push(str);
        mux->V();
        full->V();
    }
    
    string pull() {
        full->P();
        mux->P();
        string str = b_buffer.front();
        b_buffer.pop();
        mux->V();
        empty->V();
        return str;
    }
};

#endif
