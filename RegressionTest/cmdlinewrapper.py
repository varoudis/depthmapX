class CommandLineError(Exception):
    def __init__(self, message):
        self.message = message
    

class DepthmapCmd():
    def __init__(self):
        self.infile = None
        self.outfile = None
        self.simpleMode = False
        self.mode = None
        self.extraArgs = {}
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
        
        for key, value in self.extraArgs.items():
            if isinstance(value, list):
                for v in value:
                    args.append(key)
                    args.append(v)
            else:
                args.append(key)
                if value:
                    args.append(value)

        return args    

