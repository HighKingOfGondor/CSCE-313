#ifndef MLFQ_HEADER
#define MLFQ_HEADER

#include <iostream>
#include <fstream>
#include <tuple>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
using namespace std;

#define LEVEL1_CAPACITY	20
#define LEVEL2_CAPACITY	20
#define LEVEL3_CAPACITY	20

#define LEVLE1_QUANTUM	4
#define LEVLE2_QUANTUM	8
#define LEVLE3_QUANTUM	16

#define FCFS_WORKLOAD	5

#include "Scheduler.h"
#include "ArrivalEarlierThan.h"

class MLFQ:Scheduler {
private:
	Process last_process_fcfs;
public:
	vector<vector<shared_ptr<Process> > > upperLevels;
	priority_queue<Process, vector<Process>, ArrivalEarlierThan> lowestLevel;
	void update_last_process_fcfs(Process process_to_fcfs);
	void degrade_process(shared_ptr<Process> p, unsigned int level, unsigned int pos);
	int level_jump(shared_ptr<Process> p, unsigned int start_level, unsigned int pos, unsigned int goal_level);
	void schedule_tasks();
    int returnQuantum(int);
	MLFQ(string file);
};


#endif
