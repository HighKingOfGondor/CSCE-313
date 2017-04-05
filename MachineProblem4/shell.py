import os
import pipes
import sys
import pty
import signal
import shlex
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
            return pipe(tokens)
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
        os.open(file_name, os.O_RDONLY)  # should open on 0
        os.execvp(cmd[0], cmd)
        os.close(0)
        os.dup(old)  # should dup to 0
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
    string=""

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
