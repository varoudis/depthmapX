import json
import os.path
import cmdlinewrapper
from performanceregressionconfig import PerformanceRegressionConfig


class ConfigError(Exception):
    def __init__(self, message):
        self.message = message

def buildSubCmd( subType, subConfig ):
    if subType == "Visibility":
        scmd = cmdlinewrapper.VisibilityCmd()
        scmd.visibilityMode = subConfig["visibilityMode"]
        if "globalMeasures" in subConfig:
            scmd.globalMeasures = True
        if "localMeasures" in subConfig:
            scmd.localMeasures = True
        if "radius" in subConfig:
            scmd.radius = subConfig["radius"]
        return scmd;
    elif subType == "Link":
        scmd = cmdlinewrapper.LinkCmd()
        if "linksFile" in subConfig:
            scmd.linksFile = subConfig["linksFile"]
        if "manualLinks" in subConfig:
            scmd.manualLinks = subConfig["manualLinks"]
        return scmd;
    else:
        raise ConfigError("Unknown sub commandline config " + subType)
    
def buildCmd(testcase):
    cmd = cmdlinewrapper.DepthmapCmd()
    cmd.infile = testcase["infile"]
    cmd.outfile = testcase["outfile"]
    cmd.mode = testcase["mode"]
    if "simple" in testcase and not testcase["simple"]  == "false":
        cmd.simpleMode = True
    if "subcmds" in testcase:
        for (key, value) in testcase["subcmds"].items():
            cmd.modeLines.append(buildSubCmd(key, value))
    return cmd

class TestCase():
    def __init__(self, cmd):
        self.cmd = cmd

def buildTestcase(testcase, rundir, configdir):
    return TestCase(buildCmd(testcase)) 


        
    

class RegressionConfig():
    def __init__(self, filename):
        with open(filename, "r") as f:
            config = json.load(f)
        configdir = os.path.dirname(filename)
        self.rundir = config["rundir"]
        self.basebinlocation = config["basebinlocation"]
        self.testbinlocation = config["testbinlocation"]
        self.performanceRegression = PerformanceRegressionConfig(config.get("performance", None))
        self.testcases = {}
        for (name, tc) in config["testcases"].items():
            self.testcases[name] = buildTestcase(tc, self.rundir, configdir)


