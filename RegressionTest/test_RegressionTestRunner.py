import unittest
from test_config import writeConfig
from RegressionTestRunner import RegressionTestRunner
from disposablefile import DisposableFile, DisposableDirectory
import os

class TestRegressionTestRunner(unittest.TestCase):
    def runfunc(self, workingdir, args):
        if self.counter >= 0:
            self.counter = self.counter - 1
        if self.counter == 0:
            return (False, "Failed")
        if not os.path.exists(workingdir):
            os.makedirs(workingdir)
        with open(os.path.join(workingdir, "outfile.graph"), "w") as f:
            f.write("123")
        with open(os.path.join(workingdir, "runtimes.csv"), "w") as f:
            f.write("action,duration\n")
        return (True, "")    
        

    def test_RegressionTestRunnerAllGoesWell(self):
        with DisposableFile("testconfig.json") as f, DisposableDirectory("testrundir") as d:
            writeConfig(f.filename(), d.name())
            runner = RegressionTestRunner(f.filename(), lambda w, a: self.runfunc(w, a))
            self.counter = -1
            self.assertTrue(runner.run())

    def test_RegressionTestRunnerOneRunFails(self):
        with DisposableFile("testconfig.json") as f, DisposableDirectory("testrundir") as d:
            writeConfig(f.filename(), d.name())
            runner = RegressionTestRunner(f.filename(), lambda w, a: self.runfunc(w, a))
            self.counter = 2
            self.assertFalse(runner.run())

if __name__ == "__main__":
    unittest.main()
