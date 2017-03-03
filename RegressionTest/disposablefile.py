import os
import shutil

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

class DisposableDirectoryError(Exception):
    def __init__(self, message):
        self.message = message

class DisposableDirectory:
    def __init__(self, directoryName, create = False):
        if directoryName == ".":
            raise DisposableDirectoryError("The disposable directory cannot be the current directory")
        if os.path.exists(directoryName):
            raise DisposableDirectoryError("You can't make an existing directory disposable")
        self.__directoryName = directoryName
        if create:
            os.makedirs(directoryName)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, backtrace):
        if os.path.exists(self.__directoryName):
            shutil.rmtree(self.__directoryName)

    def name(self):
        return self.__directoryName
