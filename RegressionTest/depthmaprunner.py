import os.path
import cmdlinewrapper
import difflib

class DepthmapRunner():
    def __init__(self, runFunc, binary ):
        self.__runFunc = runFunc
        self.__binary = binary
        
    def runDepthmap(self, cmdWrapper, runDir):
        args = [self.__binary]
        args.extend(cmdWrapper.toCmdArray())
        return self.__runFunc(runDir, args)

def diffBinaryFiles(file1, file2):
    with open(file1, "rb") as f:
        content1 = f.read()
    with open(file2, "rb") as f:
        content2 = f.read()
    gen = difflib.diff_bytes(difflib.unified_diff, [content1], [content2])
    return not(list(gen))

class DepthmapRegressionRunner():
    def __init__(self, runFunc, baseBinary, testBinary, workingDir):
        self.__baseRunner = DepthmapRunner(runFunc, baseBinary)
        self.__testRunner = DepthmapRunner(runFunc, testBinary)
        self.__workingDir = workingDir

    def runTestCase(self, name, infile, outfile, mode, simpleMode = False, subcmds = []):
        cmd = commandlinewrapper.DepthmapCmd()
        cmd.infile = infile
        cmd.outfile = outfile
        cmd.mode = mode
        cmd.simpleMode = simpleMode
        cmd.modeLines = subcmds

        baseDir = os.path.join(self.__workingDir, name + "_base")
        (baseSuccess, baseOut) = self.__baseRunner.runDepthmap(cmd, baseDir)
        if not baseSuccess:
            print("Baseline run failed with arguments " + cmd.toCmdArray())
            print(baseOut)
            return False
        testDir = os.path.join(self.__workingDir, name + "_test")
        (testSuccess, testOut) = self.__testRunner(cmd, testDir)
        if not testSuccess:
            print("Test run failed with arguments " + cmd.toCmdArray())
            print(testOut)
            return false

        baseFile = os.path.join(baseDir, outfile)
        testFile = os.path.join(testDir, outfile)
        if not os.path.exist(baseFile):
            print( "Baseline output {0} does not exist".format(baseFile))
            return False
        if not os.path.exist(testFile):
            print( "Test output {0} does not exist".format(testFile))
            return False
        

        return True


