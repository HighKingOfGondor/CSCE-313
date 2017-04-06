#include "RoundRobin.h"
using namespace std;

RoundRobin::RoundRobin() {
    time_quantum = 0;
}

RoundRobin::RoundRobin(string file, int quantum) {
    extractProcessInfo(file);
    set_time_quantum(quantum);
    sort(begin(process_info), end(process_info), [](ProcessInfo const &t1, ProcessInfo const &t2) {
        return get<1>(t1) < get<1>(t2);
    });
    
    for(auto& elem : process_info) {
        shared_ptr<Process> current_process(new Process(get<0>(elem), get<1>(elem), get<2>(elem)));
        processVec.push_back(current_process);
    }
}

void RoundRobin::set_time_quantum(int quantum) {
    time_quantum = quantum;
}

int RoundRobin::get_time_quantum() {
    return time_quantum;
}


//Schedule tasks based on RoundRobin Rule
//the jobs are put in the order the arrived
void RoundRobin::schedule_tasks() {
    int sys_time = 0;
    while(processVec.size() != 0) {
        bool found = false;
        for (int current_process = 0; current_process <= processVec.size(); current_process++) {
            if(current_process == processVec.size()) {
                break;
            } else if(processVec[current_process]->get_arrival_time() <= sys_time) {
                int service_time = 0;
                while (processVec[current_process]->is_Completed() == false && service_time < time_quantum) {
                    processVec[current_process]->Run(sys_time);
                    sys_time++;
                    service_time++;
                }
                if(processVec[current_process]->is_Completed() == true) {
                    processVec.erase(processVec.begin() + current_process);
                }
                found = true;
            }
        }
        if(!found)
        {
            cout << "sys time = " << sys_time << " no pid running" << endl;
            sys_time++;
        }
    }
}

RoundRobin::RoundRobin(vector<ProcessInfo> &process_info, int quantum){
    set_time_quantum(quantum);
    for(auto elem : process_info) {
        shared_ptr<Process> current_process(new Process(get<0>(elem), get<1>(elem), get<2>(elem)));
        processVec.push_back(current_process);
    }
}

void RoundRobin::print(){
    for (auto elem : processVec){
        cout << elem->getPid() << "; " << elem->get_arrival_time() << "; " << elem->get_cpu_burst_time() << endl;
    }
}
