import runhelpers
import config
import depthmaprunner
import os

defaultConfigFile = "regressionconfig.json"

class RegressionTestRunner():
    def __init__(self, configfile, runfunc):
        self.config = config.RegressionConfig(configfile)
        self.testBinary = runhelpers.getTestExecutable(self.config.testbinlocation)
        print("Binary under test is " + self.testBinary)
        self.baseBinary = runhelpers.getExecutable(self.config.basebinlocation)
        print("Baseline binary is " + self.baseBinary)
        self.runfunc = runfunc

    def run(self):
        if not os.path.exists(self.config.rundir):
            os.makedirs(self.config.rundir)
        runner = depthmaprunner.DepthmapRegressionRunner( self.runfunc, self.baseBinary, self.testBinary, self.config.rundir )

        good = True
        for name, case in self.config.testcases.items():
            print("Running test case " + name)
            success, output = runner.runTestCase(name, case.cmd.infile, case.cmd.outfile, case.cmd.mode, case.cmd.simpleMode, case.cmd.modeLines)
            if not success:
                good = False
                print ("Failed:\n" + output)
            else:
                print("ok")
        return good        

if __name__ == "__main__":
    r = RegressionTestRunner(defaultConfigFile, runhelpers.runExecutable)
    if not r.run():
        exit(-1)

