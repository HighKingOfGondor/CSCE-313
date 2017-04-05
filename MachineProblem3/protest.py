import sys
import re
import os
from subprocess import check_output, CalledProcessError


class Proctest(object):
    """
    Object to contain process data and getter functions
    """
    def __init__(self, pid):

        fileStatus = "/proc/" + pid + "/status"
        fileStat = "/proc/" + pid + "/stat"
        fileStatus1 = fileStatus.strip("\n")
        fileStat1 = fileStat.strip("\n")

        self.processID = pid
        self.status = []
        self.uids = []
        self.guids = []
        with open (fileStatus1, 'r') as f:
            self.status = f.read().splitlines()
        for line in self.status:
            match = re.search(r'\bPpid:\b', line)
            if match:
                self.ppid = line[line.index(match) + 1]
            match = re.search(r'\bUid:\b', line)
            if match:
                self.uids = line.split()
        for line in self.status:
            match = re.search(r'\bGid:\b', line)
            if match:
                self.guids = line.split()
            match = re.search(r'\bState:\b', line)
            if match:
                self.state = line[line.index(match) + 1]
            match = re.search(r'\bTgid:\b', line)
            if match:
                self.tid = line[line.index(match) + 1]
            match = re.search(r'\bThreads:\b', line)
            if match:
                self.threadNum = line[line.index(match) + 1]

        self.stat = []
        with open (fileStat1, 'r') as f1:
            self.stat = f1.read().split()



    def getpid(self):
        localpid = self.processID
        return localpid

    def getppid(self):
        localppid = self.ppid
        return localppid

    def geteuid(self):
        euid = self.uids[2]
        return euid

    def getegid(self):
        geiud = self.guids[2]
        return geiud

    def getruid(self):
        ruid = self.uids[1]
        return ruid

    def getrgid(self):
        rgid = self.guids[1]
        return rgid

    def getfsuid(self):
        fsuid = self.uids[4]
        return fsuid

    def getfsgid(self):
        fsgid = self.guids[4]
        return fsgid

    def getstate(self):
        localstate = self.state
        return localstate

    def getthread_count(self):
        thread_count = self.threadNum[1]
        return thread_count

    def getpriority(self):
        priority = self.stat[17]
        return priority

    def getniceness(self):
        nice = self.stat[18]
        return nice

    def getstime(self):
        stime = self.stat[14]
        return stime

    def getutime(self):
        utime = self.stat[13]
        return utime

    def getcstime(self):
        cstime = self.stat[16]
        return cstime

    def getcutime(self):
        cutime = self.stat[15]
        return cutime

    def getstartcode(self):
        startTime = self.stat[21]
        return startTime

    def getendcode(self):
        endCode = self.stat[26]
        return endCode

    def getesp(self):
        esp = self.stat[28]
        return esp

    def geteip(self):
        eip = self.stat[29]
        return eip

    def getfiles(self):
        """
        Returns process's current number of open file descriptors

        Work needed here!
        """
        pass


    def getvoluntary_context_switches(self):
        """
        Returns the number of times that the process has voluntarily
        yielded control to the kernel.

        Work needed here!
        """
        pass


    def getnonvoluntary_context_switches(self):
        """
        Returns the number of times that the process has had control
        taken from it forcefully by the kernel.

        Work needed here!
        """
        pass


    def getlast_cpu(self):
        """
        Returns the last cpu that the process executed on

        Work needed here!
        """
        pass


    def getallowed_cpus(self):
        """
        Returns a list of allowed processors

        Work needed here!
        """
        pass


    def getmemory_map(self):
        """
        Returns process's memory map

        Work needed here!
        """
        pass

def main():

    # Read in PID
    sys.stdout.write("Enter the PID of a process: ")
    sys.stdout.flush()
    process_pid = sys.stdin.readline()

    process_data = Proctest(process_pid)

    # Start printing out values
    print ""
    print "Process Information:"
    print "  1) Identifiers"
    print "     PID: %s" % process_data.getpid()
    print "     PPID: %s" % process_data.getppid()
    print "     EUID: %s" % process_data.geteuid()
    print "     EGID: %s" % process_data.getegid()
    print "     RUID: %s" % process_data.getruid()
    print "     RGID: %s" % process_data.getrgid()
    print "     FSUID: %s" % process_data.getfsuid()
    print "     FSGID: %s" % process_data.getfsgid()
    print ""
    print "  2) State"
    print "     State: %s" % process_data.getstate()
    print ""
    print "  3) Thread Information"
    print "     Thread Count: %s" % process_data.getthread_count()
    print ""
    print "  4) Priority"
    print "     Priority Number: %s" % process_data.getpriority()
    print "     Niceness Value: %s" % process_data.getniceness()
    print ""
    print "  5) Time Information"
    print "     stime: %s" % process_data.getstime()
    print "     utime: %s" % process_data.getutime()
    print "     cstime: %s" % process_data.getcstime()
    print "     cutime: %s" % process_data.getcutime()
    print ""
    print "  6) Address Space"
    print "     Startcode: %s" % process_data.getstartcode()
    print "     Endcode: %s" % process_data.getendcode()
    print "     ESP: %s" % process_data.getesp()
    print "     EIP: %s" % process_data.geteip()
    print ""
    print "  7) Resources"
    print "     File Handles: %s" % process_data.getfiles()
    print "     Voluntary Context Switches: %s" % process_data.getvoluntary_context_switches()
    print "     Nonvoluntary Context Switches: %s" % process_data.getnonvoluntary_context_switches()
    print ""
    print "  8) Processor"
    print "     Last Processor: %s" % process_data.getlast_cpu()
    print "     Allowed Cores: %s" % process_data.getallowed_cpus()
    print ""
    print "  9) Memory Map"
    temp_mem_array = process_data.getmemory_map()
    for line in temp_mem_array:
        print "     %s" % line


if __name__ == "__main__":
    main()
