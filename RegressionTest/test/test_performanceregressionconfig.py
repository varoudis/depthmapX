import unittest
from context import performanceregressionconfig
import disposablefile


class TestDisabledConfig(unittest.TestCase):
    def test_missingConfig(self):
        p = performanceregressionconfig.PerformanceRegressionConfig(None)
        self.assertFalse(p.enabled)

    def test_disabledConfig(self):
        p = performanceregressionconfig.PerformanceRegressionConfig({"enabled": "0"})
        self.assertFalse(p.enabled)
        
        p = performanceregressionconfig.PerformanceRegressionConfig({"enabled": "False"})
        self.assertFalse(p.enabled)

        p = performanceregressionconfig.PerformanceRegressionConfig({"enabled": "True"})
        self.assertTrue(p.enabled)

class TestSuccessfulConfig(unittest.TestCase):
    def test_defaultValues(self):
        p = performanceregressionconfig.PerformanceRegressionConfig({})
        self.assertTrue(p.enabled)
        self.assertEqual(p.runsPerInstance, 3)
        self.assertEqual(p.relativeThresholdInPercent, 1)
        self.assertEqual(p.absoluteThresholdInSeconds, 1)
        
    def test_overrideValues(self):
        p = performanceregressionconfig.PerformanceRegressionConfig(
            { "runsPerInstance": "5",
              "relativeThresholdInPercent": 1.5,
              "absoluteThresholdInSeconds": "4.1"
                })
        self.assertTrue(p.enabled)
        self.assertEqual(p.runsPerInstance, 5)
        self.assertEqual(p.relativeThresholdInPercent, 1.5)
        self.assertEqual(p.absoluteThresholdInSeconds, 4.1)
        


if __name__ == "__main__":
    unittest.main()
