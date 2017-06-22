import csv

import os
import unittest
from disposablefile import DisposableFile, DisposableDirectory
from context import performancerunner

class PerformanceCheckTest(unittest.TestCase):
    def test_filesMissing(self):
        message = performancerunner.checkPerformance("f1.csv", "f2.csv")
        self.assertEqual(message, "Base performance timing file f1.csv is missing")

        with DisposableFile("f1.csv") as f1:
            with open(f1.filename(), "w") as f:
                f.write("action,duration\n")
            message = performancerunner.checkPerformance("f1.csv", "f2.csv")
            self.assertEqual(message, "Test performance timing file f2.csv is missing")

        with DisposableFile("f2.csv") as f2:
            with open(f2.filename(), "w") as f:
                f.write("action,duration\n")
            message = performancerunner.checkPerformance("f1.csv", "f2.csv")
            self.assertEqual(message, "Base performance timing file f1.csv is missing")

    def test_fileLineNumberMismatch(self):
        with DisposableFile("f1.csv") as f1, DisposableFile("f2.csv") as f2:
            with open(f1.filename(), "w") as f:
                f.write("action,duration\nfoo,10\n")
            with open(f2.filename(), "w") as f:
                f.write("action,duration\n")
            message = performancerunner.checkPerformance("f1.csv", "f2.csv")
            self.assertEqual(message, "baseline performance file f1.csv has more lines than the test one f2.csv")

        with DisposableFile("f1.csv") as f1, DisposableFile("f2.csv") as f2:
            with open(f1.filename(), "w") as f:
                f.write("action,duration\nfoo,10\n")
            with open(f2.filename(), "w") as f:
                f.write("action,duration\nfoo,10\nbar,20\n")
            message = performancerunner.checkPerformance("f1.csv", "f2.csv")
            self.assertEqual(message, "baseline performance file f1.csv has fewer lines than the test one f2.csv")

    def test_fileLabelMismatch(self):
        with DisposableFile("f1.csv") as f1, DisposableFile("f2.csv") as f2:
            with open(f1.filename(), "w") as f:
                f.write("action,duration\nfoo,10\n")
            with open(f2.filename(), "w") as f:
                f.write("action,duration\nbar,10\n")
            message = performancerunner.checkPerformance("f1.csv", "f2.csv")
            self.assertEqual(message, "performance line mismatch: base 'foo', test 'bar'")

    def test_successfulRunEmptyFile(self):
        with DisposableFile("f1.csv") as f1, DisposableFile("f2.csv") as f2:
            with open(f1.filename(), "w") as f:
                f.write("action,duration\n")
            with open(f2.filename(), "w") as f:
                f.write("action,duration\n")
            message = performancerunner.checkPerformance("f1.csv", "f2.csv")
            self.assertEqual(message, "")

    def test_successfulRun(self):
        with DisposableFile("f1.csv") as f1, DisposableFile("f2.csv") as f2:
            with open(f1.filename(), "w") as f:
                f.write("action,duration\nfoo,10\nbar,21\nbaz,1000\nblub,10\n")
            with open(f2.filename(), "w") as f:
                f.write("action,duration\nfoo,10.9\nbar,10\nbaz,1010\nblub,10\n")
            message = performancerunner.checkPerformance("f1.csv", "f2.csv")
            self.assertEqual(message, "")

    def test_performanceRegression(self):
        with DisposableFile("f1.csv") as f1, DisposableFile("f2.csv") as f2:
            with open(f1.filename(), "w") as f:
                f.write("action,duration\nfoo,10\n")
            with open(f2.filename(), "w") as f:
                f.write("action,duration\nfoo,18\n")
            message = performancerunner.checkPerformance("f1.csv", "f2.csv")
            self.assertEqual(message, "Performance regression: foo took 18s instead of 10s")

class test_PerformanceAggregation(unittest.TestCase):
    def test_aggregation(self):
        with DisposableDirectory("testdir") as d:
            os.makedirs(d.name())
            nameTemplate = "test{0}.csv"
            with open(os.path.join(d.name(), nameTemplate.format(0)), "w") as f:
                f.write("action,duration\nt1,3\nt2,2\n")
            with open(os.path.join(d.name(), nameTemplate.format(1)), "w") as f:
                f.write("action,duration\nt1,1.5\nt2,2.5\n")
            with open(os.path.join(d.name(), nameTemplate.format(2)), "w") as f:
                f.write("action,duration\nt1,2\nt2,1.5\n")

            resFile = performancerunner.aggregatePerformanceStats(d.name(), 3, nameTemplate)
            with open(resFile, "r") as f:
                reader = csv.DictReader(f)
                line = next(reader)
                self.assertEqual(line["action"], "t1")
                self.assertEqual(float(line["max"]), 3 )
                self.assertEqual(float(line["min"]), 1.5)
                self.assertEqual(float(line["average"]), 6.5/3)

                line = next(reader)
                self.assertEqual(line["action"], "t2")
                self.assertEqual(float(line["max"]), 2.5 )
                self.assertEqual(float(line["min"]), 1.5)
                self.assertEqual(float(line["average"]), 2)

                line = next(reader)
                self.assertEqual(line["action"], "total")
                self.assertEqual(float(line["max"]), 5 )
                self.assertEqual(float(line["min"]), 3.5)
                self.assertEqual(float(line["average"]), 12.5/3)
