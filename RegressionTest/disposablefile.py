import os.path

class DisposableFile:
    def __init__( self, filename ):
        self.__filename = filename

    def __enter__(self):
        return self 

    def __exit__(self, exc_type, exc_value, traceback):
        if os.path.exists(self.filename()):
            os.remove(self.filename())

    def filename(self):
        return self.__filename
