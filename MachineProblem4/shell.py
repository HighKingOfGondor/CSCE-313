import os
import pipes
import sys
import pty
import signal
import shlex
import subprocess
import datetime
import getpass
# * Allows users to define custom prompts using a simple command to change the string.
# * The following set of macros will be replaced as follows:
#
#User Definable Custom Prompt:
#   - DIR: Current directory
#   - WHO: User's name
#   - DATE: Current date
#   - TIME: Current time

RUN = 1
STOP = 0
commands = {}
r, w = os.pipe()

def shell_loop():
    status = RUN
    while status == RUN:
        cwd = os.getcwd()
        #displaying command prompt with the current directory
        sys.stdout.write(cwd + ": ")
        sys.stdout.flush()
        #cmd is the user input
        cmd = sys.stdin.readline()
        #tokenize the input to get commands
        tokens = tokenize(cmd)
        status = execute(tokens)

def tokenize(cmd):
    return shlex.split(cmd)

def execute(tokens):

    input_command = tokens[0]
    input_string = tokens[1:]
    if input_command in commands:
        return commands[input_command](input_string)
    for i in tokens:
        if i == ">":
            return redirect_into(tokens)
        elif i == "<":
            return redirect_out(tokens)
        elif i == "|":
            string = ' '.join(tokens)
            lis = string.split(' | ')
            return pipe(lis)
    #split the process to save the parent
    pid = os.fork()
    if pid == 0:
        os.execvp(tokens[0], tokens)
    else:
        while True:
            #this will get updates on the litl' kiddo
            wait_pid, child_status = os.waitpid(pid, os.WNOHANG)
            #if the parent noctices something wrong, it breaks the wait status
            if os.WIFEXITED(child_status) or os.WIFSIGNALED(child_status):
                break

    return RUN

def redirect_into(args):
    pid = os.fork()
    if pid == 0:
        j = 0
        file_name = ""
        cmd = []
        for i in args:
            if i == ">":
                file_name = args[j+1]
                break
            else:
                cmd.append(i)
                j += 1
        old = os.dup(1)
        os.close(1)
        os.open(file_name, os.O_WRONLY | os.O_CREAT)  # should open on 1
        os.execvp(cmd[0], cmd)
        os.close(1)
        os.dup(old)  # should dup to 1
        os.close(old)  # get rid of left overs
    else:
        while True:
            #this will get updates on the litl' kiddo
            wait_pid, child_status = os.waitpid(pid, os.WNOHANG)
            #if the parent noctices something wrong, it breaks the wait status
            if os.WIFEXITED(child_status) or os.WIFSIGNALED(child_status):
                break
    return RUN



def redirect_out(args):
    pid = os.fork()
    if pid == 0:
        j = 0
        file_name = ""
        cmd = []
        for i in args:
            if i == "<":
                file_name = args[j+1]
                break
            else:
                cmd.append(i)
                j += 1
        old = os.dup(0)
        os.close(0)
        os.open(file_name, os.O_RDONLY)  # should open on 1
        os.execvp(cmd[0], cmd)
        os.close(0)
        os.dup(old)  # should dup to 1
        os.close(old)  # get rid of left overs
    else:
        while True:
            #this will get updates on the litl' kiddo
            wait_pid, child_status = os.waitpid(pid, os.WNOHANG)
            #if the parent noctices something wrong, it breaks the wait status
            if os.WIFEXITED(child_status) or os.WIFSIGNALED(child_status):
                break
    return RUN

def pipe(args):
    #if there is one pipe ("ls | cat")
    if len(args) == 2:
        arg1 = args[0]
        list1 = arg1.split(' ')
        arg2 = args[1]
        list2  = arg2.split(' ')
        one = subprocess.Popen(list1, stdout=subprocess.PIPE,)
        two = subprocess.Popen(list2, stdin=one.stdout, stdout=subprocess.PIPE,)
        end_of_pipe = two.stdout
        for line in end_of_pipe:
            print '\t', line.strip()
    #if there are 2 pipes ("ls | cat | cat")
    if len(args) == 3:
        arg1 = args[0]
        list1 = arg1.split(' ')
        arg2 = args[1]
        list2  = arg2.split(' ')
        arg3 = args[2]
        list3 = arg3.split(' ')
        one = subprocess.Popen(list1, stdout=subprocess.PIPE,)
        two = subprocess.Popen(list2, stdin=one.stdout, stdout=subprocess.PIPE,)
        three = subprocess.Popen(list3, stdin=two.stdout, stdout=subprocess.PIPE,)
        end_of_pipe = three.stdout
        for line in end_of_pipe:
            print '\t', line.strip()
    #if there are 3 pipes ("ls | cat | cat | cat")
    if len(args) == 4:
        arg1 = args[0]
        list1 = arg1.split(' ')
        arg2 = args[1]
        list2  = arg2.split(' ')
        arg3 = args[2]
        list3 = arg3.split(' ')
        arg4 = args[3]
        list4 = arg4.split(' ')
        one = subprocess.Popen(list1, stdout=subprocess.PIPE,)
        two = subprocess.Popen(list2, stdin=one.stdout, stdout=subprocess.PIPE,)
        three = subprocess.Popen(list3, stdin=two.stdout, stdout=subprocess.PIPE,)
        four = subprocess.Popen(list4, stdin=three.stdout, stdout=subprocess.PIPE,)
        end_of_pipe = four.stdout
        for line in end_of_pipe:
            print '\t', line.strip()

    if len(args) == 5:
        arg1 = args[0]
        list1 = arg1.split(' ')
        arg2 = args[1]
        list2  = arg2.split(' ')
        arg3 = args[2]
        list3 = arg3.split(' ')
        arg4 = args[3]
        list4 = arg4.split(' ')
        arg5 = args[4]
        list5 = arg5.split(' ')
        one = subprocess.Popen(list1, stdout=subprocess.PIPE,)
        two = subprocess.Popen(list2, stdin=one.stdout, stdout=subprocess.PIPE,)
        three = subprocess.Popen(list3, stdin=two.stdout, stdout=subprocess.PIPE,)
        four = subprocess.Popen(list4, stdin=three.stdout, stdout=subprocess.PIPE,)
        five = subprocess.Popen(list5, stdin=four.stdout, stdout=subprocess.PIPE,)
        end_of_pipe = five.stdout
        for line in end_of_pipe:
            print '\t', line.strip()

    if len(args) == 6:
        arg1 = args[0]
        list1 = arg1.split(' ')
        arg2 = args[1]
        list2  = arg2.split(' ')
        arg3 = args[2]
        list3 = arg3.split(' ')
        arg4 = args[3]
        list4 = arg4.split(' ')
        arg5 = args[4]
        list5 = arg5.split(' ')
        arg6 = args[5]
        list6 = arg6.split(' ')
        one = subprocess.Popen(list1, stdout=subprocess.PIPE,)
        two = subprocess.Popen(list2, stdin=one.stdout, stdout=subprocess.PIPE,)
        three = subprocess.Popen(list3, stdin=two.stdout, stdout=subprocess.PIPE,)
        four = subprocess.Popen(list4, stdin=three.stdout, stdout=subprocess.PIPE,)
        five = subprocess.Popen(list5, stdin=four.stdout, stdout=subprocess.PIPE,)
        six = subprocess.Popen(list6, stdin=five.stdout, stdout=subprocess.PIPE,)
        end_of_pipe = six.stdout
        for line in end_of_pipe:
            print '\t', line.strip()
    return RUN

def getCWD():
    print os.getcwd()

def getTime():
    now = datetime.datetime.now()
    specific = ""
    if now.hour > 12:
        hour = now.hour - 12
        specific = " pm"
        if now.hour == 24:
            specific = " am"
    if now.hour == 12:
        hour = now.hour
        specific = " pm"
    if now.hour < 12:
        hour = now.hour
        specific = " am"
    print str(hour) + ":" + str(now.minute) + ":" + str(now.second) + specific

def getDate():
    now = datetime.datetime.now()
    print str(now.month) + "/" + str(now.day) + "/" + str(now.year)

def getUser():
    print getpass.getuser()

def cd(args):
    os.chdir(args[0])
    return RUN

def register(name, function):
    commands[name] = function

def exit(args):
    return STOP

def init():
    register("cd", cd)
    register("exit", exit)

def main():
    init()
    shell_loop()

if __name__ == "__main__":
    main()