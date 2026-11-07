"""Sensor telemetry validation and fleet status aggregation."""
import math
from datetime import datetime, timezone

class Fleet:
    def __init__(self):
        self.devices = {}
        self.readings = {}

    def register(self, device_id, minimum, maximum):
        if not isinstance(device_id, str) or not device_id.strip() or device_id in self.devices:
            raise ValueError("unique device ID required")
        if any(isinstance(x, bool) or not isinstance(x, (int, float)) or not math.isfinite(x) for x in (minimum, maximum)):
            raise ValueError("bounds must be finite numbers")
        if minimum >= maximum:
            raise ValueError("minimum must be below maximum")
        self.devices[device_id] = (minimum, maximum)
        self.readings[device_id] = []

    def ingest(self, device_id, value, timestamp):
        if device_id not in self.devices:
            raise ValueError("unknown device")
        if isinstance(value, bool) or not isinstance(value, (int, float)) or not math.isfinite(value):
            raise ValueError("reading must be finite number")
        if not isinstance(timestamp, str):
            raise ValueError("timestamp required")
        try:
            parsed = datetime.fromisoformat(timestamp.replace("Z", "+00:00"))
        except ValueError as exc:
            raise ValueError("invalid timestamp") from exc
        if parsed.tzinfo is None:
            raise ValueError("timezone required")
        utc = parsed.astimezone(timezone.utc)
        history = self.readings[device_id]
        if history and utc <= history[-1][0]:
            raise ValueError("timestamps must increase per device")
        history.append((utc, float(value)))
        low, high = self.devices[device_id]
        return {"device": device_id, "status": "alert" if not low <= value <= high else "ok",
                "value": float(value), "timestamp": utc.isoformat()}

    def summary(self, device_id):
        if device_id not in self.devices:
            raise ValueError("unknown device")
        readings = self.readings[device_id]
        if not readings:
            return {"device": device_id, "count": 0, "average": None, "alerts": 0}
        low, high = self.devices[device_id]
        return {"device": device_id, "count": len(readings),
                "average": math.fsum(v for _, v in readings) / len(readings),
                "alerts": sum(not low <= v <= high for _, v in readings)}
