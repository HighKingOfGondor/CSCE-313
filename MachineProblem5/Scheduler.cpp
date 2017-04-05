#include "Scheduler.h"

//Read a process file to extract process information
//All content goes to proces_info vector
void Scheduler::extractProcessInfo(string file){
    ifstream stream;
    stream.open(file, ifstream::in);
    int pid;
    int arrival_time;
    int burst_time;
    if (stream) {
        while (stream) {
            stream >> pid >> arrival_time >> burst_time;
            auto process = ProcessInfo(make_tuple(pid, arrival_time, burst_time));
            process_info.push_back(process);
        }
    } else {
        std::cerr << "couldn't process file!" << endl;
    }
}
