/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part1.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;
int main(int argc, char **argv)
{
    int pid = fork();
    if (pid == 0) {
        execl("bin/ls", "/bin/ls", "-la", NULL);
    } else if (pid == -1) {
        cout << "Error: Fork failed: " << strerror(errno) << endl;
    } else {
        waitpid(pid, NULL, 0);
    }
    exit(0);
}
