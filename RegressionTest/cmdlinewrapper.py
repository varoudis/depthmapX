class CommandLineError(Exception):
    def __init__(self, message):
        self.message = message
    

class DepthmapCmd():
    infile = None
    outfile = None
    simpleMode = False
    mode = None
    modeLines = []


    def toCmdArray(self):
        if self.infile == None:
            raise CommandLineError("infile must be defined")
        if self.outfile == None:
            raise CommandLineError("outfile must be defined")
        if self.mode == None:
            raise CommandLineError("mode must be defined")
        args = ["-f", self.infile, "-o", self.outfile, "-m", self.mode]
        if self.simpleMode:
            args.append("-s")
        
        for modeLine in self.modeLines:
            args.extend(modeLine.toCmdArray())

        return args    

class VisibilityCmd():
    visibilityMode = None
    globalMeasures = False
    localMeasures = False
    radius = None

    def toCmdArray(self):
        if self.visibilityMode == None:
            raise CommandLineError("visibility mode must be defined")
        args = ["-vm", self.visibilityMode ]
        if self.globalMeasures:
            args.append("-vg")
        if self.localMeasures:
            args.append("-vl")
        if not self.radius == None:
            args.extend(["-vr", self.radius])
        return args
