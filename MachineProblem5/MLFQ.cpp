#include "MLFQ.h"

//The goal of this function is to fill the 4 levels with process_info
//You should sort the process_info by the arrival_time of the Process;
//Process with smaller arrival time will have smaller index in the vector

//After filling in the top 3 levels, then do insertion for the fcfs level
MLFQ::MLFQ(string file){
    extractProcessInfo(file);
    sort(begin(process_info), end(process_info), [](ProcessInfo const &t1, ProcessInfo const &t2) {
        return get<1>(t1) <= get<1>(t2);
    });
    
    vector<shared_ptr<Process>> l1, l2, l3;
    for (auto& elem : process_info) {
        if (l1.size() < LEVEL1_CAPACITY) {
            shared_ptr<Process> current_process(new Process(get<0>(elem), get<1>(elem), get<2>(elem)));
            l1.push_back(current_process);
        } else if (l2.size() < LEVEL2_CAPACITY) {
            shared_ptr<Process> current_process(new Process(get<0>(elem), get<1>(elem), get<2>(elem)));
            l2.push_back(current_process);
        } else if (l3.size() < LEVEL3_CAPACITY) {
            shared_ptr<Process> current_process(new Process(get<0>(elem), get<1>(elem), get<2>(elem)));
            l3.push_back(current_process);
        } else {
            Process current_process(get<0>(elem), get<1>(elem), get<2>(elem));
            lowestLevel.push(current_process);
        }
    }
    upperLevels.push_back(l1);
    upperLevels.push_back(l2);
    upperLevels.push_back(l3);
    
}
//This function is used to keep track of the process who entered the fcfs queue most recently
//The purpose is to properly adjust the upcoming process's arrival time
//If the arrival time is changed, you can still check its original arrival time by accessing the
//process_info variable
void MLFQ::update_last_process_fcfs(Process process_to_fcfs) {
    last_process_fcfs = process_to_fcfs;
}

//This function has the following assumptions:
/*
	1. The start_level is not equal goal_level;
	2. goal_level is larger then start_level
	3. When you use this function, you should know the process should not jump from level 0 to level 2 or 3 if the level 1 has a space there.
 Generally, when you degrade a process, it tries to go to the level below by one level and if that level is full, it will keep going down
 until it finds a level which has space there.
	4. Successful jump will return 1, else 0
	5. To successfully jump to the goal_level, the process must go to the end of the vector corresponding to goal_level
 */
//start_level is the level the process is located at, it is one value of 0 , 1, 2;
//pos is its index in the vector
//goal_level is the level it tries to enter
int MLFQ::level_jump(shared_ptr<Process> p, unsigned int start_level, unsigned int pos, unsigned int goal_level) {
    if (start_level >= goal_level) return 0;
    if (upperLevels[goal_level].size() >= 20) return 0;
    return 1;
}


/*
 p is the process which is going to be degrade to lower levle
 levle is the level it is located currently
 legal value of level can be: 0, 1, 2 Since no need to degrade for last level
 pos is the its index in the vector
 0: it is located at the top level
 1:  it is located at the second highest level
 2:  it is located at the third highest level
 3: it is in the fcfs level, no need to degrade, it will stay there until finishing the job and leave
 */

//pos is the index of the process in the vector
//Your goal is to degrade this process by one level
//You can use level_jump() function here based on which level the process is going to jump
void MLFQ::degrade_process(shared_ptr<Process> p, unsigned int level, unsigned int pos) {
    bool foundSpot = false;
    unsigned int nextLevel = level + 1;
    if (level == 3) return;
    if (level == 2) {
        upperLevels[level].erase(upperLevels[level].begin() + pos);
        lowestLevel.push(*p);
    } else {
        int num = 1;
        while (!foundSpot) {
            if (nextLevel == 3) {
                foundSpot = true;
                upperLevels[level].erase(upperLevels[level].begin() + pos);
                lowestLevel.push(*p);
            } else if (level_jump(p, level, pos, nextLevel) && nextLevel < 3) {
                foundSpot = true;
                upperLevels[level].erase(upperLevels[level].begin() + pos);
                upperLevels[nextLevel].push_back(p);
            } else {
                nextLevel++;
            }
        }
    }
}

int MLFQ::returnQuantum(int i) {
    if (i == 0) return LEVLE1_QUANTUM;
    if (i == 1) return LEVLE2_QUANTUM;
    if (i == 2) return LEVLE3_QUANTUM;
    else return 0;
}

/*
 You can use multiple loops here to do the job based on the document;
 Make sure print out the timing information correctly
 */
void MLFQ::schedule_tasks(){
    int sys_time = 0;
    int i = 0;
    while (i < 3) {
        int j = 0;
        while(upperLevels[i].size() != 0) {
            bool found = false;
            if (upperLevels[i][j]->get_arrival_time() <= sys_time) {
                int service_time = 0;
                while(upperLevels[i][j]->is_Completed() == false && service_time < returnQuantum(i)) {
                    upperLevels[i][j]->Run(sys_time);
                    sys_time++;
                    service_time++;
                }
                if (upperLevels[i][j]->is_Completed() == true) {
                    upperLevels[i].erase(upperLevels[i].begin() + j);
                    j--;
                } else {
                    degrade_process(upperLevels[i][j], i, j);
                    j--;
                }
                j++;
                found = true;
            } else {
                cout << "sys time = " << sys_time << " no pid running" << endl;
                sys_time++;
            }
        }
        i++;
    }
    
    while(!lowestLevel.empty()) {
        Process curProcess = lowestLevel.top();
        while (curProcess.is_Completed() == false) {
            if (curProcess.get_arrival_time() > sys_time) {
                cout << "sys time = " << sys_time << " no pid running" << endl;
                sys_time++;
            } else if (curProcess.get_arrival_time() <= sys_time) {
                curProcess.Run(sys_time);
                sys_time++;
            }
        }
        lowestLevel.pop();
    }
}
