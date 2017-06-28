import os
import shutil
import subprocess
import platform

def runTest():
   pass 

class cd:
    """Context manager for changing the current working directory"""
    def __init__(self, newPath):
        self.newPath = os.path.expanduser(newPath)

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)


def prepareDirectory(dirname):
    if os.path.exists( dirname ):
        shutil.rmtree(dirname)
    os.makedirs(dirname)

def runExecutable( workingDir, arguments ):
    """ Prepares a clean run directoy and runs the process in this """
    with cd(workingDir):
        with open("out.txt", "w") as outfile:
            result = subprocess.run(arguments, stdout = outfile, stderr = subprocess.STDOUT )
        output = ""
        if os.path.exists( "out.txt"):
            with open( "out.txt", "r" ) as f:
                output = f.read()
        if os.path.exists( "err.txt" ):
            with open( "err.txt", "r") as f:
                error = f.read();
        return (result.returncode == 0, output)

def getExecutable(basedir):
    sys = platform.system()
    if sys == "Windows":
        return os.path.join(basedir, sys, "depthmapXcli.exe")
    else:
        return os.path.join(basedir, sys, "depthmapXcli")

def getTestExecutable(basedir):
    sys = platform.system()
    if sys == "Windows":
        return os.path.join(basedir, "depthmapXcli", "release", "depthmapXcli.exe")
    else:
        return os.path.join(basedir, "depthmapXcli", "depthmapXcli")


