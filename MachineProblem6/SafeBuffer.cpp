//
//  SafeBuffer.cpp
//
//
//  Created by Joshua Higginbotham on 11/4/15.
//
//

#include "SafeBuffer.h"
#include <string>
#include <queue>

SafeBuffer::SafeBuffer() {
    pthread_mutex_init(&m, NULL);

}

SafeBuffer::~SafeBuffer() {
    pthread_mutex_destroy(&m);
}

int SafeBuffer::size() {
    pthread_mutex_lock(&m);
    size_t size = safe_buffer.size();
    pthread_mutex_unlock(&m);
    return size;
}

void SafeBuffer::push_back(string str) {
    pthread_mutex_lock(&m);
    safe_buffer.push(str);
    pthread_mutex_unlock(&m);
	return;
}

string SafeBuffer::retrieve_front() {
    pthread_mutex_lock(&m);
    string front = safe_buffer.front();
    safe_buffer.pop();
    pthread_mutex_unlock(&m);
	return front;
}
