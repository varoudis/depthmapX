import argparse
import unittest

if __name__ == "__main__":
    parser = argparse.ArgumentParser("Find and run all unittest in a folder")
    parser.add_argument('--folder', '-f', dest='folder', help='Optional folder to search, defaults to .')
    args = parser.parse_args()

    if args.folder:
        folder = args.folder
    else:
        folder = '.'
    loader = unittest.TestLoader()
    suite = loader.discover(folder)
    runner = unittest.TextTestRunner()
    result = runner.run(suite)
    if not result.wasSuccessful():
        exit(-1)
