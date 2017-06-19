import unittest
from context import config
from disposablefile import DisposableFile
import RegressionTestRunner
import os

def writeConfig(filename, rundir):
    with open(filename, "w") as f:
        f.write('{ "rundir": "'+ rundir +'",\n')
        f.write('   "basebinlocation": "../baselineBinaries",\n')
        f.write('   "testbinlocation": "../..",\n')
        f.write('  "testcases": {\n')
        f.write('      "test1": {\n')
        f.write('           "infile": "infile.graph",\n')
        f.write('           "outfile": "outfile.graph",\n')
        f.write('           "mode": "VGA",\n')
        f.write('           "subcmds": {\n')
        f.write('                "Visibility": {\n')
        f.write('                     "visibilityMode": "metric",\n')
        f.write('                     "radius": "7"}}}}}')
    


class TestMethods(unittest.TestCase):
    def test_buildCmd(self):
        data = { "infile": "foo.graph", "outfile": "bar.graph", "mode": "VGA", "subcmds": { "Visibility" : {"visibilityMode": "visibility", "globalMeasures": "1", "radius": "5"}}}
        cmd = config.buildCmd( data )
        self.assertEqual(cmd.infile, "foo.graph")
        self.assertEqual(cmd.outfile, "bar.graph")
        self.assertEqual(cmd.mode, "VGA")
        self.assertEqual(len(cmd.modeLines), 1)


    def test_buildSubCmd(self):
        data = { "visibilityMode": "visibility", "globalMeasures": "1", "radius": "5"}
        cmd = config.buildSubCmd("Visibility", data)
        self.assertEqual(cmd.visibilityMode, "visibility")
        self.assertTrue(cmd.globalMeasures)
        self.assertFalse(cmd.localMeasures)
        self.assertEqual(cmd.radius, "5")


    def test_buildSubCmdInvalid(self):
        data = { "visibilityMode": "visibility", "globalMeasures": "1", "radius": "5"}
        with self.assertRaises(config.ConfigError) as cm:
            config.buildSubCmd("foobar", data)
        self.assertEqual(cm.exception.message, "Unknown sub commandline config foobar")    

    def test_configClass(self):
        with DisposableFile("test.config") as testfile:
            writeConfig(testfile.filename(), "../foo/bar")
            conf = config.RegressionConfig(testfile.filename())
            self.assertEqual(len(conf.testcases), 1)
            self.assertEqual(conf.rundir, "../foo/bar")
            self.assertTrue("test1" in conf.testcases)


class TestRealConfig(unittest.TestCase):
    def test_realConfig(self):
        configFile = os.path.join("..", RegressionTestRunner.defaultConfigFile)
        self.assertNotEqual( configFile, "" )
        conf = config.RegressionConfig(configFile)

if __name__ == "__main__":
    unittest.main()
