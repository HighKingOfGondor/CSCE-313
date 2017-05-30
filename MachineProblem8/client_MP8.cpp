/*
    File: client_E8.cpp

    Author: J. Higginbotham
    Department of Computer Science
    Texas A&M University
    Date  : 2016/05/21

    Based on original code by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31

*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <assert.h>
#include <fstream>
#include <numeric>
#include <vector>
#include "reqchannel.h"
#include "bounded_buffer.h"

struct request_info {
    Bounded_buffer* request_buffer;
    int num_req, num_workers, buffer_size;
    string message;
};

struct worker_info {
    NetworkRequestChannel* chan;
    Bounded_buffer* request_buffer, *john_buf, *jane_buf, *joe_buf;
};

struct stat_info {
    int num_req;
    vector<int>* count;
    Bounded_buffer* buf;
};

class atomic_standard_output {
    pthread_mutex_t console_lock;
public:
    atomic_standard_output() { pthread_mutex_init(&console_lock, NULL); }
    ~atomic_standard_output() { pthread_mutex_destroy(&console_lock); }
    void print(std::string s){
        pthread_mutex_lock(&console_lock);
        std::cout << s << std::endl;
        pthread_mutex_unlock(&console_lock);
    }
};

atomic_standard_output threadsafe_standard_output;

std::string make_histogram_table(std::string name1, std::string name2,
        std::string name3, std::vector<int> *data1, std::vector<int> *data2,
        std::vector<int> *data3) {
    std::stringstream tablebuilder;
    tablebuilder << std::setw(25) << std::right << name1;
    tablebuilder << std::setw(15) << std::right << name2;
    tablebuilder << std::setw(15) << std::right << name3 << std::endl;
    for (int i = 0; i < data1->size(); ++i) {
        tablebuilder << std::setw(10) << std::left
                << std::string(
                        std::to_string(i * 10) + "-"
                                + std::to_string((i * 10) + 9));
        tablebuilder << std::setw(15) << std::right
                << std::to_string(data1->at(i));
        tablebuilder << std::setw(15) << std::right
                << std::to_string(data2->at(i));
        tablebuilder << std::setw(15) << std::right
                << std::to_string(data3->at(i)) << std::endl;
    }
    tablebuilder << std::setw(10) << std::left << "Total";
    tablebuilder << std::setw(15) << std::right
            << accumulate(data1->begin(), data1->end(), 0);
    tablebuilder << std::setw(15) << std::right
            << accumulate(data2->begin(), data2->end(), 0);
    tablebuilder << std::setw(15) << std::right
            << accumulate(data3->begin(), data3->end(), 0) << std::endl;

    return tablebuilder.str();
}

void* request_thread_function(void* arg) {
    request_info* info = (request_info*) arg;

    for (int i = 0; i < info->num_req; i++) {
        info->request_buffer->push(info->message);
    }
    pthread_exit(NULL);
}

void* worker_thread_function(void* arg) {
    worker_info* w =(worker_info*) arg;

    while(true){
        std::string request = w->request_buffer->pull();
        std::string response = w->chan->send_request(request);

        if(request == "data John Smith"){
            w->john_buf->push(response);
        }
        else if(request == "data Jane Smith"){
            w->jane_buf->push(response);
        }
        else if(request == "data Joe Smith"){
            w->joe_buf->push(response);
        }
        else if(request == "quit"){
            break;
        }
    }
    pthread_exit(NULL);
}

void* stat_thread_function(void* arg) {
    stat_info* stat = (stat_info*) arg;

    for(int i = 0; i < stat->num_req; i++){
        std::string response = stat->buf->pull();
        stat->count->at(stoi(response) / 10) += 1;
    }
    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
    int n = 10; //default number of requests per "patient"
    int b = 50; //default size of request_buffer
    int w = 10; //default number of worker threads
    string host_name="127.0.0.1";
    bool USE_ALTERNATE_FILE_OUTPUT = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:b:w:mh")) != -1) {
        switch (opt) {
        case 'n':
            n = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 'w':
            w = atoi(optarg);
            break;
        case 'm':
            USE_ALTERNATE_FILE_OUTPUT = true;
            break;
        case 'h':
            host_name=optarg;
            break;
        default:
            std::cout << "This program can be invoked with the following flags:" << std::endl;
            std::cout << "-n [int]: number of requests per patient" << std::endl;
            std::cout << "-b [int]: maximum number of requests that will be allowed in the request buffer" << std::endl;
            std::cout << "-w [int]: number of worker threads" << std::endl;
            std::cout << "-m: use output2.txt instead of output.txt for all file output" << std::endl; //purely for convenience, you may find it helpful since you have to make two graphs instead of one, and they require different data
            std::cout << "-h: print this message and quit" << std::endl;
            std::cout << "Example: ./client_solution -n 10000 -b 50 -w 120 -m" << std::endl;
            std::cout << "If a given flag is not used, a default value will be given" << std::endl;
            std::cout << "to its corresponding variable. If an illegal option is detected," << std::endl;
            std::cout << "behavior is the same as using the -h flag." << std::endl;
            exit(0);
        }
    }

    int pid = fork();
    if (pid == 0) {
        ofstream ofs;
        if(USE_ALTERNATE_FILE_OUTPUT) ofs.open("output2.txt", ios::out | ios::app);
        else ofs.open("output.txt", ios::out | ios::app);
        
        std::cout << "n = " << n << std::endl;
        std::cout << "b = " << b << std::endl;
        std::cout << "w = " << w << std::endl;
        
        std::cout << "CLIENT STARTED:" << std::endl;
        std::cout << "Establishing control channel... " << std::flush;
        NetworkRequestChannel *chan = new NetworkRequestChannel("control", NetworkRequestChannel::CLIENT_SIDE);
        std::cout << "done." << std::endl;

        auto start = std::chrono::steady_clock::now(); //start time

        request_info requests;
        request_info john_info, jane_info, joe_info;
        worker_info wi[w];
        stat_info john_stat, jane_stat, joe_stat;
        Bounded_buffer* request_buffer = new Bounded_buffer(b);
        Bounded_buffer* john_buf = new Bounded_buffer(b);
        Bounded_buffer* jane_buf = new Bounded_buffer(b); 
        Bounded_buffer* joe_buf = new Bounded_buffer(b); 
        NetworkRequestChannel* worker_channel[w];
        pthread_t workers[w];
        pthread_t john_st, jane_st, joe_st;
        vector<int> john_frequency_count(10, 0);
        vector<int> jane_frequency_count(10, 0);
        vector<int> joe_frequency_count(10, 0);

        requests.num_req = n;
        requests.num_workers = w;
        requests.buffer_size = b;

        john_info = requests;
        jane_info = requests;
        joe_info = requests;

        john_info.request_buffer = request_buffer;
        jane_info.request_buffer = request_buffer;
        joe_info.request_buffer = request_buffer;

        john_info.message = "data John Smith";
        jane_info.message = "data Jane Smith";
        joe_info.message = "data Joe Smith";

        pthread_t John, Jane, Joe;

        pthread_attr_t attr;
        pthread_attr_init(&attr);

        pthread_create(&John, &attr, request_thread_function, &john_info);
        pthread_create(&Jane, &attr, request_thread_function, &jane_info);
        pthread_create(&Joe, &attr, request_thread_function, &joe_info);


        for(int i = 0; i < w; i++){
            std::string str = chan->send_request("newthread");
            NetworkRequestChannel* temp = new NetworkRequestChannel(str, NetworkRequestChannel::CLIENT_SIDE);
            worker_channel[i] = temp;
        }

        for(int i = 0; i < w; i++){
            wi[i].chan = worker_channel[i];
            wi[i].request_buffer = request_buffer;
            wi[i].john_buf = john_buf;
            wi[i].jane_buf = jane_buf;
            wi[i].joe_buf = joe_buf;
        }

        for (int i = 0; i < w; i++){
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&workers[i], &attr, worker_thread_function, &wi[i]);
        }

        john_stat.count = &john_frequency_count;
        jane_stat.count = &jane_frequency_count;
        joe_stat.count = &joe_frequency_count;

        john_stat.num_req = n;
        jane_stat.num_req = n;
        joe_stat.num_req = n;

        john_stat.buf = john_buf;
        jane_stat.buf = jane_buf;
        joe_stat.buf = joe_buf;

        pthread_create(&john_st, &attr, stat_thread_function, &john_stat);
        pthread_create(&jane_st, &attr, stat_thread_function, &jane_stat);
        pthread_create(&joe_st, &attr, stat_thread_function, &joe_stat);

        pthread_join(John, NULL);
        pthread_join(Jane, NULL);
        pthread_join(Joe, NULL);

        for(int i = 0; i < w; i++){
            request_buffer->push("quit");
        }

        for (int i = 0; i < w; i++){
            pthread_join(workers[i], NULL);
        }

        pthread_join(john_st, NULL);
        pthread_join(jane_st, NULL);
        pthread_join(joe_st, NULL);

        auto end = std::chrono::steady_clock::now(); //end time

        delete request_buffer, john_buf, jane_buf, joe_buf;

        for(int i = 0; i < w; i++){
            delete worker_channel[i];
        }

        ofs.close();
        std::cout << "Sleeping..." << std::endl;
        usleep(10000);
        chan->send_request("quit");
        string a = make_histogram_table("data John","data Jane","data Joe",&john_frequency_count,&jane_frequency_count, &joe_frequency_count);
        cout << a << endl;

        std::cout << "\nTime: " << ((end - start) / std::chrono::milliseconds(1)) * .001 << " seconds\n\n";

    }
    else if(pid != 0) execl("dataserver", NULL);
}

