import unittest
from sensorfleet import Fleet

class FleetTests(unittest.TestCase):
    def test_aggregate(self):
        f = Fleet()
        f.register("sensor1", 10, 20)
        self.assertEqual(f.summary("sensor1")["count"], 0)
        self.assertEqual(f.ingest("sensor1", 15, "2026-01-01T00:00:00Z")["status"], "ok")
        self.assertEqual(f.ingest("sensor1", 25, "2026-01-01T00:00:01Z")["status"], "alert")
        self.assertEqual(f.summary("sensor1")["average"], 20)
        self.assertEqual(f.summary("sensor1")["alerts"], 1)
    def test_validation(self):
        f = Fleet()
        with self.assertRaises(ValueError):
            f.register("x", 3, 3)
        f.register("x", 0, 1)
        with self.assertRaises(ValueError):
            f.ingest("x", float("nan"), "2026-01-01T00:00:00Z")
        f.ingest("x", 0, "2026-01-01T00:00:00Z")
        with self.assertRaises(ValueError):
            f.ingest("x", 0, "2026-01-01T00:00:00Z")
