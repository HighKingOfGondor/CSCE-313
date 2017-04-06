#ifndef ROUNDROBIN_HEADER
#define ROUNDROBIN_HEADER
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "Scheduler.h"
#include "Process.h"
//RoundRobin Scheduler

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
