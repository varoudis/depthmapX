

class PerformanceRegressionConfig:
    """ Encapsulate performance regression config
        This takes an optional performance regression config. All Elements
        in the performance regression config are optional. If it is None
        completly, performance regresssion will be disabled
        "peformanceRegression":
        {
            enabled = 1,   <- enabling performance regression - only required if nothing else is set, the existence of a non null config will turn on performance regression
            runsPerInstance = 3, <- how many runs of each run def to run and average over
            relativeThresholdInPercent = 1.5, <- how many percent can the new version be slower without failing
            absoluteThresholdInSeconds = 2.3, <- how many seconds the new version can be slower without failing
                                                 of the two above, breaching the lower one will lead to failure.
        }
    """
    def __init__(self, perfConfig):
        if None == perfConfig or ("enabled" in perfConfig and perfConfig["enabled"] not in ["True", "true", "1", "yes"]):
            self.enabled = False
            return
        self.enabled = True;
        self.runsPerInstance = int(perfConfig.get("runsPerInstance", 3))
        self.relativeThresholdInPercent = float(perfConfig.get("relativeThresholdInPercent", 1))
        self.absoluteThresholdInSeconds = float(perfConfig.get("absoluteThresholdInSeconds",1))
