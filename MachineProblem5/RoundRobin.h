#ifndef ROUNDROBIN_HEADER
#define ROUNDROBIN_HEADER
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "Scheduler.h"
#include "Process.h"
//RoundRobin Scheduler

//You can try 3 different approaches to change time_quantum 
//as the CPU keeps working
/*
1. use the least execution time of a certain process as the time quantum
2. Always use the greatest common dividor as the time quantum
3. Use a constant
*/
class RoundRobin : Scheduler{
private:
	int time_quantum;
	//used shared pointer from c++11 features
	vector<shared_ptr<Process> > processVec;
public:
	RoundRobin();
	RoundRobin(string file, int time_quantum);
	RoundRobin(vector<ProcessInfo> &process_info, int time_quantum);
	void set_time_quantum(int quantum);
	int get_time_quantum();
	void schedule_tasks();
	void print();
};



#endif
