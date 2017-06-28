import unittest
from context import runhelpers
from disposablefile import DisposableDirectory
import os
import platform
import sys

class TestRunHelpers(unittest.TestCase):
    def test_prepareDirectory(self):
        with DisposableDirectory("testdir", True) as d:
            self.assertTrue(os.path.isdir(d.name()))
            testfile = os.path.join(d.name(), "testfile.txt")
            with open(testfile, "w") as f:
                f.write("123")
            self.assertTrue(os.path.exists(testfile))
            runhelpers.prepareDirectory(d.name())
            self.assertTrue(os.path.isdir(d.name()))
            self.assertFalse(os.path.exists(testfile))

    def test_cd(self):
        currentpath = os.getcwd()
        with DisposableDirectory("testdir", True) as d:
            with runhelpers.cd("testdir"):
                self.assertEqual(os.getcwd(), os.path.join(currentpath, d.name()))
            self.assertEqual(os.getcwd(), currentpath, d.name())
                
    def test_getBinary(self):
        result = runhelpers.getExecutable("foo")
        sys = platform.system()
        if sys == "Windows":
            self.assertEqual( result, "foo\\Windows\\depthmapXcli.exe")
        else:
            self.assertEqual( result, "foo/" + sys + "/depthmapXcli" )

    def test_getTestBinary(self):
        result = runhelpers.getTestExecutable("foo")
        sys = platform.system()
        if sys == "Windows":
            self.assertEqual( result, "foo\\depthmapXcli\\release\\depthmapXcli.exe")
        else:
            self.assertEqual( result, "foo/depthmapXcli/depthmapXcli" )

    
    def test_runExecutable(self):
        with DisposableDirectory("testdir", True) as d:
            retcode, output = runhelpers.runExecutable( d.name(), [sys.executable, "-c", "print('foo')"])
            self.assertTrue(retcode)
            self.assertEqual(output, "foo\n")

    def test_runExecutableFail(self):
        with DisposableDirectory("testdir") as d:
            runhelpers.prepareDirectory(d.name())
            retcode, output = runhelpers.runExecutable( d.name(), [sys.executable, "-c", "exit(-1)"])
            self.assertFalse(retcode)
            self.assertEqual(output, "")

    def test_runExecutableException(self):
        with DisposableDirectory("testdir") as d:
            runhelpers.prepareDirectory(d.name())
            retcode, output = runhelpers.runExecutable( d.name(), [sys.executable, "-c", "raise Exception()"])
            self.assertFalse(retcode)
            self.assertEqual(output, 'Traceback (most recent call last):\n  File "<string>", line 1, in <module>\nException\n')
            
if __name__=="__main__":
    unittest.main()
