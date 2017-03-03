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
        with disposablefile.DisposableDirectory("testdir1", true) as d:
            self.assertEqual("testdir1", d.name())
            self.assertTrue(os.path.exists(d.name()))
            self.assertTrue(os.path.isdirectory(d.name()))
            with open ( os.path.join(d.name(), "testfile.txt"), "w") as f:
                f.write("123")
            self.assertTrue(os.path.exists(os.path.join(d.name(), "testfile.txt")))
        self.assertFalse(os.path.exists(d.name()))


if __name__=="__main__":
    unittest.main()
