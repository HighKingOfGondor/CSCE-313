#include "SRTF.h"
using namespace std;

void SRTF::printByArrivalTime(){
    cout << "pid   " << "  arrival time     " << "     burst time:\n";
    for (auto elem : process_info){
        cout << get<0>(elem) << "\t" << get<1>(elem) << "\t" << get<2>(elem) << endl;
    }
}

SRTF::SRTF(string file)
{
    cout << file;
    extractProcessInfo(file);
    sort(begin(process_info), end(process_info), [](ProcessInfo const &t1, ProcessInfo const &t2) {
        return get<1>(t1) < get<1>(t2);
    });
    
}
//scheduling implementation
void SRTF::schedule_tasks(){
    
    vector<Process> processes;
    int sys_time = 0;
    Process current_process;
    
    for(auto elem : process_info) {
        shared_ptr<Process> process(new Process(get<0>(elem), get<1>(elem), get<2>(elem)));
        SRTF_queue.push(process);
    }
    
    while(!SRTF_queue.empty()) {
        current_process = *SRTF_queue.top();
        SRTF_queue.pop();
        processes.push_back(current_process);
    }
    
    while(processes.size() != 0) {
        for (int selected_process = 0; selected_process <= processes.size(); selected_process++) {
            if(selected_process == processes.size()) {
                cout << "sys time = " << sys_time << " no pid running" << endl;
                sys_time++;
                break;
            } else if(processes[selected_process].get_arrival_time() <= sys_time) {
                while (processes[selected_process].is_Completed() == false) {
                    processes[selected_process].Run(sys_time);
                    sys_time++;
                }
                processes.erase(processes.begin() + selected_process);
                break;
            }
        }
    }
}
