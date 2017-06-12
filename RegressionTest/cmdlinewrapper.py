class CommandLineError(Exception):
    def __init__(self, message):
        self.message = message
    

class DepthmapCmd():
    def __init__(self):
        self.infile = None
        self.outfile = None
        self.simpleMode = False
        self.mode = None
        self.modeLines = []
        self.timingFile = None


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

        if self.timingFile:
            args.extend(["-t", self.timingFile])
        
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
        
class LinkCmd():
    linksFile = None
    manualLinks = None

    def toCmdArray(self):
        if self.linksFile == None and self.manualLinks == None:
            raise CommandLineError("links must be given as a file or each one manually")
        if self.linksFile != None and self.manualLinks != None:
            raise CommandLineError("links must be given as a file or each one manually")
        args = [];
        if self.manualLinks:
            for manualLink in self.manualLinks:
                args.extend(["-lnk", manualLink])
        if self.linksFile:
            args.extend(["-lf", self.linksFile]);
        return args
