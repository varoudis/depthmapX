import depthmaprunner
from cmdlinewrapper import DepthmapCmd
import unittest
from disposablefile import DisposableFile

class BinaryDiffTest(unittest.TestCase):
    def test_binaryDiff(self):
        with DisposableFile( "testfile1.bin" ) as f1, DisposableFile( "testfile2.bin") as f2:
            with open( f1.filename(), "wb" ) as f:
                f.write( bytes.fromhex( "2000 0839") )
            with open( f2.filename(), "wb" ) as f:
                f.write( bytes.fromhex( "2000 0838" ))
            self.assertTrue(depthmaprunner.diffBinaryFiles(f1.filename(), f1.filename()))
            self.assertFalse(depthmaprunner.diffBinaryFiles(f1.filename(), f2.filename()))


    

class DepthmapRunnerTest(unittest.TestCase):
    def runfunc(self, rundir, args):
        self.assertEqual(rundir, self.__rundir)
        self.assertEqual(args, self.__args)

    def testDepthmapRunner(self):
        cmd = DepthmapCmd()
        cmd.infile = "foo"
        cmd.outfile = "bar"
        cmd.mode = "mode"

        runner = depthmaprunner.DepthmapRunner(lambda d, a: self.runfunc(d, a), "bin")
        
        self.__args = ["bin", "-f", "foo", "-o", "bar", "-m", "mode"]
        self.__rundir = "dir"
        runner.runDepthmap(cmd, "dir")

class DepthmapRegressioRunnerTest(unittest.TestCase):
    def runfuncSucceedAlwaysSame(self, rundir, args):
        os.makedirs(rundir)
        outputfile = None
        for i in range(0, len(args)):
            if args[i] == "-o" && i < len(args):
                outputfile = args[i+1]
                break
        self.assertFalse( outputfile == None )
        outpath = os.path.join(rundir, outputfile)
        with open (outpath, "w") as f:
            f.write("123")
       return (True, "")

    def TestSuccessfullRun(self):
        runner = depthmaprunner.DepthmapRegressionRunner(lambda d, a: self.runfuncSucceedAlwaysSame(d,a), "basebin", "testbin", "testdir")

if __name__=="__main__":
    unittest.main()
