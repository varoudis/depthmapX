import unittest

import collections
from context import cmdlinewrapper

class TestDepthmapCmd(unittest.TestCase):
    def test_correctBehaviour(self):
        cmd = cmdlinewrapper.DepthmapCmd()
        cmd.infile = "foo"
        cmd.outfile = "bar"
        cmd.mode = "visibility"

        self.assertEqual(cmd.toCmdArray(), ["-f", "foo", "-o", "bar", "-m", "visibility" ])
        cmd.simpleMode = True
        self.assertEqual(cmd.toCmdArray(), ["-f", "foo", "-o", "bar", "-m", "visibility", "-s" ])

    def test_exceptions(self):
        cmd = cmdlinewrapper.DepthmapCmd()
        with self.assertRaises(cmdlinewrapper.CommandLineError) as cm:
            cmd.toCmdArray()
        self.assertEqual(cm.exception.message, "infile must be defined")    
        cmd.infile = "foo"
        with self.assertRaises(cmdlinewrapper.CommandLineError) as cm:
            cmd.toCmdArray()
        self.assertEqual(cm.exception.message, "outfile must be defined")    
        cmd.outfile = "bar"
        with self.assertRaises(cmdlinewrapper.CommandLineError) as cm:
            cmd.toCmdArray()
        self.assertEqual(cm.exception.message, "mode must be defined")    

    def test_extraArgs(self):
        cmd = cmdlinewrapper.DepthmapCmd()
        cmd.infile = "foo"
        cmd.outfile = "bar"
        cmd.mode = "visibility"
        # use ordered dict here for testability
        cmd.extraArgs = collections.OrderedDict([("-lnk", ["foo", "bar"]), ("-vm", "metric"), ("-vg", "" )])

        self.assertEqual(cmd.toCmdArray(), ["-f", "foo", "-o", "bar", "-m", "visibility", "-lnk", "foo", "-lnk", "bar", "-vm", "metric", "-vg" ])

            
if __name__ == "__main__":
    unittest.main()


