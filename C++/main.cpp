#include "Buffer.h"
#include "Consumer.h"
#include "Producer.h"
#include "global.h"
#include <iostream>
#include <pthread.h>
using namespace std;

Buffer global_buffer;

int FakeJob::id = 0;

void *masterThread(void *arg){
    Producer * pointer = static_cast<Producer *>(arg);
    Producer master = *pointer;
    master.run();
    return (void *)  "Done";
}
void *slaveThread(void *arg){
    Consumer * pointer = static_cast<Consumer *>(arg);
    Consumer slave = *pointer;
    slave.run();
    return (void *) "Done";
}

int main(){
    int buffer_size;
    int max_job_length;
    cout << "Enter size of buffer:  ";
    cin >> buffer_size;
    cout << "Enter max job length:  ";
    cin >> max_job_length;

    //create a buffer of size given by users
    Buffer temp(buffer_size);
    //assign global_buffer to be the created and initialized buffer
    global_buffer = temp;

    Producer master(max_job_length);
    Consumer slaves[buffer_size];

    for(int i = 0; i < buffer_size;i++)
        slaves[i].Tid = i;

    int ret;
    ret = pthread_create(&master.thread, NULL, &masterThread, &master);
    if(ret!=0)  cout << "Creating master thread failed" << endl;


    for(int i = 0; i < buffer_size; i++){
        ret = pthread_create(&slaves[i].thread, NULL, &slaveThread, &slaves[i]);
        if(ret!=0)  cout << "Creating master thread failed" << endl;
    }

    sleep(30);

    return 0;
}