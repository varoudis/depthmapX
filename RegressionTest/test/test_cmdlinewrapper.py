import unittest
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

    def test_subcmd(self):
        cmd = cmdlinewrapper.DepthmapCmd()
        cmd.infile = "foo"
        cmd.outfile = "bar"
        cmd.mode = "visibility"

        vis = cmdlinewrapper.VisibilityCmd()
        vis.visibilityMode = "metric"
        cmd.modeLines.append(vis)

        self.assertEqual(cmd.toCmdArray(), ["-f", "foo", "-o", "bar", "-m", "visibility", "-vm", "metric" ])

            
class TestVisibiltyCmd(unittest.TestCase):
    def test_correctBehaviour(self):
        cmd = cmdlinewrapper.VisibilityCmd()
        cmd.visibilityMode = "isovist"
        self.assertEqual(cmd.toCmdArray(), ["-vm", "isovist"])
        cmd.radius = "5"
        self.assertEqual(cmd.toCmdArray(), ["-vm", "isovist", "-vr", "5"])
        cmd.globalMeasures = True;
        self.assertEqual(cmd.toCmdArray(), ["-vm", "isovist", "-vg", "-vr", "5"])
        cmd.localMeasures = True;
        self.assertEqual(cmd.toCmdArray(), ["-vm", "isovist", "-vg", "-vl", "-vr", "5"])



if __name__ == "__main__":
    unittest.main()


