#include "FCFS.h"
using namespace std;

FCFS::FCFS(string file)
{
    extractProcessInfo(file);
    for(auto elem : process_info) {
        Process current_process(get<0>(elem), get<1>(elem), get<2>(elem));
        fcfs_queue.push(current_process);
    }
    
}

void FCFS::print(){
    cout<<"pid\t"<<"  arrival time\t"<<"\tburst time:\n";
    for(auto ele:process_info){
        cout<<get<0>(ele)<<"\t"<<get<1>(ele)<<"\t"<<get<2>(ele)<<endl;
    }
}

//Assuming the time the CPU starts working with process is system time 0
void FCFS::schedule_tasks(){
    int sys_time = 0;
    while(!fcfs_queue.empty()) {
        Process curProcess = fcfs_queue.top();
        while (curProcess.is_Completed() == false) {
            if (curProcess.get_arrival_time() > sys_time) {
                cout << "sys time = " << sys_time << " no pid running" << endl;
                sys_time++;
            } else if (curProcess.get_arrival_time() <= sys_time) {
                curProcess.Run(sys_time);
                sys_time++;
            }
        }
        fcfs_queue.pop();
    }
}

FCFS::FCFS(){}
