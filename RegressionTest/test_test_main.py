import unittest
from disposablefile import DisposableDirectory
import runhelpers

class TestUnitTestMain(unittest.TestCase):
    def test_capture_pass(self):
        with DisposableDirectory("testdir_pass", True) as d:
            retcode, output = runhelpers.runExecutable( d.name(), ["python", "../test_main.py", "-f", "../pass"])
            if not retcode:
                print("printing the underlying test output to help diagnose the issue:")
                print(output)
            self.assertTrue(retcode)

    def test_capture_fail(self):
        with DisposableDirectory("testdir_fail", True) as d:
            retcode, output = runhelpers.runExecutable( d.name(), ["python", "../test_main.py", "-f", "../fail"])
            if retcode:
                print("printing the underlying test output to help diagnose the issue:")
                print(output)
            self.assertFalse(retcode)

if __name__=="__main__":
    unittest.main()
