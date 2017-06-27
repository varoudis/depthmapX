import os.path
import cmdlinewrapper
import difflib
import pprint
import csv

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

def checkPerformance(baseFile, testFile):
    """ 
    Check the performance of 2 depthmap runs against each other
    This function expects the timing from a base and test run and  parses them
    as CSV. For now, it expects the entries to be the same. It will return an
    error message if
    * one or both of the files are missing
    * the number of lines or the labels don't match
    * the test run is more than 5 seconds or 5% slower than the baseline
      (whatever is greater)
    """
    if not os.path.exists(baseFile):
        return "Base performance timing file {0} is missing".format(baseFile)
    if not os.path.exists(testFile):
        return "Test performance timing file {0} is missing".format(testFile)
    with open(baseFile) as baseHandle, open(testFile) as testHandle:
        baseReader = csv.DictReader(baseHandle)
        testReader = csv.DictReader(testHandle)

        baseDone = False
        testDone = False
        
        while True:
            try:
                baseLine = next(baseReader)
            except StopIteration:
                baseDone = True

            try:
                testLine = next(testReader)
            except StopIteration:
                testDone = True

            if baseDone and testDone:
                return ""
            if baseDone and not testDone:
                return "baseline performance file {0} has fewer lines than the test one {1}".format(baseFile, testFile)
            if testDone and not baseDone:
                return "baseline performance file {0} has more lines than the test one {1}".format(baseFile, testFile)

            if not baseLine["action"] == testLine["action"]:
                return "performance line mismatch: base '{0}', test '{1}'".format(baseLine["action"], testLine["action"])

            baseTime = float(baseLine["duration"])
            testTime = float(testLine["duration"])

            allowance = max(5.0, baseTime / 20.0 )
            if testTime > baseTime + allowance:
                return "Performance regression: {0} took {1}s instead of {2}s".format(baseLine["action"], testLine["duration"], baseLine["duration"])
            
        
    
class DepthmapRegressionRunner():
    def __init__(self, runFunc, baseBinary, testBinary, workingDir):
        self.__baseRunner = DepthmapRunner(runFunc, baseBinary)
        self.__testRunner = DepthmapRunner(runFunc, testBinary)
        self.__workingDir = workingDir

    def runTestCase(self, name, cmd):
        cmd.timingFile = "runtimes.csv"
        baseDir = os.path.join(self.__workingDir, name + "_base")
        (baseSuccess, baseOut) = self.__baseRunner.runDepthmap(cmd, baseDir)
        if not baseSuccess:
            print("Baseline run failed with arguments " + pprint.pformat(cmd.toCmdArray()))
            print(baseOut)
            return (False, "Baseline run failed")
        testDir = os.path.join(self.__workingDir, name + "_test")
        (testSuccess, testOut) = self.__testRunner.runDepthmap(cmd, testDir)
        if not testSuccess:
            print("Test run failed with arguments " + pprint.pformat(cmd.toCmdArray()))
            print(testOut)
            return (False, "Test run failed")

        baseFile = os.path.join(baseDir, cmd.outfile)
        testFile = os.path.join(testDir, cmd.outfile)
        if not os.path.exists(baseFile):
            message = "Baseline output {0} does not exist".format(baseFile)
            print (message)
            return (False, message)
        if not os.path.exists(testFile):
            message = "Test output {0} does not exist".format(testFile)
            print(message)
            return (False, message)
        
        if not diffBinaryFiles(baseFile, testFile):
            message = "Test outputs differ"
            print (message)
            return (False, message)

        message = checkPerformance( os.path.join(baseDir, cmd.timingFile), os.path.join(testDir, cmd.timingFile))
        if message:
            print(message)
            return (False,message)

        return (True, "")


