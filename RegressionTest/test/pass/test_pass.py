import unittest

class TestFailure(unittest.TestCase):
    """
        Test case that always passes to test that we capture failures correctly
    """
    def test_fail_this(self):
        print("This will always pass and should only be used in tests of the unittest framework")
        self.assertTrue(True)


if __name__=="__main__":
    unittest.main()
