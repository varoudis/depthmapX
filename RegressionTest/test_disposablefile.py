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

if __name__=="__main__":
    unittest.main()
