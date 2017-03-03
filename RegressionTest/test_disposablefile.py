import disposablefile
import unittest
import os.path

class TestDisposableFile(unittest.TestCase):
    def testFileDeletion(self):
        with disposablefile.DisposableFile("testfile.xyz") as f:
            self.assertEqual(f.filename(), "testfile.xyz")
            with open( f.filename(), "w") as tf:
                tf.write("foo")
            self.assertTrue( os.path.exists(f.filename()))
        self.assertFalse( os.path.exists("testfile.xyz"))


class TestDisposableDirectory(unittest.TestCase):
    def testLifetime(self):
        with disposablefile.DisposableDirectory("testdir1", True) as d:
            self.assertEqual("testdir1", d.name())
            self.assertTrue(os.path.exists(d.name()))
            self.assertTrue(os.path.isdir(d.name()))
            with open ( os.path.join(d.name(), "testfile.txt"), "w") as f:
                f.write("123")
            self.assertTrue(os.path.exists(os.path.join(d.name(), "testfile.txt")))
        self.assertFalse(os.path.exists(d.name()))

    def testNotAutomaticallyCreated(self):
        with disposablefile.DisposableDirectory("testdir1", False) as d:
            self.assertEqual("testdir1", d.name())
            self.assertFalse(os.path.exists(d.name()))
            os.makedirs(d.name())
            self.assertTrue(os.path.exists(d.name()))
            self.assertTrue(os.path.isdir(d.name()))
            with open ( os.path.join(d.name(), "testfile.txt"), "w") as f:
                f.write("123")
            self.assertTrue(os.path.exists(os.path.join(d.name(), "testfile.txt")))
        self.assertFalse(os.path.exists(d.name()))
            
    def testNeverCreated(self):
        with disposablefile.DisposableDirectory("testdir1", False) as d:
            self.assertEqual("testdir1", d.name())
            self.assertFalse(os.path.exists(d.name()))
        self.assertFalse(os.path.exists(d.name()))
        
    def testExceptions(self):
        with self.assertRaises(disposablefile.DisposableDirectoryError) as cm:
            disposablefile.DisposableDirectory(".")
        self.assertEqual(cm.exception.message,"The disposable directory cannot be the current directory" )
        with disposablefile.DisposableDirectory("testdir1", True) as d:
            with self.assertRaises(disposablefile.DisposableDirectoryError) as cm:
                disposablefile.DisposableDirectory(d.name())
            self.assertEqual( cm.exception.message, "You can't make an existing directory disposable" )


if __name__=="__main__":
    unittest.main()
