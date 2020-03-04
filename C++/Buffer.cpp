#include "Buffer.h"

#include <cstdlib>
#include <iostream>

Buffer::Buffer(int length){
    maxSize = length;
    front = 0;
    rear = 0;
    bufLen = 0;
    Q.resize(length);
    sem_init(&lock, 0, 1);
    sem_init(&full, 0, 1);
    sem_init(&empty, 0, 1);
}
void Buffer::push(FakeJob job){
    if(isFull()){
        sem_wait(&full);
    }
    sem_wait(&lock);
    //increment element tracker (bufLen)
    bufLen++;
    //move rear over by one
    rear = (rear + 1) % maxSize;
    //add new job to the end of the buffer
    Q[rear] = job;
    sem_post(&lock);
    //if there are threads waiting for the queue to not be empty, signal to wake one up
    int emptyVal;
    sem_getvalue(&empty, &emptyVal);
    if(emptyVal < 1){
        sem_post(&empty);
    }
    sem_post(&lock);
}

FakeJob Buffer::pull(){

    if(isEmpty()){
        sem_wait(&empty);
    }
    sem_wait(&lock);
    //decrement element tracker
    bufLen--;
    //move the front pointer over by one
    front = (front + 1) % maxSize;

    FakeJob pulled = Q[front];
    sem_post(&lock);
    //if there are threads waiting for the queue to not be full, signal to wake one up
    int fullVal;
    sem_getvalue(&full, &fullVal);
    if(fullVal < 1){
        sem_post(&full);
    }
    return pulled;
}
