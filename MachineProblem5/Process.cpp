#include "Process.h"

//Constructor
/*
initialize everything to 0
*/
Process::Process() {
    pid = 0;
    arrival_time = 0;
    cpu_burst_time = 0;
    remaining_time = 0;
    isCompleted = false;
}

//Constructor
//make sure set isCompleted to false
Process::Process(int pid_, int arrival_time_, int cpu_burst_time_) {
    pid = pid_;
    arrival_time = arrival_time_;
    cpu_burst_time = cpu_burst_time_;
    remaining_time = cpu_burst_time_;
    isCompleted = false;
}

//Copy Construcor
//Construct it with pid, arrival_time, cpu_burst_time, remaining_time and isCompleted
Process::Process(const Process &p){
    pid = p.pid;
    arrival_time = p.arrival_time;
    cpu_burst_time = p.cpu_burst_time;
    remaining_time = p.remaining_time;
    isCompleted = p.isCompleted;
}

//Every time, When your process is runing, use this function to update 
//the remaining time and monitor if the process is done or not
void Process::Run(int run_time) {
    //actually system time?
    cout << "sys time = " << run_time << ", pid running = " << pid << endl;
    remaining_time--;
    if (remaining_time <= 0) {
        cout << "sys time = " << run_time << ", pid job finished = " << pid << endl;
        isCompleted = true;
    }
}

int Process::getPid() const {
    return pid;
}
int Process::get_remaining_time() const {
    return remaining_time;
}
int Process::get_cpu_burst_time() const {
    return cpu_burst_time;
}
int Process::get_arrival_time() const {
    return arrival_time;
}
bool Process::is_Completed() const {
    return isCompleted;
}

//This function will be used in MLFQ implementation
void Process::change_arrival_time(int new_arrival_time) {
    arrival_time = new_arrival_time;
}

//This function might be used in MLFQ implementation since you need to put 
//undone process from other levels to last level, you may want to update 
//remaining time for them.
void Process::update_remaining_time(int updated_remaining_time){
    remaining_time = updated_remaining_time;
}

//Assignment Operator Overloading
//make sure do the assignment for pid, arrival_time, cpu_burst_time, remaining_time and isCompleted
Process & Process::operator=(const Process & p)
{
    pid = p.pid;
    arrival_time = p.arrival_time;
    cpu_burst_time = p.cpu_burst_time;
    remaining_time = p.remaining_time;
    isCompleted = p.isCompleted;
    return *this;
}


