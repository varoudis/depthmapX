import os
import shutil
import subprocess

runDir = "./rundir"

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
    print(dirname)
    if os.path.exists( dirname ):
        shutil.rmtree(dirname)
    os.makedirs(dirname)

def runExecutable( workingDir, arguments ):
    """ Prepares a clean run directoy and runs the process in this """
    prepareDirectory(workingDir)
    with cd(workingDir):
        print(arguments)
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

if __name__ == "__main__":
    prepareDirectory(runDir)
    (success, output) = runExecutable(runDir, ["c:\\Msys64\\usr\\bin\\ls.exe"])
    print(success)
    print("output:")
    print(output)


