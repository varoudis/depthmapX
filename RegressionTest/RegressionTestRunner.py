import runhelpers
import config
import depthmaprunner
import os

defaultConfigFile = "regressionconfig.json"

class RegressionTestRunner():
    def __init__(self, configfile, runfunc):
        self.config = config.RegressionConfig(configfile)
        self.testBinary = runhelpers.getExecutable(self.config.testbinlocation)
        self.baseBinary = runhelpers.getExecutable(self.config.basebinlocation)
        self.runfunc = runfunc

    def run(self):
        if not os.path.exists(self.config.rundir):
            os.makedirs(self.config.rundir)
        runner = depthmaprunner.DepthmapRegressionRunner( self.runfunc, self.baseBinary, self.testBinary, self.config.rundir )
        results = {}
        for name, case in self.config.testcases.items():
            results[name] = runner.runTestCase(name, case.cmd.infile, case.cmd.outfile, case.cmd.mode, case.cmd.simpleMode, case.cmd.modeLines)
            
        good = True    
        for name, result in results.items():
            if not result[0]:
                good = False
                print( name + " failed: " + result[1])
            else:
                print(name + " ok")
        return good        

if __name__ == "__main__":
    r = RegressionTestRunner(defaultConfigFile, runhelpers.runExecutable)
    if not r.run():
        exit(-1)

