import unittest

class TestFailure(unittest.TestCase):
    """
        Test case that always fails to test that we capture failures correctly
    """
    def test_fail_this(self):
        print("This will always fail and should only be used in tests of the unittest framework")
        self.assertTrue(False)


if __name__=="__main__":
    unittest.main()
